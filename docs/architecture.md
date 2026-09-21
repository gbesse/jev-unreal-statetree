# Architecture

The StateTree task starts one gateway request on `EnterState`, polls only that request from `Tick`, and cancels it on `ExitState`. Relevant world changes must update the bound revision. A response is accepted only when request ID, captured revision, pack, schema, model, and finite outcome all match.

The plugin never holds a TypeSafe credential. `SessionToken` is runtime-only and authenticates to an application-owned gateway. Direct loopback HTTP exists for development; non-loopback endpoints require HTTPS.
