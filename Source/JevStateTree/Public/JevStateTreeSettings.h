// Purpose: Expose only non-secret gateway settings in Unreal Project Settings.
#pragma once
#include "Engine/DeveloperSettings.h"
#include "JevStateTreeSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Jev StateTree"))
class JEVSTATETREE_API UJevStateTreeSettings final : public UDeveloperSettings
{
    GENERATED_BODY()
public:
    UPROPERTY(Config, EditAnywhere, Category="Gateway")
    FString GatewayUrl = TEXT("http://127.0.0.1:8787/v1/decision");

    UPROPERTY(Config, EditAnywhere, Category="Gateway", meta=(ClampMin="1", ClampMax="60"))
    int32 TimeoutSeconds = 35;
};
