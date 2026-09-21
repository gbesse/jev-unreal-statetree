// Purpose: Start exactly one request on state entry, poll it, and cancel it when StateTree exits.
#include "StateTreeJevDecisionTask.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FStateTreeJevDecisionTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult&) const
{
    FInstanceDataType& Data = Context.GetInstanceData(*this); Data.Outcome.Empty(); Data.Error.Empty();
    UWorld* World = Context.GetWorld(); UJevDecisionSubsystem* Subsystem = World ? World->GetSubsystem<UJevDecisionSubsystem>() : nullptr;
    if (!Subsystem) { Data.Error = TEXT("JevDecisionSubsystem is unavailable."); return EStateTreeRunStatus::Failed; }
    Data.RequestId = Subsystem->Request(Data.Decision, Data.StateJson, Data.Revision); return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStateTreeJevDecisionTask::Tick(FStateTreeExecutionContext& Context, float) const
{
    FInstanceDataType& Data = Context.GetInstanceData(*this); UWorld* World = Context.GetWorld(); UJevDecisionSubsystem* Subsystem = World ? World->GetSubsystem<UJevDecisionSubsystem>() : nullptr; FJevDecisionResult Result;
    if (!Subsystem || !Subsystem->Poll(Data.RequestId, Data.Revision, Result)) { Data.Error = TEXT("Decision request disappeared."); return EStateTreeRunStatus::Failed; }
    if (!Result.bComplete) return EStateTreeRunStatus::Running; Data.Outcome = Result.Outcome; Data.Error = Result.Error; return Result.bSucceeded ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
}

void FStateTreeJevDecisionTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult&) const
{
    FInstanceDataType& Data = Context.GetInstanceData(*this); if (UWorld* World = Context.GetWorld()) if (UJevDecisionSubsystem* Subsystem = World->GetSubsystem<UJevDecisionSubsystem>()) Subsystem->Cancel(Data.RequestId);
}
