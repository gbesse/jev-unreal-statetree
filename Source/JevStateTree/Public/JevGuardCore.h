// Purpose: Provide an engine-independent, unit-testable guard for stale, misrouted, or unapproved outcomes.
#pragma once
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include <unordered_set>

namespace JevStateTree
{
struct GuardInput
{
    double SchemaVersion = 0;
    std::string RequestId;
    std::string ExpectedRequestId;
    std::string Revision;
    std::string CapturedRevision;
    std::string CurrentRevision;
    std::string Pack;
    std::string ExpectedPack;
    std::string Model;
    std::string Outcome;
    std::vector<std::string> AllowedOutcomes;
    std::string ExpectedModel;
};

inline bool HasText(const std::string& Value)
{
    return std::any_of(Value.begin(), Value.end(), [](unsigned char Character) { return !std::isspace(Character); });
}

inline std::string Validate(const GuardInput& Input)
{
    if (!HasText(Input.RequestId) || !HasText(Input.ExpectedRequestId) || !HasText(Input.Revision) ||
        !HasText(Input.CapturedRevision) || !HasText(Input.CurrentRevision) || !HasText(Input.Pack) || !HasText(Input.ExpectedPack))
        return "Identite de decision vide / Empty decision identity / Identidad de decision vacia";
    for (const auto* Value : {&Input.RequestId, &Input.ExpectedRequestId, &Input.Revision, &Input.CapturedRevision,
         &Input.CurrentRevision, &Input.Pack, &Input.ExpectedPack, &Input.Model, &Input.Outcome, &Input.ExpectedModel})
        if (Value->size() > 4096) return "Identite trop longue / Identity too long / Identidad demasiado larga";
    if (Input.AllowedOutcomes.size() > 255) return "Trop d'issues / Too many outcomes / Demasiados resultados";
    for (const auto& Value : Input.AllowedOutcomes)
        if (Value.size() > 4096) return "Issue trop longue / Outcome too long / Resultado demasiado largo";
    std::unordered_set<std::string> Seen;
    if (Input.AllowedOutcomes.empty()) return "Politique vide / Empty policy / Politica vacia";
    for (const auto& Outcome : Input.AllowedOutcomes)
        if (!HasText(Outcome) || !Seen.insert(Outcome).second)
            return "Issues vides ou dupliquees / Empty or duplicate outcomes / Resultados vacios o duplicados";
    if (Input.CurrentRevision != Input.CapturedRevision) return "world revision changed during evaluation";
    if (Input.RequestId != Input.ExpectedRequestId || Input.Revision != Input.CapturedRevision) return "decision request mismatch";
    if (Input.SchemaVersion != 1 || Input.Pack != Input.ExpectedPack || !HasText(Input.Model)) return "Provenance de decision invalide / Invalid decision provenance / Procedencia de decision no valida";
    if (!Input.ExpectedModel.empty() && Input.Model != Input.ExpectedModel)
        return "Modele different / Model mismatch / Modelo distinto";
    if (!HasText(Input.Outcome) || std::find(Input.AllowedOutcomes.begin(), Input.AllowedOutcomes.end(), Input.Outcome) == Input.AllowedOutcomes.end()) return "decision outcome is not allowed";
    return {};
}
}
