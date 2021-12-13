cc_binary(
  name = "main",
  srcs = ["main.cc"],
  copts = ["-std=c++17"],
  defines = ["V8_COMPRESS_POINTERS=1"],
  deps = [
    "//endo:endo",
    "//modules/timer:endo_module_timer"
  ],
)
