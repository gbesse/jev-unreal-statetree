// Purpose: Exercise the engine-independent provenance and finite-outcome guard with a native compiler.
#include "JevGuardCore.h"
#include <cassert>
#include <iostream>
#include <limits>

int main()
{
    JevStateTree::GuardInput Input{1,"req","req","7","7","7","npc-combat","npc-combat","jev-1.13.0","retreat",{"attack","retreat","flank"}};
    assert(JevStateTree::Validate(Input).empty());
    Input.CurrentRevision = "8"; assert(JevStateTree::Validate(Input) == "world revision changed during evaluation");
    Input.CurrentRevision = "7"; Input.Outcome = "delete-save"; assert(JevStateTree::Validate(Input) == "decision outcome is not allowed");
    Input.Outcome = "attack"; Input.RequestId = "wrong"; assert(JevStateTree::Validate(Input) == "decision request mismatch");
    Input.RequestId = "req";
    const auto Valid = Input;
    Input.RequestId.clear(); Input.ExpectedRequestId.clear();
    Input.Revision.clear(); Input.CapturedRevision.clear(); Input.CurrentRevision.clear();
    Input.Pack.clear(); Input.ExpectedPack.clear();
    assert(!JevStateTree::Validate(Input).empty());
    for (std::string JevStateTree::GuardInput::* Field : {&JevStateTree::GuardInput::RequestId,
            &JevStateTree::GuardInput::ExpectedRequestId, &JevStateTree::GuardInput::Revision,
            &JevStateTree::GuardInput::CapturedRevision, &JevStateTree::GuardInput::CurrentRevision,
            &JevStateTree::GuardInput::Pack, &JevStateTree::GuardInput::ExpectedPack})
    {
        Input = Valid; (Input.*Field).clear(); assert(!JevStateTree::Validate(Input).empty());
    }
    for (double Version : {0.0, 1.1, 1.9, -1.0, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::quiet_NaN()})
    {
        Input = Valid; Input.SchemaVersion = Version;
        assert(!JevStateTree::Validate(Input).empty());
    }
    Input = Valid; Input.SchemaVersion = 1.0; assert(JevStateTree::Validate(Input).empty());
    for (std::string JevStateTree::GuardInput::* Field : {&JevStateTree::GuardInput::RequestId,
            &JevStateTree::GuardInput::ExpectedRequestId, &JevStateTree::GuardInput::Revision,
            &JevStateTree::GuardInput::CapturedRevision, &JevStateTree::GuardInput::CurrentRevision,
            &JevStateTree::GuardInput::Pack, &JevStateTree::GuardInput::ExpectedPack,
            &JevStateTree::GuardInput::Model, &JevStateTree::GuardInput::Outcome})
    { Input = Valid; (Input.*Field) = " \t\r\n"; assert(!JevStateTree::Validate(Input).empty()); }
    Input = Valid; Input.Model = " "; assert(!JevStateTree::Validate(Input).empty());
    for (const auto& Allowed : std::vector<std::vector<std::string>>{{},{"attack","attack"},{"attack"," "},{"attack",""}})
    { Input = Valid; Input.AllowedOutcomes = Allowed; assert(!JevStateTree::Validate(Input).empty()); }
    Input = Valid; Input.AllowedOutcomes = {"attack","retreat"}; assert(JevStateTree::Validate(Input).empty());
    std::cout << "Jev guard tests passed\n";
}
