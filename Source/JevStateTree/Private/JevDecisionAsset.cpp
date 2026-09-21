// Purpose: Reject unusable or ambiguous finite decision assets before runtime.
#include "JevDecisionAsset.h"
#include "Misc/DataValidation.h"

EDataValidationResult UJevDecisionAsset::IsDataValid(FDataValidationContext& Context) const
{
    if (PackId.IsNone()) Context.AddError(FText::FromString(TEXT("PackId is required.")));
    if (AllowedOutcomes.Num() < 2) Context.AddError(FText::FromString(TEXT("Declare at least two allowed outcomes.")));
    TSet<FString> Unique;
    for (const FString& Outcome : AllowedOutcomes)
    {
        if (Outcome.TrimStartAndEnd().IsEmpty()) Context.AddError(FText::FromString(TEXT("Outcomes cannot be empty.")));
        if (Unique.Contains(Outcome)) Context.AddError(FText::FromString(TEXT("Outcomes must be unique.")));
        Unique.Add(Outcome);
    }
    return Context.GetNumErrors() == 0 ? EDataValidationResult::Valid : EDataValidationResult::Invalid;
}
