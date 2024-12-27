#pragma once
#include <fstream>
#include <string>

class Logger {
private:
  std::string _fileName;
  std::ofstream _logFile;

public:
  Logger(std::string logFile);
  void log(std::string message);
  ~Logger();
};
