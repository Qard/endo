#include "module_wrap.h"

namespace jackson {

ModuleWrap::ModuleWrap(Isolate* isolate, Local<Module> module, Local<String> url)
  : module_(isolate, module),
    url_(isolate, url) {};

bool ModuleWrap::operator==(Local<Module>& mod) {
  return module_.Get(Isolate::GetCurrent())->GetIdentityHash() == mod->GetIdentityHash();
}

Local<Module> ModuleWrap::module() {
  return module_.Get(Isolate::GetCurrent());
}

Local<String> ModuleWrap::url() {
  return url_.Get(Isolate::GetCurrent());
}

}
