#pragma once

#include <unordered_map>

#include "module_wrap.h"

namespace jackson {

using v8::Local;
using v8::MaybeLocal;
using v8::Module;
using v8::String;

class Environment;

class ModuleRegistry {
  std::unordered_multimap<int, ModuleWrap*> cache;
  Environment* env_;

 public:
  ModuleRegistry(Environment* env) : env_(env) {}
  void Add(Local<String> url, Local<Module> mod);
  MaybeLocal<Module> GetModule(Local<String> url);
  MaybeLocal<String> GetUrl(Local<Module> mod);
};

} // jackson

#include "env.h"
