#include "ksp_plugin/plugin_reader.hpp"

#include "absl/log/log_sink_registry.h"
#include "base/stoppable_thread.hpp"

namespace principia {
namespace ksp_plugin {
namespace _plugin_reader {
namespace internal {

using namespace principia::base::_stoppable_thread;

PluginReader::PluginReader(serialization::Plugin const& message,
                           google::protobuf::Arena& arena) {
  reader_ = MakeStoppableThread([this, &message, &arena]() {
    absl::AddLogSink(&logs_);
    auto result = Plugin::ReadFromMessage(message);
    arena.Reset();
    absl::RemoveLogSink(&logs_);
    absl::MutexLock l(lock_);
    result_ = std::move(result);
  });
}

std::unique_ptr<Plugin> PluginReader::get() {
  absl::MutexLock l(lock_);
  return std::move(result_);
}

std::string const& PluginReader::logs() {
  logs_snapshot_ = logs_.logs();
  int tail_lines = 0;
  auto tail_start = logs_snapshot_.rbegin();
  for (; tail_start != logs_snapshot_.rend(); ++tail_start) {
    if (*tail_start == '\n') {
      ++tail_lines;
    }
    if (tail_lines == 10) {
      break;
    }
  }
  logs_snapshot_ =
      logs_snapshot_.substr(tail_start.base() - logs_snapshot_.begin());
  return logs_snapshot_;
}

}  // namespace internal
}  // namespace _plugin_future
}  // namespace ksp_plugin
}  // namespace principia