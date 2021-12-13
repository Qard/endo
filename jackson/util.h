#pragma once

#include <iostream>
#include <cstdarg>
#include <string>

#include <v8.h>

using v8::Context;
using v8::Exception;
using v8::Function;
using v8::FunctionCallback;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Promise;
using v8::PropertyCallbackInfo;
using v8::String;
using v8::TryCatch;
using v8::Value;

namespace jackson {
class Environment;
}

static inline jackson::Environment* Env(Isolate* isolate) {
  return static_cast<jackson::Environment*>(isolate->GetData(0));
}
static inline jackson::Environment* Env(Local<Context> context) {
  return Env(context->GetIsolate());
}

static inline jackson::Environment* Env(const FunctionCallbackInfo<Value>& info) {
  return Env(info.GetIsolate());
}

static inline jackson::Environment* Env(const PropertyCallbackInfo<Value>& info) {
  return Env(info.GetIsolate());
}

static inline std::string to_string(std::string s) {
  return s;
}
static inline std::string to_string(Local<Value> v) {
  return *String::Utf8Value(Isolate::GetCurrent(), v);
}
template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
static inline std::string to_string(T t) {
  return std::to_string(t);
}

static inline void Debug() {
  std::cout << std::endl;
}
template<typename T, typename... Types>
static inline void Debug(T head, Types... tail) {
  std::cout << to_string(head) << " ";
  Debug(tail...);
}

template <typename T>
inline void USE(T&&) {}

#define intern(isolate, v) String::NewFromUtf8Literal(isolate, v, \
  NewStringType::kInternalized)

static inline Local<String> str(Isolate* isolate, const char* s) {
  return String::NewFromUtf8(isolate, s, NewStringType::kNormal)
    .ToLocalChecked();
}

static inline Local<String> str(Isolate* isolate, std::string s) {
  return str(isolate, s.c_str());
}

static inline Local<Function> fn(Local<Context> context, FunctionCallback v) {
  return Function::New(context, v).ToLocalChecked();
}

static inline Local<FunctionTemplate> fnt(Isolate* isolate, FunctionCallback fn) {
  return FunctionTemplate::New(isolate, fn);
}

static inline Local<Promise::Resolver> Resolver(Local<Context> context) {
  return Promise::Resolver::New(context).ToLocalChecked();
}

static inline Local<Promise> Resolve(Local<Context> context, Local<Value> value = Local<Value>()) {
  if (value.IsEmpty()) value = v8::Undefined(Isolate::GetCurrent());
  Local<Promise::Resolver> resolver = Resolver(context);
  USE(resolver->Resolve(context, value));
  return resolver->GetPromise();
}

static inline Local<Promise> Reject(Local<Context> context, Local<Value> error) {
  Local<Promise::Resolver> resolver = Resolver(context);
  USE(resolver->Reject(context, error));
  return resolver->GetPromise();
}

static inline Local<Promise> Reject(Local<Context> context, Local<String> message) {
  return Reject(context, Exception::Error(message));
}

static inline Local<Promise> Reject(Local<Context> context, const char* message) {
  return Reject(context, str(context->GetIsolate(), message));
}

static inline void ReportException(Isolate* isolate, TryCatch* try_catch) {
  v8::HandleScope handle_scope(isolate);
  v8::String::Utf8Value exception(isolate, try_catch->Exception());
  v8::Local<v8::Message> message = try_catch->Message();

  if (message.IsEmpty()) {
    // V8 didn't provide any extra information about this error; just
    // print the exception.
    fprintf(stderr, "%s\n", *exception);
  } else {
    // Print (filename):(line number): (message).
    v8::String::Utf8Value filename(isolate, message->GetScriptOrigin().ResourceName());
    v8::Local<v8::Context> context(isolate->GetCurrentContext());

    int linenum = message->GetLineNumber(context).FromJust();
    fprintf(stderr, "exception: %s:%i: %s\n", *filename, linenum, *exception);

    // Print line of source code.
    v8::String::Utf8Value sourceline(isolate, message->GetSourceLine(context).ToLocalChecked());
    if (sourceline.length()) {
      fprintf(stderr, "%s\n", *sourceline);
    }

    // Print wavy underline (GetUnderline is deprecated).
    int start = message->GetStartColumn(context).FromJust();
    for (int i = 0; i < start; i++) {
      fprintf(stderr, " ");
    }
    int end = message->GetEndColumn(context).FromJust();
    for (int i = start; i < end; i++) {
      fprintf(stderr, "^");
    }
    fprintf(stderr, "\n");

    v8::Local<v8::Value> stack_trace_string;
    if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
        stack_trace_string->IsString() &&
        v8::Local<v8::String>::Cast(stack_trace_string)->Length() > 0) {
      v8::String::Utf8Value stack_trace(isolate, stack_trace_string);
      fprintf(stderr, "%s\n", *stack_trace);
    }
  }
}
