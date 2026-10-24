# Jev StateTree for Unreal Engine

## Fixture boundary / Limite de la fixture / Límite de la fixture

Run `npm run demo:check` to exercise health values 29 and 30 on the local synthetic gateway. The fixture changes from `retreat` to `attack` at 30 and checks request identity and revision for every response. This is an HTTP contract example; validate the StateTree task separately in Unreal Editor before shipping it.

Exécutez `npm run demo:check` pour essayer les valeurs de santé 29 et 30 sur la passerelle synthétique locale. La fixture passe de `retreat` à `attack` à 30 et vérifie l'identité de la requête et la révision de chaque réponse. Cet exemple vérifie le contrat HTTP ; validez la tâche StateTree séparément dans Unreal Editor avant sa publication.

Ejecute `npm run demo:check` para probar los valores de salud 29 y 30 en la pasarela sintética local. La fixture cambia de `retreat` a `attack` en 30 y comprueba la identidad de la solicitud y la revisión de cada respuesta. Este ejemplo verifica el contrato HTTP; valide la tarea StateTree por separado en Unreal Editor antes de publicarla.

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

## October 2026 improvement · Amélioration d’octobre 2026 · Mejora de octubre de 2026

`npm run demo:check` now also verifies that invalid health is rejected with HTTP 400 by the synthetic gateway. This does not validate the plugin in Unreal Editor.

`npm run demo:check` vérifie aussi que la passerelle synthétique refuse une santé invalide avec HTTP 400. Cela ne valide pas le plugin dans Unreal Editor.

`npm run demo:check` también comprueba que la pasarela sintética rechaza una salud inválida con HTTP 400. Esto no valida el complemento en Unreal Editor.

## Contrôle d’adoption · Adoption check · Comprobación de adopción

[Français : essayer un cas concret](examples/adoption-check.md) · [English: try a concrete case](examples/adoption-check.md) · [Español: pruebe un caso concreto](examples/adoption-check.md).


FR : Le garde C++ portable refuse les identifiants de requête, pack ou révision vides, même lorsque deux champs vides sont égaux. `npm test` compile cette logique avec avertissements traités en erreurs et couvre chaque champ. L’identité reste déclarée, pas authentifiée. Ce changement est publié en source via PR ; aucun nouveau tag de plugin ni binaire Unreal avant la validation UE 5.8 Editor exigée ci-dessus, absente ici.

EN: The portable C++ guard rejects empty request, pack or revision identities even when matching fields are both empty. `npm test` compiles this logic with warnings as errors and covers each field. Identity remains declared rather than authenticated. This change is published as source through a PR; no new plugin tag or Unreal binary before the required UE 5.8 Editor validation above, unavailable here.

ES: El guard C++ portable rechaza identidades vacías de solicitud, pack o revisión aunque ambos campos coincidan vacíos. `npm test` compila la lógica con advertencias como errores y cubre cada campo. La identidad sigue declarada, no autenticada. Se publica código mediante PR; sin nuevo tag de plugin ni binario Unreal antes de la validación UE 5.8 Editor requerida arriba, no disponible aquí.

## Version exacte du contrat / Exact contract version / Versión exacta del contrato

FR — Le chemin natif transmet maintenant la valeur numérique du schéma au garde portable sans conversion entière. Une version `1.1` ou `1.9` ne peut plus devenir `1` par troncation ; valeurs non finies et autres versions sont rejetées. Le compilateur natif teste le garde, pas le plugin dans UE 5.8. Publication source seulement, sans tag de plugin avant les contrôles Editor/PIE exigés plus haut.

EN — The native path now passes the numeric schema version to the portable guard without integer conversion. Versions `1.1` or `1.9` cannot truncate into `1`; nonfinite values and other versions are rejected. The native compiler tests the guard, not the UE 5.8 plugin. Source publication only, without a plugin tag until the required Editor/PIE checks above.

ES — La ruta nativa pasa la versión numérica al guard portable sin conversión entera. Versiones `1.1` o `1.9` no se truncan a `1`; valores no finitos y otras versiones se rechazan. El compilador prueba el guard, no el plugin UE 5.8. Solo código fuente, sin tag hasta las comprobaciones Editor/PIE exigidas arriba.

## Identités / Identities / Identidades

FR — Les identités, révisions, modèles et issues composés uniquement d’espaces ASCII sont rejetés. Le garde C++ est testé séparément ; UE 5.8 Editor/PIE reste absent et aucun nouveau tag natif n’est publié.

EN — Identities, revisions, models and outcomes containing only ASCII whitespace are rejected. The C++ guard is tested separately; UE 5.8 Editor/PIE remains absent and no native tag is published.

ES — Se rechazan identidades, revisiones, modelos y resultados formados solo por espacios ASCII. El guard C++ se prueba por separado; falta UE 5.8 Editor/PIE y no se publica tag nativo.

## Policy integrity / Intégrité de politique / Integridad de política

FR — La politique entière est vérifiée avant sélection : liste non vide, issues non vides et uniques. Un choix valide ne masque plus une politique contradictoire. Garde C++ réel testé ; UE 5.8 Editor/PIE reste absent.

EN — The whole policy is checked before selection: nonempty list, nonblank unique outcomes. A valid choice no longer masks a contradictory policy. Actual C++ guard tested; UE 5.8 Editor/PIE remains absent.

ES — Se comprueba toda la política antes de seleccionar: lista no vacía, resultados no vacíos y únicos. Una elección válida ya no oculta una política contradictoria. Guard C++ real probado; UE 5.8 Editor/PIE sigue ausente.
