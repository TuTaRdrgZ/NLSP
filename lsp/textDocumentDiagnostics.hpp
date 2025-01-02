#ifndef TEXTDOCUMENTDIAGNOSTICS_HPP
#define TEXTDOCUMENTDIAGNOSTICS_HPP

#include "message.hpp"
#include <json.hpp>

struct _Position {
  int line;
  int character;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_Position, line, character)
};

struct _Range {
  _Position start;
  _Position end;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_Range, start, end);
};

struct _Diagnostic {
  _Range range;
  int severity;
  std::string source;
  std::string message;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_Diagnostic, range, severity, source, message)
};

struct _PublishDiagnosticsParams {
  std::string uri;
  std::vector<_Diagnostic> diagnostics;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_PublishDiagnosticsParams, uri, diagnostics)
};

struct _PublishDiagnosticsNotification {
  _Notification notification;
  _PublishDiagnosticsParams params;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_PublishDiagnosticsNotification, notification,
                                 params)
};

#endif
