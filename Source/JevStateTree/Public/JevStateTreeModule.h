// Purpose: Declare the runtime module loaded by Unreal for the Jev StateTree plugin.
#pragma once
#include "Modules/ModuleManager.h"

class FJevStateTreeModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
