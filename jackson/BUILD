load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "jackson",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.h"]),
  copts = ["-std=c++17"],
  defines = ["V8_COMPRESS_POINTERS=1"],
  visibility = ["//visibility:public"],
  deps = [
    "@libuv//:libuv",
    "@v8//:v8_noicu"
  ],
)

# cc_test(
#   name = "test",
#   srcs = ["jackson-test.cc"],
#   copts = ["-Iexternal/gtest/include"],
#   deps = [
#     "@gtest//:main",
#     "//jackson:jackson",
#   ],
# )
