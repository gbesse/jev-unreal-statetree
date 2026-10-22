// Purpose: Provide an engine-independent, unit-testable guard for stale, misrouted, or unapproved outcomes.
#pragma once
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

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
    if (Input.CurrentRevision != Input.CapturedRevision) return "world revision changed during evaluation";
    if (Input.RequestId != Input.ExpectedRequestId || Input.Revision != Input.CapturedRevision) return "decision request mismatch";
    if (Input.SchemaVersion != 1 || Input.Pack != Input.ExpectedPack || !HasText(Input.Model)) return "Provenance de decision invalide / Invalid decision provenance / Procedencia de decision no valida";
    if (!HasText(Input.Outcome) || std::find(Input.AllowedOutcomes.begin(), Input.AllowedOutcomes.end(), Input.Outcome) == Input.AllowedOutcomes.end()) return "decision outcome is not allowed";
    return {};
}
}
