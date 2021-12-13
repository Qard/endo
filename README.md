# jackson - Very experimental. Don't use this yet!

jackson is Yet-Another-JavaScript-Runtime™, this one focused on embedders and
providing a modular core. You can build in as much or as little modules
as you need or add your own to the build.

The name comes from the meme of referring to ESM files with the .mjs extension
as "Micheal Jackson Script". The jackson runtime is ESM-only and focused on
targetting modern features only, with a highly modular core that allows using
only the features you need. Legacy is history when you can include only the
features you need.

## Requirements

- bazel

## Build

Bazel is currently used for the build process. You can edit `main.cc`
however you want, add more modules by adding additional dependencies
in [BUILD](./main/BUILD) and build with:

```sh
bazel build //main:main
```

Or run the build directly with:

```sh
bazel run //main:main -- `pwd`/example.mjs --some --process --args
```

## Planned Future Changes

- [ ] Move `main` to a separate repo.
- [ ] Move modules to separate repos.
- [ ] Use Bazel 7 modules to link jackson with an entrypoint and modules.
- [ ] Add fs interface for embedders to provide virtual file systems.
- [ ] Remove dependency on `libuv` from jackson core?

## License

[see LICENSE file](./LICENSE)
