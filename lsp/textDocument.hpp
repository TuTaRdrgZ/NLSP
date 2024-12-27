#ifndef TEXTDOCUMENT_HPP
#define TEXTDOCUMENT_HPP

#include <nlohmann/json.hpp>

struct _TextDocumentItem {
  std::string uri;
  std::string languageId;
  int version;
  std::string text;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_TextDocumentItem, uri, languageId, version,
                                 text);
};

struct _TextDocumentIdentifier {
  std::string uri;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_TextDocumentIdentifier, uri);
};

struct _VersionTextDocumentIdentifier {
  _TextDocumentIdentifier textDocument;
  int version;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(_VersionTextDocumentIdentifier, textDocument,
                                 version);
};

#endif
