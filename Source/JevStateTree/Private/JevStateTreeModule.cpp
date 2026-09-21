// Purpose: Register the small runtime module without editor-only or global mutable state.
#include "JevStateTreeModule.h"
#include "Modules/ModuleManager.h"

void FJevStateTreeModule::StartupModule() {}
void FJevStateTreeModule::ShutdownModule() {}
IMPLEMENT_MODULE(FJevStateTreeModule, JevStateTree)
