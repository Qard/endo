v8_headers = [
  "include/v8-array-buffer.h",
  "include/v8-callbacks.h",
  "include/v8-container.h",
  "include/v8-context.h",
  "include/v8-cppgc.h",
  "include/v8-data.h",
  "include/v8-date.h",
  "include/v8-debug.h",
  "include/v8-embedder-heap.h",
  "include/v8-exception.h",
  "include/v8-extension.h",
  "include/v8-external.h",
  "include/v8-fast-api-calls.h",
  "include/v8-forward.h",
  "include/v8-function-callback.h",
  "include/v8-function.h",
  "include/v8-initialization.h",
  # "include/v8-inspector-protocol.h",
  # "include/v8-inspector.h",
  "include/v8-internal.h",
  "include/v8-isolate.h",
  "include/v8-json.h",
  "include/v8-local-handle.h",
  "include/v8-locker.h",
  "include/v8-maybe.h",
  "include/v8-memory-span.h",
  "include/v8-message.h",
  # "include/v8-metrics.h",
  "include/v8-microtask-queue.h",
  "include/v8-microtask.h",
  "include/v8-object.h",
  "include/v8-persistent-handle.h",
  "include/v8-platform.h",
  "include/v8-primitive-object.h",
  "include/v8-primitive.h",
  "include/v8-profiler.h",
  "include/v8-promise.h",
  "include/v8-proxy.h",
  "include/v8-regexp.h",
  "include/v8-script.h",
  "include/v8-snapshot.h",
  "include/v8-statistics.h",
  "include/v8-template.h",
  "include/v8-traced-handle.h",
  "include/v8-typed-array.h",
  # "include/v8-unwinder-state.h",
  "include/v8-unwinder.h",
  "include/v8-util.h",
  # "include/v8-value-serializer-version.h",
  "include/v8-value-serializer.h",
  "include/v8-value.h",
  # "include/v8-version-string.h",
  # "include/v8-version.h",
  "include/v8-wasm.h",
  "include/v8-weak-callback-info.h",
  "include/v8.h",
]

cc_library(
  name = "v8_headers",
  copts = ["-std=c++14"],
  strip_include_prefix = "include",
  visibility = ["//visibility:public"],
  hdrs = v8_headers
)

cc_library(
  name = "v8",
  copts = ["-std=c++14"],
  strip_include_prefix = "include",
  visibility = ["//visibility:public"],
  srcs = ["lib/libv8.dylib"],
  hdrs = v8_headers,
)

cc_library(
  name = "v8_libbase",
  srcs = ["lib/libv8_libbase.dylib"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "v8_libplatform",
  strip_include_prefix = "include",
  hdrs = [
    "include/libplatform/libplatform-export.h",
    "include/libplatform/libplatform.h",
    "include/libplatform/v8-tracing.h",
    "include/cppgc/common.h",
    "include/cppgc/ephemeron-pair.h",
    "include/cppgc/heap-state.h",
    "include/cppgc/liveness-broker.h",
    "include/cppgc/object-size-trait.h",
    "include/cppgc/process-heap-statistics.h",
    "include/cppgc/trace-trait.h",
    "include/cppgc/cross-thread-persistent.h",
    "include/cppgc/explicit-management.h",
    "include/cppgc/heap-statistics.h",
    "include/cppgc/macros.h",
    "include/cppgc/persistent.h",
    "include/cppgc/sentinel-pointer.h",
    "include/cppgc/type-traits.h",
    "include/cppgc/custom-space.h",
    "include/cppgc/garbage-collected.h",
    "include/cppgc/heap.h",
    "include/cppgc/member.h",
    "include/cppgc/platform.h",
    "include/cppgc/source-location.h",
    "include/cppgc/visitor.h",
    "include/cppgc/allocation.h",
    "include/cppgc/default-platform.h",
    "include/cppgc/heap-consistency.h",
    "include/cppgc/internal/api-constants.h",
    "include/cppgc/internal/caged-heap-local-data.h",
    "include/cppgc/internal/finalizer-trait.h",
    "include/cppgc/internal/logging.h",
    "include/cppgc/internal/persistent-node.h",
    "include/cppgc/internal/prefinalizer-handler.h",
    "include/cppgc/internal/atomic-entry-flag.h",
    "include/cppgc/internal/compiler-specific.h",
    "include/cppgc/internal/gc-info.h",
    "include/cppgc/internal/name-trait.h",
    "include/cppgc/internal/pointer-policies.h",
    "include/cppgc/internal/write-barrier.h",
    "include/cppgc/name-provider.h",
    "include/cppgc/prefinalizer.h",
    "include/cppgc/testing.h",
    "include/v8config.h",
    "include/v8-version.h"
  ],
  srcs = [
    "lib/libv8_libplatform.dylib"
  ],
  visibility = ["//visibility:public"],
  deps = [":v8"]
)
