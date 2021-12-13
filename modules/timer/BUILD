load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "endo_module_timer",
  srcs = [
    "timer.cc",
  ],
  hdrs = [
    "timer.h",
  ],
  copts = ["-std=c++17"],
  defines = ["V8_COMPRESS_POINTERS=1"],
  visibility = ["//visibility:public"],
  deps = [
    "//endo:endo",
  ],
)
