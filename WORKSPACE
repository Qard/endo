workspace(name = "jackson")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")

# new_git_repository(
#   name = "zlib",
#   build_file = "//deps/v8:bazel/BUILD.zlib",
#   remote = "https://chromium.googlesource.com/chromium/src/third_party/zlib.git",
#   commit = "6da1d53b97c89b07e47714d88cab61f1ce003c68"
# )

git_repository(
  name = "libuv",
  remote = "git@github.com:libuv/libuv.git",
  build_file = "//:deps/BUILD.libuv",
  tag = "v1.42.0"
)

#
# V8
#

http_archive(
  name = "rules_python",
  sha256 = "c68bdc4fbec25de5b5493b8819cfc877c4ea299c0dcb15c244c5a00208cde311",
  strip_prefix = "rules_python-0.31.0",
  url = "https://github.com/bazelbuild/rules_python/releases/download/0.31.0/rules_python-0.31.0.tar.gz",
)

load("@rules_python//python:repositories.bzl", "py_repositories", "python_register_toolchains")

py_repositories()

python_register_toolchains(
  name = "python3_12",
  ignore_root_user_error = True,
  # https://github.com/bazelbuild/rules_python/blob/main/python/versions.bzl
  python_version = "3.12",
)

load("@python3_12//:defs.bzl", "interpreter")
load("@rules_python//python:pip.bzl", "pip_parse")

git_repository(
  name = "v8",
  remote = "git@github.com:v8/v8.git",
  branch = "12.3.219.10"
)

pip_parse(
  name = "v8_python_deps",
  extra_pip_args = ["--require-hashes"],
  python_interpreter_target = interpreter,
  requirements_lock = "@v8//:bazel/requirements.txt",
)

load("@v8_python_deps//:requirements.bzl", v8_python_deps_install = "install_deps")

v8_python_deps_install()

git_repository(
  name = "com_googlesource_chromium_base_trace_event_common",
  build_file = "@v8//:bazel/BUILD.trace_event_common",
  commit = "29ac73db520575590c3aceb0a6f1f58dda8934f6",
  remote = "https://chromium.googlesource.com/chromium/src/base/trace_event/common.git",
)

bind(
  name = "base_trace_event_common",
  actual = "@com_googlesource_chromium_base_trace_event_common//:trace_event_common",
)

git_repository(
  name = "com_google_absl",
  remote = "https://chromium.googlesource.com/chromium/src/third_party/abseil-cpp.git",
  commit = "9280ce24a8ceedc38c0b82678997ac126a5aee8b"
)

bind(
  name = "absl_optional",
  actual = "@com_google_absl//absl/types:optional"
)

bind(
  name = "absl_btree",
  actual = "@com_google_absl//absl/container:btree"
)

bind(
  name = "absl_flat_hash_map",
  actual = "@com_google_absl//absl/container:flat_hash_map"
)

bind(
  name = "absl_flat_hash_set",
  actual = "@com_google_absl//absl/container:flat_hash_set"
)
