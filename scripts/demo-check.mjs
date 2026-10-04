// Purpose: Exercise the synthetic gateway and fixture client together on an ephemeral loopback port.
import { once } from 'node:events';

process.env.JEV_GATEWAY_PORT = '0';
const { server } = await import('../fixture/server.mjs');

try {
  if (!server.listening) await once(server, 'listening');
  const address = server.address();
  if (!address || typeof address === 'string') throw new Error('Fixture gateway did not bind a TCP port');
  process.env.JEV_GATEWAY_URL = `http://127.0.0.1:${address.port}/v1/decision`;
  await import('../examples/fixture-client.mjs');
} finally {
  server.close();
  await once(server, 'close');
}
