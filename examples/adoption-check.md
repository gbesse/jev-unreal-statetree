# jev-unreal-statetree — contrôle d’adoption · adoption check · comprobación de adopción

## Français

Point de départ local, après la préparation indiquée dans le README :

```sh
npm run demo:check
```

Le seuil fictif de santé 29/30 change la route, tandis qu’une ancienne révision doit être rejetée. Ce contrôle HTTP ne valide pas le nœud StateTree dans Unreal Editor 5.8.

## English

Local starting point, after the setup described in the README:

```sh
npm run demo:check
```

The fictional health threshold 29/30 changes the route, while a stale revision must be rejected. This HTTP check does not validate the StateTree node in Unreal Editor 5.8.

## Español

Punto de partida local, después de la preparación descrita en el README:

```sh
npm run demo:check
```

El umbral ficticio de salud 29/30 cambia la ruta y una revisión obsoleta debe rechazarse. Esta prueba HTTP no valida el nodo StateTree en Unreal Editor 5.8.
## Variante synthétique · Synthetic variation · Variante sintética

```text
health=29 -> health=30; response_revision=1; world_revision=2
```

FR : adaptez une copie de la fixture locale à cette situation, puis vérifiez le comportement décrit ci-dessus. Les valeurs sont illustratives, pas des résultats Jev mesurés.

EN: adapt a copy of the local fixture to this situation, then check the behavior described above. Values are illustrative, not measured Jev output.

ES: adapte una copia de la fixture local a esta situación y compruebe el comportamiento descrito arriba. Los valores son ilustrativos, no resultados Jev medidos.

## Second cas · Second case · Segundo caso

```text
response_revision=1; world_revision=2; outcome=attack
```

**FR :** Une réponse valide sur le fond mais liée à une ancienne révision du monde doit être rejetée. Confirmez ce garde aussi dans Unreal Editor 5.8 avant une release.

**EN:** A substantively valid response tied to an old world revision must be rejected. Confirm that guard in Unreal Editor 5.8 before a release.

**ES:** Una respuesta válida en contenido pero vinculada a una revisión antigua del mundo debe rechazarse. Confirme esta protección en Unreal Editor 5.8 antes de una release.
