//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_LOGGER_H_
#define UTILITY_INCLUDE_UTILITY_LOGGER_H_

#include <ctime>
#include <string>

enum LogLevel { kNONE, kINFO, kDEBUG };

class Logger {
 private:
  FILE *log_file_;
  time_t time_start_;
  LogLevel log_level_;

  inline void Add(LogLevel log_level, const std::string &log) {
    if (log_file_ != nullptr && log_level <= log_level_) {
      time_t time_now = time(nullptr);
      fprintf(log_file_, "Time: %ld, Message: %s\n",
              time_now - time_start_, log.c_str());
      fflush(log_file_);
    }
  }

 public:
  inline explicit Logger(LogLevel log_level, FILE *file) {
    log_level_ = log_level;
    log_file_ = file;
    time_start_ = time(nullptr);
  }

  inline ~Logger() {
    if (log_file_ != nullptr) {
      fflush(log_file_);
      fclose(log_file_);
    }
  }

  // template log
  template<typename... Args>
  inline void Add(LogLevel log_level, const std::string &format, Args... args) {
    size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args...);
    char bytes[size];
    snprintf(bytes, size, format.c_str(), args...);
    this->Add(log_level, std::string(bytes));
  }
};

#endif //UTILITY_INCLUDE_UTILITY_LOGGER_H_
