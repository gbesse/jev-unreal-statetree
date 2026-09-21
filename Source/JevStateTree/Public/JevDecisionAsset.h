// Purpose: Store an auditable DecisionPack reference and its finite outcomes as an Unreal Data Asset.
#pragma once
#include "Engine/DataAsset.h"
#include "JevDecisionAsset.generated.h"

UCLASS(BlueprintType)
class JEVSTATETREE_API UJevDecisionAsset final : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Jev")
    FName PackId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Jev")
    TArray<FString> AllowedOutcomes;

    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
};
