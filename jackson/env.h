#pragma once

#include <libplatform/libplatform.h>
#include <v8.h>
#include <uv.h>

#include "module_registry.h"
#include "module_loader.h"
#include "module_wrap.h"

namespace jackson {

using v8::Context;
using v8::Global;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::Maybe;
using v8::MaybeLocal;
using v8::Module;
using v8::Platform;

// TODO: extract isolate/context stuff for separate workers
class Environment {
  ModuleRegistry module_registry;
  ModuleLoader module_loader;
  std::unique_ptr<Platform> platform_;
  Isolate* isolate_;
  Global<Context> context_;
  uv_loop_t loop_;
  std::string entrypoint;
  std::vector<char*> args;

public:
  class Scope {
    HandleScope handle_scope;
    Context::Scope context_scope;
   public:
    Scope(Environment* env)
      : handle_scope(env->Isolate()),
        context_scope(env->Context()) {}
  };

  Environment(int argc, char* argv[]);
  ~Environment();

  void SyntheticModule(Local<String> name, Local<Module> module);

  ModuleRegistry* ModuleRegistry();
  ModuleLoader* ModuleLoader();
  Local<Context> Context();
  Isolate* Isolate();
  uv_loop_t* Loop();
  std::vector<char*> Args();

  void Tick();
  int Run();
};

}
