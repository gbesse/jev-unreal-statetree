// Purpose: Provide an engine-independent, unit-testable guard for stale, misrouted, or unapproved outcomes.
#pragma once
#include <algorithm>
#include <string>
#include <vector>

namespace JevStateTree
{
struct GuardInput
{
    int SchemaVersion = 0;
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

inline std::string Validate(const GuardInput& Input)
{
    if (Input.CurrentRevision != Input.CapturedRevision) return "world revision changed during evaluation";
    if (Input.RequestId != Input.ExpectedRequestId || Input.Revision != Input.CapturedRevision) return "decision request mismatch";
    if (Input.SchemaVersion != 1 || Input.Pack != Input.ExpectedPack || Input.Model.empty()) return "invalid decision provenance";
    if (Input.Outcome.empty() || std::find(Input.AllowedOutcomes.begin(), Input.AllowedOutcomes.end(), Input.Outcome) == Input.AllowedOutcomes.end()) return "decision outcome is not allowed";
    return {};
}
}
