#include "endo/env.h"
#include "modules/timer/timer.h"

using endo::Environment;

int main(int argc, char* argv[]) {
  Environment env(argc, argv);
  Environment::Scope scope(&env);
  endo::module::timer::Init(&env);
  return env.Run();
}
