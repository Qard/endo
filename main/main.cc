#include "jackson/env.h"
#include "modules/timer/timer.h"

using jackson::Environment;

int main(int argc, char* argv[]) {
  Environment env(argc, argv);
  Environment::Scope scope(&env);
  jackson::module::timer::Init(&env);
  return env.Run();
}
