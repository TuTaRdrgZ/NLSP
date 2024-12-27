#include "logger.hpp"

Logger::Logger(std::string fileName) : _fileName(fileName) {
  // open the log file
  _logFile.open(fileName);
  if (!_logFile.is_open()) {
    throw std::runtime_error("Hey you didn't provide a valid file name");
  }
}

void Logger::log(std::string message) {
  // write the message to the log file
  _logFile << message << std::endl;
}

Logger::~Logger() {
  // close the log file
  _logFile.close();
}
