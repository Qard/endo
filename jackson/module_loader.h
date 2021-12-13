#pragma once

#include <v8.h>

namespace jackson {

using v8::Local;
using v8::MaybeLocal;
using v8::Module;
using v8::String;

class Environment;

class ModuleLoader {
  Environment* env_;

  MaybeLocal<Module> Import(Local<String> resolved);
 public:
  ModuleLoader(Environment* env) : env_(env) {};

  MaybeLocal<Module> Import(Local<String> url, Local<String> parent);
  MaybeLocal<Module> Import(Local<String> url, Local<Module> parent);
  MaybeLocal<String> Resolve(Local<String> url, Local<String> parent = Local<String>());
  MaybeLocal<String> Resolve(Local<String> url, Local<Module> parent);
};

} // jackson

#include "env.h"
