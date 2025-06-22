const ngrok = require('ngrok');

(async () => {
  const url = await ngrok.connect({ proto: 'tcp', addr: 14242 });
  console.log(`Ngrok TCP URL: ${url}`);
})();
