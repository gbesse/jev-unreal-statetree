// Purpose: Own asynchronous gateway requests and make them pollable by StateTree instance data without unsafe callbacks.
#pragma once
#include "Subsystems/WorldSubsystem.h"
#include "JevDecisionSubsystem.generated.h"

class UJevDecisionAsset;

USTRUCT(BlueprintType)
struct FJevDecisionResult
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) bool bComplete = false;
    UPROPERTY(BlueprintReadOnly) bool bSucceeded = false;
    UPROPERTY(BlueprintReadOnly) FString Outcome;
    UPROPERTY(BlueprintReadOnly) FString Error;
    UPROPERTY(BlueprintReadOnly) FString Model;
};

UCLASS()
class JEVSTATETREE_API UJevDecisionSubsystem final : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Jev") void SetSessionToken(const FString& Token);
    FGuid Request(const UJevDecisionAsset* Asset, const FString& StateJson, const FString& Revision);
    bool Poll(const FGuid& RequestId, const FString& CurrentRevision, FJevDecisionResult& OutResult);
    void Cancel(const FGuid& RequestId);

private:
    struct FPending { FString Revision; FString Pack; TArray<FString> Allowed; FJevDecisionResult Result; TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> Http; };
    TMap<FGuid, FPending> Pending;
    FString SessionToken;
};
