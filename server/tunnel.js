const localtunnel = require('localtunnel');

(async () => {
  const tunnel = await localtunnel({ port: 4201 });
  console.log(`LocalTunnel URL: ${tunnel.url}`);
})();
