#ifndef TEXTDOCUMENTDIDOPEN_HPP
#define TEXTDOCUMENTDIDOPEN_HPP
#include "message.hpp"
#include "textDocument.hpp"

struct _DidOpenTextDocumentParams {
  _TextDocumentItem textDocument;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_DidOpenTextDocumentParams, textDocument)
};

struct _DidOpenTextDocumentNotification {
  _Notification notification;
  _DidOpenTextDocumentParams params;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_DidOpenTextDocumentNotification, notification,
                                 params)
};

#endif
