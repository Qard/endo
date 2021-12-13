workspace(name = "endojs")

# load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
# http_archive(
#     name = "bazel_skylib",
#     urls = [
#         "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.3/bazel-skylib-1.0.3.tar.gz",
#         "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.0.3/bazel-skylib-1.0.3.tar.gz",
#     ],
#     sha256 = "1c531376ac7e5a180e0237938a2536de0c54d93f5c278634818e0efc952dd56c",
# )
# load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")
# bazel_skylib_workspace()

load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

# local_repository(
#   name = "v8",
#   path = "deps/v8"
# )

# new_git_repository(
#   name = "zlib",
#   build_file = "//deps/v8:bazel/BUILD.zlib",
#   remote = "https://chromium.googlesource.com/chromium/src/third_party/zlib.git",
#   commit = "6da1d53b97c89b07e47714d88cab61f1ce003c68"
# )

new_git_repository(
  name = "gtest",
  remote = "git@github.com:google/googletest.git",
  build_file = "//:deps/gtest.BUILD",
  tag = "v1.10.x"
)

new_git_repository(
  name = "libuv",
  remote = "git@github.com:libuv/libuv.git",
  build_file = "//:deps/libuv.BUILD",
  tag = "v1.42.0"
)

new_local_repository(
  name = "v8",
  path = "/usr/local",
  build_file = "//:deps/v8.BUILD"
)
