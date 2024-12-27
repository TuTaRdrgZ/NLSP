#include "logger/logger.hpp"
#include "lsp/initialize.hpp"
#include "lsp/textDocumentDiagnostics.hpp"
#include "lsp/textDocumentDidChange.hpp"
#include "lsp/textDocumentDidOpen.hpp"
#include "norminette/analysis.hpp"
#include "rpc/rpc.hpp"
#include <iostream>
#include <string>

void handleMessage(Logger &log, State state, std::string &method,
                   std::vector<uint8_t> &contents);
std::optional<std::vector<uint8_t>> readFromStdin(std::vector<uint8_t> &buffer);
void writeResponse(Logger &log, json &response);

int main() {
  Logger log(
      "/home/tuta/tuta/coding/randomProjects/lsp/norminette-lsp-cpp/log.txt");
  log.log("Hey I started");
  std::vector<uint8_t> buffer;
  buffer.reserve(1024);
  State state = NewState();

  while (true) {
    auto inputResult = readFromStdin(buffer);
    if (!inputResult) {
      break; // No more data in `stdin`
    }

    while (true) {
      auto splitResult = Split(buffer);
      if (!splitResult) {
        break; // No more tokes available
      }

      auto [advance, token, err] = *splitResult;
      if (!err->empty()) {
        log.log("Error in Split: " + *err);
        buffer.erase(buffer.begin(), buffer.begin() + advance);
        continue;
      }

      auto [method, contents, decodeError] = DecodeMessage(token);
      if (!decodeError->empty()) {
        log.log("Error in Decode: " + *decodeError);
        buffer.erase(buffer.begin(), buffer.begin() + advance);
        continue;
      }

      handleMessage(log, state, method, contents);
      buffer.erase(buffer.begin(), buffer.begin() + advance);
    }
  }

  return 0;
}

/* This function checks the method and calls the appropriate function
 * and sends the response
 * @Params: Logger &log, std::string &method, std::vector<uint8_t> &contents
 * @Return: void
 */
void handleMessage(Logger &log, State state, std::string &method,
                   std::vector<uint8_t> &contents) {
  log.log("received msg with method: " + method);
  if (method == "initialize") {

    _InitializeRequest request;
    json j = json::parse(contents);

    request.params.clientInfo.name = j["params"]["clientInfo"]["name"];
    request.params.clientInfo.version = j["params"]["clientInfo"]["version"];
    request.request.id = j["id"];

    // create response
    json jsonResponse = NewInitializeResponse(request.request.id);
    writeResponse(log, jsonResponse);

  } else if (method == "textDocument/didOpen") {

    _DidOpenTextDocumentNotification request;
    json j = json::parse(contents);

    log.log("Contents: " + std::string(contents.begin(), contents.end()));

    request.params.textDocument.uri = j["params"]["textDocument"]["uri"];
    request.params.textDocument.text = j["params"]["textDocument"]["text"];

    state.Documents[request.params.textDocument.uri] =
        request.params.textDocument.uri;
    state.Documents[request.params.textDocument.text] =
        request.params.textDocument.text;

    json diagnostics = OpenDocument(log, state, request.params.textDocument.uri,
                                    request.params.textDocument.text);
    nlohmann::json message = {{"jsonrpc", "2.0"},
                              {"method", "textDocument/publishDiagnostics"},
                              {"params",
                               {{"uri", request.params.textDocument.uri},
                                {"diagnostics", diagnostics}}}};
    writeResponse(log, message);

  } else if (method == "textDocument/didChange") {

    _TextDocumentDidChangeNotification request;
    json j = json::parse(contents);

    request.params.textDocument.textDocument.uri =
        j["params"]["textDocument"]["uri"];
    state.Documents[request.params.textDocument.textDocument.uri] =
        request.params.textDocument.textDocument.uri;

    for (auto &change : j["params"]["contentChanges"]) {

      json diagnostics = UpdateDocument(
          log, state, request.params.textDocument.textDocument.uri,
          change["text"]);
      nlohmann::json message = {
          {"jsonrpc", "2.0"},
          {"method", "textDocument/publishDiagnostics"},
          {"params",
           {{"uri", request.params.textDocument.textDocument.uri},
            {"diagnostics", diagnostics}}}};
      writeResponse(log, message);
    }
  } else {
    log.log("Method not found: " + method);
  }
}

/*
 * This function writes the response to stdout
 * @Params: Logger &log, std::string &response
 * @Return: void
 */
void writeResponse(Logger &log, json &response) {
  std::string responseStr = EncodeMessage(response);
  std::cout << responseStr;
  log.log("Sent the reply: " + responseStr);
}

/* This function reads from stdin and returns the buffer
 * @Params: std::vector<uint8_t> buffer
 * @Return: std::optional<std::vector<uint8_t>>
 */
std::optional<std::vector<uint8_t>>
readFromStdin(std::vector<uint8_t> &buffer) {
  char c;
  if (std::cin.get(c)) {
    buffer.push_back(static_cast<uint8_t>(c));
    return buffer;
  }
  return std::nullopt; // end of input
}
