// Preview two finite fixture outcomes without invoking the Unreal Engine.
const url = process.env.JEV_GATEWAY_URL || 'http://127.0.0.1:8787/v1/decision';
const token = process.env.JEV_GATEWAY_TOKEN || 'local-demo-token';
const scenarios = [
  {name: 'low_health', health: 20, expected: 'retreat'},
  {name: 'high_health', health: 80, expected: 'attack'},
];
const results = [];
for (const scenario of scenarios) {
  const requestId = `fixture-${scenario.name}`;
  const revision = `world-${scenario.name}`;
  const response = await fetch(url, {
    method: 'POST',
    headers: {'authorization': `Bearer ${token}`, 'content-type': 'application/json'},
    body: JSON.stringify({requestId, revision, packId: 'npc-combat', state: {health: scenario.health}}),
  });
  const body = await response.json();
  if (!response.ok || body.requestId !== requestId || body.revision !== revision || body.record?.outcome !== scenario.expected) {
    throw new Error(`Unexpected ${scenario.name} fixture response: ${response.status}`);
  }
  results.push({scenario: scenario.name, outcome: body.record.outcome, request_identity_ok: true});
}
console.log(JSON.stringify({source: 'synthetic loopback fixture; no Jev or Unreal', results}, null, 2));
