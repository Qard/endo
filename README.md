# endo - Very experimental. Don't use this yet!

endo is Yet-Another-JavaScript-Runtime™, this one focused on embedders and
providing a modular core. You can build in as much or as little modules
as you need or add your own to the build.

## Requirements

Currently the build process only supports macOS, however multi-paltform
support is intended eventually. The main blocker is getting the v8 build
process into bazel. Until a complete bazel build set up can be made, a
simplified process is used which assumes the latest stable v8 is already
built and installed as a system library somehow--homebrew is recommended
for that.

- v8 installed as a system library (at /usr/local/{include,lib})
- bazel

## Build

Bazel is currently used for the build process. You can edit `main.cc`
however you want, add more modules by adding additional dependencies
in [BUILD](./BUILD) and build with:

```sh
bazel build //:endo
```

Or run the build directly with:

```sh
bazel run //:endo -- `pwd`/example.mjs --some --process --args
```

The built binary can be found at `./bazel-out/darwin-fastbuild/bin/endo`.

Note that running an entrypoint file from argv automatically is temporary.
Eventually it will be up to the embedder to provide a file system interface
and an entrypoint script. The current behaviour is just a convenience during
early development.

## License

[see LICENSE file](./LICENSE)
