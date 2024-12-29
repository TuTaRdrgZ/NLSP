#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <nlohmann/json.hpp>

struct _Request {
  std::string rpc;
  int id;
  std::string method;

  // Macro to serialize or deserialize the struct
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_Request, rpc, id, method);
};

struct _Response {
  std::string jsonrpc;
  int id;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_Response, jsonrpc, id);
};

struct _Notification {
  std::string rpc;
  std::string method;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_Notification, rpc, method);
};

#endif
