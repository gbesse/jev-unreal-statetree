// Purpose: Send bounded decision envelopes to a trusted gateway and validate provenance before exposing an outcome.
#include "JevDecisionSubsystem.h"
#include "JevDecisionAsset.h"
#include "JevGuardCore.h"
#include "JevStateTreeSettings.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void UJevDecisionSubsystem::SetSessionToken(const FString& Token) { SessionToken = Token; }

FGuid UJevDecisionSubsystem::Request(const UJevDecisionAsset* Asset, const FString& StateJson, const FString& Revision)
{
    const FGuid Id = FGuid::NewGuid();
    FPending& Entry = Pending.Add(Id);
    Entry.Revision = Revision;
    const auto Fail = [&Entry](const FString& Error)
    {
        Entry.Result.bComplete = true;
        Entry.Result.bSucceeded = false;
        Entry.Result.Outcome.Empty();
        Entry.Result.Error = Error;
        Entry.Result.Model.Empty();
    };
    if (!Asset || Asset->PackId.IsNone() || Asset->AllowedOutcomes.Num() < 2) { Fail(TEXT("Invalid decision asset.")); return Id; }
    Entry.Pack = Asset->PackId.ToString(); Entry.Allowed = Asset->AllowedOutcomes;
    TSharedPtr<FJsonObject> ParsedState;
    if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(StateJson), ParsedState) || !ParsedState.IsValid()) { Fail(TEXT("StateJson must be a JSON object.")); return Id; }
    const UJevStateTreeSettings* Settings = GetDefault<UJevStateTreeSettings>();
    const bool bSecure = Settings->GatewayUrl.StartsWith(TEXT("https://"), ESearchCase::IgnoreCase);
    const bool bLoopback = Settings->GatewayUrl.StartsWith(TEXT("http://127.0.0.1:"), ESearchCase::IgnoreCase) || Settings->GatewayUrl.StartsWith(TEXT("http://localhost:"), ESearchCase::IgnoreCase);
    if (!bSecure && !bLoopback) { Fail(TEXT("Gateway requires HTTPS except on loopback.")); return Id; }
    if (SessionToken.IsEmpty()) { Fail(TEXT("Set a runtime gateway session token.")); return Id; }
    TSharedRef<FJsonObject> Payload = MakeShared<FJsonObject>(); Payload->SetStringField(TEXT("requestId"), Id.ToString(EGuidFormats::Digits)); Payload->SetStringField(TEXT("revision"), Revision); Payload->SetStringField(TEXT("packId"), Entry.Pack); Payload->SetObjectField(TEXT("state"), ParsedState);
    FString Body; FJsonSerializer::Serialize(Payload, TJsonWriterFactory<>::Create(&Body));
    if (Body.Len() > 100000) { Fail(TEXT("Decision request is too large.")); return Id; }
    Entry.Http = FHttpModule::Get().CreateRequest(); Entry.Http->SetURL(Settings->GatewayUrl); Entry.Http->SetVerb(TEXT("POST")); Entry.Http->SetHeader(TEXT("Content-Type"), TEXT("application/json")); Entry.Http->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + SessionToken); Entry.Http->SetTimeout(Settings->TimeoutSeconds); Entry.Http->SetContentAsString(Body);
    Entry.Http->OnProcessRequestComplete().BindWeakLambda(this, [this, Id](FHttpRequestPtr, FHttpResponsePtr Response, bool bConnected) {
        FPending* Item = Pending.Find(Id); if (!Item) return;
        const auto FailItem = [Item](const FString& Error) { Item->Result.bComplete = true; Item->Result.bSucceeded = false; Item->Result.Outcome.Empty(); Item->Result.Error = Error; Item->Result.Model.Empty(); };
        if (!bConnected || !Response.IsValid() || Response->GetResponseCode() < 200 || Response->GetResponseCode() >= 300) { FailItem(TEXT("Gateway request failed.")); return; }
        TSharedPtr<FJsonObject> Root; if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Response->GetContentAsString()), Root) || !Root.IsValid()) { FailItem(TEXT("Gateway returned invalid JSON.")); return; }
        const TSharedPtr<FJsonObject>* Record = nullptr; if (!Root->TryGetObjectField(TEXT("record"), Record) || !Record || !Record->IsValid()) { FailItem(TEXT("Gateway response has no record.")); return; }
        const TSharedPtr<FJsonObject>* Pack = nullptr; (*Record)->TryGetObjectField(TEXT("pack"), Pack);
        FString RequestId, ResponseRevision, PackName, Model, Outcome; double SchemaVersion = 0.0;
        const bool bFieldsValid = Root->TryGetStringField(TEXT("requestId"), RequestId) && Root->TryGetStringField(TEXT("revision"), ResponseRevision) && (*Record)->TryGetNumberField(TEXT("schemaVersion"), SchemaVersion) && Pack && Pack->IsValid() && (*Pack)->TryGetStringField(TEXT("name"), PackName) && (*Record)->TryGetStringField(TEXT("model"), Model) && (*Record)->TryGetStringField(TEXT("outcome"), Outcome);
        if (!bFieldsValid) { FailItem(TEXT("Gateway response fields are invalid.")); return; }
        JevStateTree::GuardInput Guard; Guard.SchemaVersion = static_cast<int>(SchemaVersion); Guard.RequestId = TCHAR_TO_UTF8(*RequestId); Guard.ExpectedRequestId = TCHAR_TO_UTF8(*Id.ToString(EGuidFormats::Digits)); Guard.Revision = TCHAR_TO_UTF8(*ResponseRevision); Guard.CapturedRevision = TCHAR_TO_UTF8(*Item->Revision); Guard.CurrentRevision = Guard.CapturedRevision; Guard.Pack = TCHAR_TO_UTF8(*PackName); Guard.ExpectedPack = TCHAR_TO_UTF8(*Item->Pack); Guard.Model = TCHAR_TO_UTF8(*Model); Guard.Outcome = TCHAR_TO_UTF8(*Outcome); for (const FString& Value : Item->Allowed) Guard.AllowedOutcomes.emplace_back(TCHAR_TO_UTF8(*Value));
        const std::string Error = JevStateTree::Validate(Guard); Item->Result.bComplete = true; Item->Result.bSucceeded = Error.empty(); Item->Result.Error = UTF8_TO_TCHAR(Error.c_str()); Item->Result.Outcome = Item->Result.bSucceeded ? UTF8_TO_TCHAR(Guard.Outcome.c_str()) : TEXT(""); Item->Result.Model = UTF8_TO_TCHAR(Guard.Model.c_str());
    });
    if (!Entry.Http->ProcessRequest()) Fail(TEXT("Could not start gateway request."));
    return Id;
}

bool UJevDecisionSubsystem::Poll(const FGuid& RequestId, const FString& CurrentRevision, FJevDecisionResult& OutResult)
{
    FPending* Entry = Pending.Find(RequestId); if (!Entry) return false;
    if (Entry->Revision != CurrentRevision) { if (Entry->Http.IsValid()) Entry->Http->CancelRequest(); Entry->Result.bComplete = true; Entry->Result.bSucceeded = false; Entry->Result.Outcome.Empty(); Entry->Result.Error = TEXT("World revision changed during evaluation."); Entry->Result.Model.Empty(); }
    OutResult = Entry->Result; if (OutResult.bComplete) Pending.Remove(RequestId); return true;
}

void UJevDecisionSubsystem::Cancel(const FGuid& RequestId) { if (FPending* Entry = Pending.Find(RequestId)) if (Entry->Http.IsValid()) Entry->Http->CancelRequest(); Pending.Remove(RequestId); }
