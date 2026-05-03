#include "base/string_log_sink.hpp"

#include <exception>
#include <sstream>
#include <string>
#include <utility>

#include "absl/log/check.h"
#include "absl/time/time.h"
#include "base/macros.hpp"  // 🧙 For OS_WIN.

namespace principia {
namespace base {
namespace _string_log_sink {

void StringLogSink::Send(absl::LogEntry const& entry) {
  absl::MutexLock l(lock_);
  // FATAL messages are sent twice, first without and then with the
  // stacktrace.  Log the message only once.
  if (entry.stacktrace().empty()) {
    logs_ << entry.text_message_with_prefix_and_newline();
  } else {
    logs_ << entry.stacktrace();
  }
}

std::string StringLogSink::logs() const {
  absl::MutexLock l(lock_);
  return logs_.str();
}

void StringLogSink::Flush() {
  absl::MutexLock l(lock_);
  logs_.flush();
}

}  // namespace _string_log_sink
}  // namespace base
}  // namespace principia
