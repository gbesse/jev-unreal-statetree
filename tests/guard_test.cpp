// Purpose: Exercise the engine-independent provenance and finite-outcome guard with a native compiler.
#include "JevGuardCore.h"
#include <cassert>
#include <iostream>

int main()
{
    JevStateTree::GuardInput Input{1,"req","req","7","7","7","npc-combat","npc-combat","jev-1.13.0","retreat",{"attack","retreat","flank"}};
    assert(JevStateTree::Validate(Input).empty());
    Input.CurrentRevision = "8"; assert(JevStateTree::Validate(Input) == "world revision changed during evaluation");
    Input.CurrentRevision = "7"; Input.Outcome = "delete-save"; assert(JevStateTree::Validate(Input) == "decision outcome is not allowed");
    Input.Outcome = "attack"; Input.RequestId = "wrong"; assert(JevStateTree::Validate(Input) == "decision request mismatch");
    std::cout << "Jev guard tests passed\n";
}
