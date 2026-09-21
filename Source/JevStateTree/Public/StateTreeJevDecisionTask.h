// Purpose: Expose one asynchronous Jev gateway request as an event-driven StateTree task with finite outputs.
#pragma once
#include "StateTreeTaskBase.h"
#include "JevDecisionSubsystem.h"
#include "StateTreeJevDecisionTask.generated.h"

class UJevDecisionAsset;

USTRUCT()
struct FStateTreeJevDecisionTaskInstanceData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, Category="Input") TObjectPtr<UJevDecisionAsset> Decision;
    UPROPERTY(EditAnywhere, Category="Input") FString StateJson = TEXT("{}");
    UPROPERTY(EditAnywhere, Category="Input") FString Revision;
    UPROPERTY(EditAnywhere, Category="Output") FString Outcome;
    UPROPERTY(EditAnywhere, Category="Output") FString Error;
    FGuid RequestId;
};

USTRUCT(meta=(DisplayName="Jev Decision", Category="AI|Jev"))
struct JEVSTATETREE_API FStateTreeJevDecisionTask final : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()
    using FInstanceDataType = FStateTreeJevDecisionTaskInstanceData;
    virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, float DeltaTime) const override;
    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
