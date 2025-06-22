const localtunnel = require('localtunnel');

(async () => {
  const tunnel1 = await localtunnel({ port: 4201 });
  console.log(`LocalTunnel 1 URL (port 4201): ${tunnel1.url}`);

  const tunnel2 = await localtunnel({ port: 4202 });
  console.log(`LocalTunnel 2 URL (port 4202): ${tunnel2.url}`);
})();
