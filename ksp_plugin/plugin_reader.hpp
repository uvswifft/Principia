#pragma once

#include <future>
#include <functional>
#include <memory>
#include <string>

#include "absl/synchronization/mutex.h"
#include "base/not_null.hpp"
#include "base/string_log_sink.hpp"
#include "google/protobuf/arena.h"
#include "ksp_plugin/plugin.hpp"

namespace principia {
namespace ksp_plugin {
namespace _plugin_reader {
namespace internal {

using namespace principia::base::_not_null;
using namespace principia::base::_string_log_sink;
using namespace principia::ksp_plugin::_plugin;

class PluginReader {
 public:
  // `message` must be allocated on `arena`. `arena` is reset once the plugin
  // has been read.
  PluginReader(
      serialization::Plugin const& message,
      google::protobuf::Arena& arena);

  std::unique_ptr<Plugin> get();

  // The result is owned by this object and is is not mutated until the next
  // call to `logs()`.
  std::string const& logs();

 private:
  StringLogSink logs_;
  std::string logs_snapshot_;
  absl::Mutex lock_;
  std::jthread reader_;
  std::unique_ptr<Plugin> result_ ABSL_GUARDED_BY(lock_);
};

}  // namespace internal

using internal::PluginReader;

}  // namespace _plugin_reader
}  // namespace ksp_plugin
}  // namespace principia