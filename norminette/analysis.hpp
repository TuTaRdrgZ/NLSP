#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP
#include "../logger/logger.hpp"
#include "../lsp/textDocumentDiagnostics.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

/* This function splits a string by a delimiter
 * @Params: const std::string &str, char delimiter
 * @Returns: std::vector<std::string>
 */
inline std::vector<std::string> split(const std::string &str, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(str);

  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

struct State {
  // Map of file names to contents
  std::map<std::string, std::string> Documents;
};

inline State NewState() {
  return State{.Documents = std::map<std::string, std::string>()};
}

/* This function runs a command and returns its output
 * @Params: const std::string &command
 * @Returns: std::string
 */
inline std::string executeCommand(const std::string &command) {
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
  if (!pipe)
    throw std::runtime_error("popen() failed!");
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

/* This function runs Norminette on a file and returns the diagnostics
 * @Params: Logger &logger, std::string filePath, const std::string &text
 * @Returns: std::vector<_Diagnostic>
 */
inline std::vector<_Diagnostic>
runNorminette(Logger &logger, std::string filePath, const std::string &text) {
  // If we have unsaved buffer data, create a temp file for Norminette to
  // process
  std::vector<_Diagnostic> diagnostics;
  if (!text.empty()) {
    char tempFileName[] = "/tmp/norminette";
    // append .c to the file name
    if (filePath.find(".c"))
      strcat(tempFileName, ".c");
    else
      strcat(tempFileName, ".h");
    mkstemp(tempFileName);
    logger.log("NewStatefile: " + std::string(tempFileName));

    std::ofstream tempFile(tempFileName);
    if (!tempFile.is_open()) {
      throw std::runtime_error("Failed to create temporary file");
    }
    tempFile << text;
    tempFile.close();
    filePath = tempFileName;

    logger.log("Running norminette on " + filePath);

    // Execute Norminette
    std::string command = "norminette " + filePath;
    std::string output;
    try {
      output = executeCommand(command);
    } catch (const std::runtime_error &e) {
      logger.log("Failed to run Norminette: " + std::string(e.what()));
    }
    logger.log("Norminette output: " + output);

    // Parse Norminette output
    std::istringstream stream(output);
    std::string line;
    while (std::getline(stream, line)) {
      std::vector<std::string> match = split(line, ':');
      if (match.size() < 3) {
        continue;
      }
      int row = std::stoi(match[2]);
      int col = std::stoi(match[3]);
      std::string message = match[4];

      logger.log("Norminette output line: " + line);
      logger.log("Row: " + std::to_string(row));
      logger.log("Col: " + std::to_string(col));
      logger.log("Message: " + message);

      diagnostics.push_back(
          _Diagnostic{{row - 1, col - 4, row - 1,
                       col - 4},    // Adjusting 1-based to 0-based indexing
                      1,            // Severity: Error
                      "norminette", // Source
                      message});    // Message
    }
    executeCommand("rm " + filePath);
  }

  return diagnostics;
}

inline std::vector<_Diagnostic>
getDiagnosticsForFile(Logger &logger, const std::string &filePath,
                      const std::string &text) {
  logger.log("Text: " + text);
  try {
    return runNorminette(logger, filePath, text);
  } catch (const std::exception &e) {
    std::cerr << "Failed to run Norminette: " << e.what() << std::endl;
    return {};
  }
}

inline std::vector<_Diagnostic>
OpenDocument(Logger &log, State state, std::string uri, std::string text) {
  state.Documents[uri] = text;
  return getDiagnosticsForFile(log, uri, text);
}

inline std::vector<_Diagnostic>
UpdateDocument(Logger &log, State state, std::string uri, std::string text) {
  state.Documents[uri] = text;
  return getDiagnosticsForFile(log, uri, text);
}

#endif
