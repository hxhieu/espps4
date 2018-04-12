var wifi = require('Wifi'),
  WebServer = require('WebServer');

var indexjs = atob(
  'ZnVuY3Rpb24gaGVsbG8oKXsgYWxlcnQoJ0hlbGxvIGZyb20gaW4gbWVtb3J5IGNsaWVudCBzaWRlIGphdmFzY3JpcHQnKTsgfQ=='
);

function onInit() {
  if (wifi)
    wifi.startAP(
      'ESP8266_PS4',
      { password: 'abcd_1234', authMode: 'wpa_wpa2' },
      startServer
    );
  else startServer();
}

function startServer() {
  var webs = new WebServer({
    port: 80,
    file_system: '/var/www/',
    memory: {
      'index.html': {
        type: 'text/html',
        content:
          '<html><head><script src="index.js"></script></head><body>' +
          '<p>Hello from in memory HTML!</p>' +
          '<br><button onclick="hello()"> Run client side JavaScript</button>' +
          "<br><br><button onclick=\"window.open('index.njs', '_top')\"> Run server side JavaScript</button>" +
          "<br><br><button onclick=\"window.open('index.txt', '_top')\"> Go to a simple text</button>" +
          '<br><p align="right"> <small>Espruino WebServer</small> </p>' +
          '</body></html>',
      },
      'index.txt': {
        content: 'Hello from in memory text!',
      },
      'index.js': {
        type: 'application/javascript',
        content: indexjs,
      },
      'index.njs': {
        content: index_njs,
      },
      'favicon.ico': {
        type: 'image/x-icon',
        content:
          '\0\0\x01\0\x01\0\x10\x10\x10\0\x01\0\x04\x00\xf0\0\0\0\x16\0\0\x00\x89PNG\x0d\x0a\x1a\x0a\0\0\0\x0dIHDR\0\0\0\x10\0\0\0\x10\x08\x06\0\0\0\x1f\xf3\xffa\0\0\x00\xb7IDAT8\x8d\xa5S\xc1\x0d\x03!\x0csN\xb7\x91w\xcaP\xde)3\xd1G\x09\x0a\x85\xab\xa8\xea\x0f\x02\x82c\x1b0\x92x\x82\xbb\xb7:\x8f\x08D\x84\xd5\xb5\x1b\x00H\xb6>N\x04uN\x12\x92\x10\x11S\xcd]\x0b\xbf\xa9\xe9\x8a\x00\xa0I\x1a*\x06A\x97\xb7\x90\xd4\x8e$A\x12\xee\xde\xb2vR\x90$\xc8q\xf6\x03\xbc\x15Ldw]\x88zpc\xab*\x8c\x08H\xb2A\x90\x1e\x97\xce\x1bd3\x00\xb8v\x9b\xa7p\xf7\xb6\x10\x9cb\xc9\xe0Wd\x06\x17\x80v\xe2\xfb\x09\x17\x00H\xfa\x8b\xc0\xba\x9c\xe3CU\xf1\xc8@\xd2\x08fW\xf8i3?U\x12\x18z\x16\xf5A\x9ddc_\xee\xbd~e{*z\x01|\xcdnfT\x03\x0an\0\0\0\x00IEND\xaeB`\x82',
      },
    },
  });

  webs.on('start', function(WebServer) {
    console.log('WebServer listening on port ' + WebServer.port);
  });
  webs.on('request', function(request, response, parsedUrl, WebServer) {
    console.log('WebServer requested', parsedUrl);
  });
  webs.on('error', function(error, WebServer) {
    console.log('WebServer error', error);
  });

  webs.createServer();
}

function index_njs(req, res, uri, webs) {
  return {
    type: 'text/html',
    content:
      '<html>' +
      '<p>Hello from in memory server side javascript!</p>' +
      '<p><b>Espruino Memory Usage: </b><br>' +
      JSON.stringify(process.memory()) +
      '</p>' +
      '<p><b>Espruino Flash Usage: </b><br>' +
      JSON.stringify(require('Flash').getFree()) +
      '</p>' +
      '</html>',
  };
}
