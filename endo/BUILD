load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "endo_base",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.h"]),
  copts = ["-std=c++17"],
  defines = ["V8_COMPRESS_POINTERS=1"],
  deps = [
    "@libuv//:libuv",
    "@v8//:v8",
    "@v8//:v8_libplatform"
  ],
)

cc_library(
  name = "endo",
  hdrs = [
    "env.h",
    # TODO: Make this private
    "util.h",
  ],
  visibility = ["//visibility:public"],
  deps = [
    ":endo_base"
  ],
)

# cc_test(
#   name = "test",
#   srcs = ["endo-test.cc"],
#   copts = ["-Iexternal/gtest/include"],
#   deps = [
#     "@gtest//:main",
#     ":endo",
#   ],
# )
