#include "module_registry.h"

namespace jackson {

void ModuleRegistry::Add(Local<String> url, Local<Module> mod) {
  auto wrap = new ModuleWrap(env_->Isolate(), mod, url);
  cache.emplace(mod->GetIdentityHash(), wrap);
}

MaybeLocal<Module> ModuleRegistry::GetModule(Local<String> url) {
  auto findByUrl = [url](std::pair<int, ModuleWrap*> pair) {
    return pair.second->url() == url;
  };

  auto found = std::find_if(begin(cache), end(cache), findByUrl);
  if (found == end(cache)) {
    return MaybeLocal<Module>();
  }

  return MaybeLocal<Module>(found->second->module());
}

MaybeLocal<String> ModuleRegistry::GetUrl(Local<Module> mod) {
  auto found = cache.find(mod->GetIdentityHash());
  if (found == end(cache)) {
    return MaybeLocal<String>();
  }

  return MaybeLocal<String>(found->second->url());
}

} // jackson
