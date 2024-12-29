#ifndef INITIALIZE_HPP
#define INITIALIZE_HPP
#include "message.hpp"
using json = nlohmann::json;

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
  /* response.result.capabilities.hoverProvider = true; */
  /* response.result.capabilities.definitionProvider = true; */
  /* response.result.capabilities.codeActionProvider = true; */
  /* response.result.capabilities.completionProvider = nlohmann::json::object();
   */

  response.result.serverInfo.name = "NLSP";
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
