#pragma once

#include <array>
#include <sstream>
#include <string>

#include "absl/log/log_sink.h"
#include "absl/synchronization/mutex.h"

namespace principia {
namespace base {
namespace _string_log_sink {

class StringLogSink : public absl::LogSink {
 public:
  StringLogSink() = default;

  void Send(absl::LogEntry const& entry) override;
  void Flush() override;

  std::string logs() const;

 private:
  mutable absl::Mutex lock_;
  std::stringstream logs_ ABSL_GUARDED_BY(lock_);
};

}  // namespace _string_log_sink
}  // namespace base
}  // namespace principia
