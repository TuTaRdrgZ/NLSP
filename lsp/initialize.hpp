#ifndef INITIALIZE_HPP
#define INITIALIZE_HPP
#include "message.hpp"
using json = nlohmann::json;
/* #include <optional> */

/* struct ClientInfo { */
/*   std::string name; */
/*   std::string version; */
/**/
/*   // Custom serialization and deserialization */
/*   friend void to_json(nlohmann::json &j, const ClientInfo &ci) { */
/*     j = nlohmann::json{{"name", ci.name}, {"version", ci.version}}; */
/*   } */
/**/
/*   friend void from_json(const nlohmann::json &j, ClientInfo &ci) { */
/*     j.at("name").get_to(ci.name); */
/*     j.at("version").get_to(ci.version); */
/*   } */
/* }; */
/**/
/* struct InitializeRequestParams { */
/*   std::optional<ClientInfo> clientInfo; */
/**/
/*   // Custom serialization and deserialization */
/*   friend void to_json(nlohmann::json &j, const InitializeRequestParams &irp)
 * { */
/*     if (irp.clientInfo) { */
/*       j["clientInfo"] = *irp.clientInfo; */
/*     } */
/*   } */
/**/
/*   friend void from_json(const nlohmann::json &j, InitializeRequestParams
 * &irp) { */
/*     if (j.contains("clientInfo")) { */
/*       irp.clientInfo = j.at("clientInfo").get<ClientInfo>(); */
/*     } else { */
/*       irp.clientInfo = std::nullopt; */
/*     } */
/*   } */
/* }; */

struct _ClientInfo {
  std::string name;
  std::string version;

  // serialization and deserialization
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_ClientInfo, name, version)
};

struct _InitializeRequestParams {
  _ClientInfo clientInfo;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_InitializeRequestParams, clientInfo)
};

struct _InitializeRequest {
  _Request request;
  _InitializeRequestParams params;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_InitializeRequest, request, params);
};

struct _ServerCapabilities {
  int textDocumentSync;
  bool hoverProvider;
  bool definitionProvider;
  bool codeActionProvider;
  json completionProvider;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_ServerCapabilities, textDocumentSync,
                                 hoverProvider, definitionProvider,
                                 codeActionProvider);
};

struct _ServerInfo {
  std::string name;
  std::string version;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_ServerInfo, name, version);
};

struct _InitializeResult {
  _ServerCapabilities capabilities;
  _ServerInfo serverInfo;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_InitializeResult, capabilities, serverInfo);
};

struct _InitializeResponse {
  _Response response;
  _InitializeResult result;
};

/* This function creates a new InitializeResponse and
 * fills the response field with the given id and the RPC version
 * @Params: int id
 * @Return: InitializeResponse
 */
inline _InitializeResponse NewInitializeResponse(int id) {
  _InitializeResponse response;
  response.response.jsonrpc = "2.0";
  response.response.id = id;

  response.result.capabilities.textDocumentSync = 1;
  response.result.capabilities.hoverProvider = true;
  response.result.capabilities.definitionProvider = true;
  response.result.capabilities.codeActionProvider = true;
  response.result.capabilities.completionProvider = nlohmann::json::object();
  // Empty object for CompletionProvider

  response.result.serverInfo.name = "norminette-lsp";
  response.result.serverInfo.version = "0.0.0.1-testing";

  return response;
}

/* This function serializes the InitializeResponse struct
 * @Params: nlohmann::json &j, const _InitializeResponse &response
 * @Return: void
 */
inline void to_json(nlohmann::json &j, const _InitializeResponse &response) {
  j = nlohmann::json{{"jsonrpc", response.response.jsonrpc},
                     {"id", response.response.id},
                     {"result", response.result}};
}
#endif
