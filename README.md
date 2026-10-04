# Jev StateTree for Unreal Engine

**Run finite, revision-guarded Jev decisions as native Unreal Engine 5.8 StateTree tasks.**

[![Tests](https://github.com/gbesse/jev-unreal-statetree/actions/workflows/test.yml/badge.svg)](https://github.com/gbesse/jev-unreal-statetree/actions/workflows/test.yml) [MIT](LICENSE) · Unreal Engine 5.8 · C++ runtime plugin · Public alpha

This is deliberately narrower than a generic Blueprint API. A `Jev Decision` task starts exactly one asynchronous request when its state is entered, exposes `Outcome` and `Error` bindings, rejects stale world revisions, and cancels when the state exits. It never calls Jev every frame.

## Install

Copy the repository into `<Project>/Plugins/JevStateTree`, enable **StateTree**, **Gameplay StateTree**, and **Jev StateTree**, then rebuild the project. Create a `JevDecisionAsset`, declare the registered gateway pack and allowed outcomes, and add **AI / Jev / Jev Decision** to a StateTree.

At runtime, set a short-lived gateway session token:

```cpp
GetWorld()->GetSubsystem<UJevDecisionSubsystem>()->SetSessionToken(SessionToken);
```

Bind a small JSON object and a world revision to the task. Increment the revision whenever relevant state changes.

## Offline gateway

```sh
JEV_GATEWAY_TOKEN=local-demo-token npm run demo
```

The fixture is synthetic and never contacts Jev. For production, host your own authenticated gateway and register DecisionPacks server-side. Do not ship a TypeSafe key or a reusable gateway secret inside a packaged game.

## Preview finite outcomes without Unreal

Run `npm run demo:check` for a self-contained check that starts the fixture on an ephemeral loopback port, runs the client, and shuts the fixture down. For interactive use, run `JEV_GATEWAY_TOKEN=local-demo-token npm run demo` in one terminal and `npm run demo:client` in another. The client sends two synthetic NPC states and checks that low health yields `retreat` while higher health yields `attack`, with matching request IDs and revisions. This previews the gateway envelope only; it does not compile or exercise the StateTree task in Editor.

## How it decides

Unreal owns available actions. The gateway returns one finite outcome plus provenance. The subsystem verifies schema version, request ID, revision, pack name, model, and membership in the asset's allowed outcomes before StateTree receives anything. See [architecture](docs/architecture.md).

## Validation and boundaries

`npm test` compiles the engine-independent C++ guard with warnings-as-errors and validates plugin metadata. Unreal Engine is not installed in this development environment, so the `.uplugin`, module and StateTree task were not compiled or loaded in Editor here. Before tagging a release, open a clean UE 5.8 C++ project, rebuild, validate the data asset, and exercise enter/cancel/revision-change paths in PIE.

This is not a per-frame inference system, navigation system, planner, or authority layer. Network results can be delayed or wrong; gameplay code must remain deterministic when a request fails. Thresholds and packs require game-specific testing.

## Related projects

[Unity Jev Behavior](https://github.com/gbesse/unity-jev-behavior) · [Reflex Godot](https://github.com/gbesse/reflex-godot) · [DecisionPacks](https://github.com/gbesse/decisionpacks) · [WorldKit](https://github.com/gbesse/worldkit)

Independent project; not affiliated with TypeSafe AI or Epic Games. [TypeSafe API](https://docs.typesafe.ai/api) · [Unreal StateTree](https://dev.epicgames.com/documentation/en-us/unreal-engine/overview-of-state-tree-in-unreal-engine)
