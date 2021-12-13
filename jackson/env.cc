#include <iostream>

#include <assert.h>
#include <unistd.h>
#include <unordered_map>

#include "util.h"
#include "env.h"

using v8::Array;
using v8::ArrayBuffer;
using v8::Context;
using v8::Data;
using v8::FixedArray;
using v8::FunctionCallbackInfo;
using v8::Global;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::Maybe;
using v8::MaybeLocal;
using v8::MicrotaskQueue;
using v8::Module;
using v8::ModuleRequest;
using v8::Object;
using v8::ObjectTemplate;
using v8::Platform;
using v8::Promise;
using v8::ScriptCompiler;
using v8::ScriptOrigin;
using v8::ScriptOrModule;
using v8::SealHandleScope;
using v8::String;
using v8::TryCatch;
using v8::V8;
using v8::Value;

static MaybeLocal<Module> ModuleResolveCallback(
    Local<Context> context,
    Local<String> specifier,
    Local<FixedArray> import_attributes,
    Local<Module> referrer) {
  jackson::Environment* env = Env(context);

  return env->ModuleLoader()->Import(specifier, referrer);
}

Local<Value> RunModule(Local<Context> context, Local<Module> mod, bool isImport = false) {
  jackson::Environment* env = Env(context);

  if (mod->InstantiateModule(context, ModuleResolveCallback).IsNothing()) {
    if (mod->GetStatus() == Module::kErrored) {
      env->Isolate()->ThrowException(mod->GetException());
    }
    return Local<Value>();
  }

  Local<Value> result;
  if (!mod->Evaluate(context).ToLocal(&result)) {
    env->Isolate()->ThrowError("Failed to evaluate module");
    return Local<Value>();
  }

  // TODO: is this the correct place for this to support top-level await?
  Local<Promise> result_promise = result.As<Promise>();
  while (result_promise->State() == Promise::kPending) {
    env->Tick();
  }

  if (isImport) {
    return mod->GetModuleNamespace();
  } else {
    return result;
  }
}

Local<Value> CompileAndRun(Local<Context> context, Local<String> url, Local<String> parent = Local<String>(), bool isImport = false) {
  jackson::Environment *env = Env(context);

  Local<Module> mod;
  if (!env->ModuleLoader()->Import(url, parent).ToLocal(&mod)) {
    return Local<Value>();
  }

  return RunModule(context, mod, isImport);
}

MaybeLocal<Promise> DynamicImportCallback(
    Local<Context> context,
    Local<Data> host_defined_options,
    Local<Value> resource_name,
    Local<String> specifier,
    Local<FixedArray> import_assertions) {
  jackson::Environment* env = Env(context);

  TryCatch try_catch(env->Isolate());
  Local<String> parent = resource_name.As<String>();
  Local<Value> result = CompileAndRun(context, specifier, parent, true);
  if (try_catch.HasCaught() && !try_catch.HasTerminated()) {
    return Reject(context, try_catch.Exception());
  }

  return Resolve(context, result);
}

void MetaCallback(Local<Context> context, Local<Module> module, Local<Object> meta) {
  jackson::Environment* env = Env(context);

  Local<String> url;
  if (env->ModuleRegistry()->GetUrl(module).ToLocal(&url)) {
    USE(meta->Set(context, intern(env->Isolate(), "url"), url));
  }
}

void AttachModuleSystem(Isolate* isolate) {
  isolate->SetHostImportModuleDynamicallyCallback(DynamicImportCallback);
  isolate->SetHostInitializeImportMetaObjectCallback(MetaCallback);
}

bool RunMain(Isolate* isolate, std::string path) {
  jackson::Environment* env = Env(isolate);

  TryCatch try_catch(isolate);
  try_catch.SetVerbose(true);

  Local<String> file = str(isolate, path.c_str());
  Local<Value> result = CompileAndRun(env->Context(), file);
  if (try_catch.HasCaught() && !try_catch.HasTerminated()) {
    ReportException(isolate, &try_catch);
    return false;
  }

  return result.As<Promise>()->State() == Promise::kFulfilled;
}

