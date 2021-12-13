#pragma once

#include <v8.h>

namespace jackson {

using v8::Context;
using v8::Global;
using v8::Isolate;
using v8::Local;
using v8::Module;
using v8::String;

class ModuleWrap {
  Global<Module> module_;
  Global<String> url_;

 public:
  ModuleWrap(Isolate* isolate, Local<Module> module, Local<String> url);

  bool operator==(Local<Module> &mod);
  Local<Module> module();
  Local<String> url();
};

} // jackson
