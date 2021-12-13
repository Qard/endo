#include <v8.h>
#include <uv.h>

#include "jackson/util.h"
#include "timer.h"

namespace jackson {
namespace module {
namespace timer {

using v8::Context;
using v8::FunctionCallbackInfo;
using v8::Global;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::MaybeLocal;
using v8::Module;
using v8::Promise;
using v8::Value;

void Sleep(const FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  HandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  jackson::Environment* env = Env(isolate);

  Local<Promise::Resolver> resolver =
    Promise::Resolver::New(context).ToLocalChecked();

  if (info.Length() != 1 || !info[0]->IsNumber()) {
    USE(resolver->Reject(context, Exception::Error(
      intern(isolate, "Must provide number of milliseconds to sleep"))));
    info.GetReturnValue().Set(resolver->GetPromise());
    return;
  }

  int64_t ms = info[0]->IntegerValue(context).ToChecked();

  auto persistent = new Global<Promise::Resolver>(isolate, resolver);
  uv_timer_t* handle = new uv_timer_t;
  handle->data = persistent;

  uv_timer_init(env->Loop(), handle);
  uv_timer_start(handle, [](uv_timer_t* handle) -> void {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    Local<Context> context = isolate->GetCurrentContext();
    Context::Scope context_scope(context);

    auto persistent = static_cast<Global<Promise::Resolver>*>(handle->data);

    uv_timer_stop(handle);
    uv_close(reinterpret_cast<uv_handle_t*>(handle),
            [](uv_handle_t* handle) -> void {delete handle;});

    Local<Promise::Resolver> resolver = persistent->Get(isolate);
    USE(resolver->Resolve(context, v8::Undefined(isolate)));

    persistent->Reset();
    delete persistent;
  }, ms, 0);

  info.GetReturnValue().Set(resolver->GetPromise());
}

MaybeLocal<Value> ModuleEvaluationSteps(Local<Context> context, Local<Module> module) {
  Isolate* isolate = context->GetIsolate();

  USE(module->SetSyntheticModuleExport(
    isolate, intern(isolate, "sleep"), fn(context, Sleep)));

  return Resolve(context, v8::Undefined(isolate));
}

void Init(jackson::Environment* env) {
  Isolate* isolate = env->Isolate();
  Local<String> name = intern(isolate, "timer");
  Local<Module> timer = Module::CreateSyntheticModule(
    isolate, name, {intern(isolate, "sleep")}, ModuleEvaluationSteps);

  env->SyntheticModule(name, timer);
}

} // timer
} // module
} // jackson