bool ProcessMessages(Isolate* isolate, Platform* platform) {
  SealHandleScope shs(isolate);

  while (v8::platform::PumpMessageLoop(platform, isolate)) {
    isolate->PerformMicrotaskCheckpoint();
  }

  if (platform->IdleTasksEnabled(isolate)) {
    v8::platform::RunIdleTasks(platform, isolate, 1.0 / 1000);
  }

  return true;
}

static void Print(const FunctionCallbackInfo<Value>& args) {
  for (int i = 0; i < args.Length(); i++) {
    HandleScope handle_scope(args.GetIsolate());
    String::Utf8Value s(args.GetIsolate(), args[i]);
    std::cout << (i ? " " : "") << *s;
  }
  std::cout << std::endl;
}

static void GetArgv(Local<String> property, const v8::PropertyCallbackInfo<Value>& info) {
  jackson::Environment* env = Env(info);

  std::vector<char*> args = env->Args();
  std::vector<Local<Value>> argv;
  argv.reserve(args.size());

  for (size_t i = 0; i < args.size(); i++) {
    argv.emplace_back(str(env->Isolate(), args[i]));
  }

  info.GetReturnValue().Set(Array::New(env->Isolate(), argv.data(), args.size()));
}

Local<ObjectTemplate> Globals(Isolate* isolate, std::vector<char*> args) {
  Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
  global->Set(isolate, "print", fnt(isolate, Print));
  global->SetAccessor(str(isolate, "ARGV"), GetArgv);
  return global;
}

namespace jackson {

Environment::Environment(int argc, char* argv[])
  : module_registry(this),
    module_loader(this),
    args(argv + 1, argv + argc) {
  argv = uv_setup_args(argc, argv);

  V8::InitializeICUDefaultLocation(args[0]);
  V8::InitializeExternalStartupData(args[0]);
  platform_ = v8::platform::NewDefaultPlatform();

  // Turn on top-level await by default
  if (std::find(args.begin(), args.end(),
      "--harmony-top-level-await") == args.end()) {
    args.push_back(const_cast<char*>("--harmony-top-level-await"));
    argc++;
  }
  V8::SetFlagsFromCommandLine(&argc, &args[0], true);
  args.resize(argc);
  entrypoint = args[0];

  V8::InitializePlatform(platform_.get());
  V8::Initialize();

  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
    ArrayBuffer::Allocator::NewDefaultAllocator();

  isolate_ = Isolate::New(create_params);
  isolate_->SetData(0, this);

  isolate_->SetHostImportModuleDynamicallyCallback(DynamicImportCallback);
  isolate_->SetHostInitializeImportMetaObjectCallback(MetaCallback);

  delete create_params.array_buffer_allocator;

  // Setup context for this environment
  // TODO: Allow creating multiple contexts?
  {
    Isolate::Scope isolate_scope(isolate_);
    HandleScope handle_scope(isolate_);
    auto context = Context::New(isolate_, nullptr, Globals(isolate_, args));
    context_.Reset(isolate_, context);
  }

  uv_loop_init(&loop_);
}

Environment::~Environment() {
  uv_loop_close(&loop_);

  context_.Reset();

  isolate_->Dispose();
  isolate_ = nullptr;

  V8::Dispose();
  // V8::ShutdownPlatform();
}

ModuleRegistry* Environment::ModuleRegistry() {
  return &module_registry;
}

ModuleLoader* Environment::ModuleLoader() {
  return &module_loader;
}

Isolate* Environment::Isolate() {
  return isolate_;
}

Local<Context> Environment::Context() {
  return context_.Get(isolate_);
}

uv_loop_t* Environment::Loop() {
  return &loop_;
}

std::vector<char*> Environment::Args() {
  return args;
}

void Environment::SyntheticModule(Local<String> name, Local<Module> module) {
  RunModule(Context(), module);
  ModuleRegistry()->Add(name, module);
}

void Environment::Tick() {
  uv_run(&loop_, UV_RUN_ONCE);
  ProcessMessages(isolate_, platform_.get());
}

int Environment::Run() {
  Isolate::Scope scope(isolate_);

  if (!RunMain(isolate_, entrypoint)) {
    return 0;
  };

  while (uv_loop_alive(&loop_)) {
    Tick();
  }

  return 1;
}

}
