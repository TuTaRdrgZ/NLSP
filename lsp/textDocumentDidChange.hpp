#ifndef TEXTDOCUMENTDIDCHANGE_HPP
#define TEXTDOCUMENTDIDCHANGE_HPP
#include "message.hpp"
#include "textDocument.hpp"

struct _TextDocumentContentChangeEvent {
  std::string text;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_TextDocumentContentChangeEvent, text);
};

struct _DidChangeTextDocumentParams {
  _VersionTextDocumentIdentifier textDocument;
  std::vector<_TextDocumentContentChangeEvent> contentChanges;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_DidChangeTextDocumentParams, textDocument,
                                 contentChanges);
};

struct _TextDocumentDidChangeNotification {
  _Notification notification;
  _DidChangeTextDocumentParams params;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_TextDocumentDidChangeNotification,
                                 notification, params);
};

#endif
