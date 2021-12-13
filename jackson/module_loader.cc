#include <filesystem>
#include <iostream>

#include <uv.h>

#include "module_loader.h"
#include "util.h"

using v8::MaybeLocal;
using v8::ScriptCompiler;
using v8::ScriptOrigin;

Local<String> cwd(Isolate* isolate) {
  size_t size = 1024;
  char cwd[size];

  int err = uv_cwd(cwd, &size);
  if (err == UV_ENOBUFS) {
    return Local<String>();
  }

  return str(isolate, cwd);
}

// NOTE: std::string implicitly converts to and from a path
std::string BaseName(std::filesystem::path path) {
  return std::filesystem::canonical(path.parent_path()).string();
}

Local<String> BaseName(Local<String> path) {
  Isolate* isolate = Isolate::GetCurrent();
  return str(isolate, BaseName(*String::Utf8Value(isolate, path)));
}

// TODO: Convert to libuv
MaybeLocal<String> ReadFile(Isolate* isolate, Local<String> path) {
  String::Utf8Value filename(isolate, path);

  // ifstream::ate needed to get file size
  std::ifstream file;
  file.open(*filename, std::ifstream::ate);
  if (!file) {
    std::string message = "File not found: ";
    message += *filename;
    isolate->ThrowError(str(isolate, message.c_str()));
    return MaybeLocal<String>();
  }

  // Get file size and seek to start of file (ifstream::ate starts at the end)
  size_t file_size = file.tellg();
  file.seekg(0);

  // Reading file to char array and returing it
  std::filebuf* file_buf = file.rdbuf();
  char contents[file_size + 1];
  contents[file_size] = '\0';
  file_buf->sgetn(contents, file_size);
  file.close();

  return str(isolate, contents);
}

namespace jackson {

MaybeLocal<Module> ModuleLoader::Import(Local<String> resolved) {
  MaybeLocal<Module> maybe_mod = env_->ModuleRegistry()->GetModule(resolved);
  if (!maybe_mod.IsEmpty()) {
    return maybe_mod;
  }

  Local<String> data;
  if (!ReadFile(env_->Isolate(), resolved).ToLocal(&data)) {
    return MaybeLocal<Module>();
  }

  // TODO: add source map support
  Local<Value> source_map_url = Local<Value>();
  ScriptOrigin origin(resolved, 0, 0, true, -1, source_map_url, false,
                      false /* is wasm */, true /* is module */);

  ScriptCompiler::Source source(data, origin);

  Local<Module> mod;
  maybe_mod = ScriptCompiler::CompileModule(env_->Isolate(), &source);
  if (!maybe_mod.ToLocal(&mod)) {
    return MaybeLocal<Module>();
  }

  // NOTE: This needs to happen here so intermediate module state
  // is available before any InstantiateModule callback completes.
  env_->ModuleRegistry()->Add(resolved, mod);

  return maybe_mod;
}

MaybeLocal<Module> ModuleLoader::Import(Local<String> url, Local<String> parent) {
  Local<String> resolved;
  if (!Resolve(url, parent).ToLocal(&resolved)) {
    return MaybeLocal<Module>();
  }

  return Import(resolved);
}

MaybeLocal<Module> ModuleLoader::Import(Local<String> url, Local<Module> parent) {
  Local<String> resolved;
  if (!Resolve(url, parent).ToLocal(&resolved)) {
    return MaybeLocal<Module>();
  }

  return Import(resolved);
}

MaybeLocal<String> ModuleLoader::Resolve(Local<String> url, Local<String> parent) {
  namespace fs = std::filesystem;
  Isolate* isolate = env_->Isolate();

  if (url.IsEmpty()) {
    isolate->ThrowError("Must provide url to resolve");
    return MaybeLocal<String>();
  }

  // If the url is already fully-formed and registered, just return it
  if (!env_->ModuleRegistry()->GetModule(url).IsEmpty()) {
    return url;
  }

  // If no parent specified compute base from current working directory,
  // otherwise get the basename of the parent module absolute path.
  Local<String> base = parent.IsEmpty() ? cwd(isolate) : BaseName(parent);

  fs::path base_string = *String::Utf8Value(isolate, base);
  fs::path path_string = *String::Utf8Value(isolate, url);
  return str(isolate, fs::weakly_canonical(base_string / path_string));
}

// Helper because InstantiateModule receives the module object without Url
// so we have to get the url from the module registry first.
MaybeLocal<String> ModuleLoader::Resolve(Local<String> url, Local<Module> parent) {
  Local<String> referrerUrl;
  if (!env_->ModuleRegistry()->GetUrl(parent).ToLocal(&referrerUrl)) {
    return MaybeLocal<String>();
  }

  return Resolve(url, referrerUrl);
}

} // jackson
