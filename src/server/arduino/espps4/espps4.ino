// Original sketch is from here https://github.com/treyjazz/ESPS4ExploitServer
// Modified by hxhieu

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <ESP8266mDNS.h>
#include <DNSServer.h>
//#include <WiFiUdp.h>

#include <FS.h>
#include <ArduinoJson.h>
#include <rBase64.h>

//#define NO_CONFIG 1
bool noConfig = false;
#define DEBUG Serial

//DNS settings
const byte DNS_PORT = 53;
DNSServer dnsServer;

//payload settings
char jsCounter = 0;
char JS_MAX = 7;
WiFiClient ps4;
String ps4IP;
int ps4Port = 9020;
#define PACKET_SIZE 4096
char plPacket[ PACKET_SIZE ];
#define PAYLOAD_WAIT 0

ESP8266WebServer server( 80 );
String host = "ps4exploit";

//AP settings
bool revertToAP = false;
IPAddress local_IP( 10, 13, 37, 1 );
IPAddress gateway( 10, 13, 37, 9 );
IPAddress subnet( 255, 255, 255, 0 );

//default settings
String wifiMode = "ap";
String apName = "ps4exploit";
String apKey = "hackmyps4";
String stName = "yourrouter";
String stKey = "routerpassword";
String stAddress = "235";
String payload = "/henblock.bin";

#define resetPin D5

void uploadPayload() {

  DEBUG.println( ps4IP );
  int tCounter = 0;

  while ( !ps4.connect( ps4IP.c_str(), ps4Port ) ) {

    tCounter++;

    if ( tCounter > 5000 ) {

      DEBUG.println( "Couldn't connect" );
      jsCounter = 0;
      ps4.stop();
      return;

    }

  }

  File file = SPIFFS.open( payload, "r" );

  if ( !file ) {

    DEBUG.printf( "%s couldn't be opened!\n", payload.c_str() );
    ps4.stop();
    return;

  }
  int filelen = file.size();
  //DEBUG.println( payload );
  DEBUG.printf( "Sending payload %s to %s port %d\r\n", file.name(), ps4IP.c_str(), ps4Port );

  //while ( ps4.available() ) { ps4.read(); }

  int totalSent = 0;

  while ( file.available() > 0 ) {

    digitalWrite ( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
    //delay( PAYLOAD_WAIT );
    //while ( ps4.available() ) { DEBUG.println( ps4.read() ); }

    int i = file.readBytes(plPacket, PACKET_SIZE );
    totalSent += ps4.write( ( const uint8_t* )plPacket, i );
    DEBUG.printf( "%d\%%\n", ( int )( ( totalSent * 100 ) / filelen ) );
    ps4.flush();
    digitalWrite ( LED_BUILTIN, HIGH );

  }

  file.close();
  DEBUG.println( "Payload sent" );
  ps4.stop();
  jsCounter = 0;


}

void loadConfig() {

  File file = SPIFFS.open( "/settings.ini", "r" );

  if ( !file ) {

    DEBUG.println( "settings.ini couldn't be opened!" );
    return;

  }

  String thisLine;

  while ( 1 ) {

    thisLine = file.readStringUntil( '\r' );
    thisLine.trim();
    if ( thisLine.indexOf( "wifimode=" ) == 0 ) wifiMode = thisLine.substring( 9 );
    if ( thisLine.indexOf( "apname=" ) == 0 ) apName = thisLine.substring( 7 );
    if ( thisLine.indexOf( "apkey=" ) == 0 ) apKey = thisLine.substring( 6 );
    if ( thisLine.indexOf( "stname=" ) == 0 ) stName = thisLine.substring( 7 );
    if ( thisLine.indexOf( "stkey=" ) == 0 ) stKey = thisLine.substring( 6 );
    if ( thisLine.indexOf( "staddress=" ) == 0 ) stAddress = thisLine.substring( 10 );
    if ( thisLine.indexOf( "payload=" ) == 0 ) payload = thisLine.substring( 8 );
    if ( ( thisLine.length() == 0 ) || !thisLine.indexOf( "=" ) ) break;

  }

  file.close();
  DEBUG.println( "settings.ini loaded" );

}

void saveConfig() {

  File file = SPIFFS.open( "/settings.ini", "w" );

  for (int i = 0; i < server.args(); i++ ) {

    String settingsLine = server.argName( i ) + "=" + server.arg( i );
    file.println( settingsLine.c_str() );

  }

  file.close();
  file = SPIFFS.open( "/settings.ini", "r" );
  char tempChar[ 400 ];
  file.readBytes( tempChar, 400 );
  DEBUG.println( tempChar );
  file.close();
  loadConfig();

}

String getContentType( String filename ) {

  if ( server.hasArg( "download" ) ) return "application/octet-stream";
  else if ( filename.endsWith( ".htm") ) return "text/html";
  else if ( filename.endsWith( ".html" ) ) return "text/html";
  else if ( filename.endsWith( ".css" ) ) return "text/css";
  else if ( filename.endsWith( ".js" ) ) return "application/javascript";
  else if ( filename.endsWith( ".png" ) ) return "image/png";
  else if ( filename.endsWith( ".gif" ) ) return "image/gif";
  else if ( filename.endsWith( ".jpg" ) ) return "image/jpeg";
  else if ( filename.endsWith( ".ico" ) ) return "image/x-icon";
  else if ( filename.endsWith( ".gz" ) ) return "application/x-gzip";
  return "text/plain";

}

bool handleFileRead( String path ) {

  path = path.substring( path.lastIndexOf( "/" ) );
  //DEBUG.println( path );
  ps4IP = server.client().remoteIP().toString();

  if ( path.endsWith( ".html" ) || path.endsWith( ".htm" ) ) {

    jsCounter = 0;
    JS_MAX = 100;

  }

  if ( path.endsWith( ".js" ) ) jsCounter ++;
  DEBUG.println( "handleFileRead: " + path );
  if ( path.endsWith( "/" ) ) path = "/index.html";
  if ( path.length()  < 3 ) path = "/index.html";
  //if ( !path.startsWith( "/" ) ) path = "/" + path;
  //if ( path.indexOf("#")) DEBUG.println( path.substring(path.indexOf( "#" ) + 1 ) );
  //set up trigger for payload transfer
  if ( path.endsWith( "idc-index.html" ) ) JS_MAX = 7;
  if ( path.endsWith( "specter-index.html" ) ) JS_MAX = 5;
  String contentType = getContentType( path );
  String pathWithGz = path + ".gz";

  if ( SPIFFS.exists( pathWithGz ) || SPIFFS.exists( path ) ) {

    digitalWrite ( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
    if ( SPIFFS.exists( pathWithGz ) ) path += ".gz";
    File file = SPIFFS.open( path, "r" );
    size_t sent = server.streamFile( file, contentType );
    file.close();
    digitalWrite ( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
    return true;

  }

  return false;

}

String apiGetPayloads(void) {
  // Should support 20++ payloads, which is plenty to be stored in the SPIFFS
  StaticJsonBuffer<2048> jsonBuffer;
  JsonArray& root = jsonBuffer.createArray();

  Dir dir = SPIFFS.openDir("/payload_");
  while (dir.next()) {
    JsonObject& payload = root.createNestedObject();
    payload.set(dir.fileName(), dir.fileSize());
  }

  String json;
  root.printTo(json);
  return json;
}


float spiffsTotalBytes;
float spiffsUsedBytes;

void startSPIFFS(void) {
  SPIFFS.begin();
  FSInfo fs_info;
  SPIFFS.info(fs_info);

  spiffsTotalBytes = (float)fs_info.totalBytes;
  spiffsUsedBytes = (float)fs_info.usedBytes;
  float fileTotalKB = spiffsTotalBytes / 1024.0;
  float fileUsedKB =  spiffsUsedBytes / 1024.0;

  float flashChipSize = (float)ESP.getFlashChipSize() / 1024.0 / 1024.0;
  float realFlashChipSize = (float)ESP.getFlashChipRealSize() / 1024.0 / 1024.0;
  float flashFreq = (float)ESP.getFlashChipSpeed() / 1000.0 / 1000.0;
  FlashMode_t ideMode = ESP.getFlashChipMode();

  DEBUG.printf("\n#####################\n");

  DEBUG.printf("__________________________\n\n");
  DEBUG.println("Firmware: ");
  DEBUG.printf("    Chip Id: %08X\n", ESP.getChipId());
  DEBUG.print("    Core version: "); DEBUG.println(ESP.getCoreVersion());
  DEBUG.print("    SDK version: "); DEBUG.println(ESP.getSdkVersion());
  DEBUG.print("    Boot version: "); DEBUG.println(ESP.getBootVersion());
  DEBUG.print("    Boot mode: "); DEBUG.println(ESP.getBootMode());


  DEBUG.printf("__________________________\n\n");

  DEBUG.println("Flash chip information: ");
  DEBUG.printf("    Flash chip Id: %08X (for example: Id=001640E0  Manuf=E0, Device=4016 (swap bytes))\n", ESP.getFlashChipId());
  DEBUG.printf("    Sketch thinks Flash RAM is size: ");  DEBUG.print(flashChipSize); DEBUG.println(" MB");
  DEBUG.print("    Actual size based on chip Id: "); DEBUG.print(realFlashChipSize); DEBUG.println(" MB ... given by (2^( \"Device\" - 1) / 8 / 1024");
  DEBUG.print("    Flash frequency: "); DEBUG.print(flashFreq); DEBUG.println(" MHz");
  DEBUG.printf("    Flash write mode: %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  DEBUG.printf("__________________________\n\n");

  DEBUG.println("File system (SPIFFS): ");
  DEBUG.print("    Total KB: "); DEBUG.print(fileTotalKB); DEBUG.println(" KB");
  DEBUG.print("    Used KB: "); DEBUG.print(fileUsedKB); DEBUG.println(" KB");
  DEBUG.printf("    Block size: %lu\n", fs_info.blockSize);
  DEBUG.printf("    Page size: %lu\n", fs_info.pageSize);
  DEBUG.printf("    Maximum open files: %lu\n", fs_info.maxOpenFiles);
  DEBUG.printf("    Maximum path length: %lu\n\n", fs_info.maxPathLength);

  Dir dir = SPIFFS.openDir("/");
  DEBUG.println("SPIFFS directory {/} :");
  while (dir.next()) {
    DEBUG.print("  "); DEBUG.println(dir.fileName());
  }

  DEBUG.printf("__________________________\n\n");

  DEBUG.printf("CPU frequency: %u MHz\n\n", ESP.getCpuFreqMHz());
  DEBUG.print("#####################");
}

String apiGetSPIFFS(void) {
  StaticJsonBuffer<80> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["total"] = spiffsTotalBytes;
  root["used"] = spiffsUsedBytes;

  String json;
  root.printTo(json);
  return json;
}

void handleApiRequest(String path) {
  String json;
  HTTPMethod m = server.method();
  String verb = "GET";

  if (m == HTTP_POST) {
    verb = "POST";
  }

  if (path.endsWith("/payloads")) {
    json = apiGetPayloads();
    server.send(200, "application/json", json);
  }
  else if (path.endsWith("/file-system")) {
    json = apiGetSPIFFS();
    server.send(200, "application/json", json);
  }

  DEBUG.println(verb + " " + path + " -> " + json);
}

void handleExploitEntry(String path) {
  path = path.substring( path.lastIndexOf( "/" ) + 1 );
  rbase64.decode(path);
  String payload = rbase64.result();

  DEBUG.println(payload);

  File f = SPIFFS.open(payload, "r");

  if (!f) {
    server.send(500, "Payload does not exist");
    return;
  }

  // Read the exploit.html
  String html = "<html><head><title>ESPPS4 Entry Point</title> <script src=" + payload + "></script> </head><body> <script src=" + "/exploit_4.05.js" + "></script> <h1 style='display:none' id=clck>...</h1> <pre id='console' style='font-size: 20px'></pre></body></html>";

  DEBUG.println(html);

  server.send( 200, "text/html", html );
}

void setup( void ) {

  pinMode( resetPin, INPUT_PULLUP );
  pinMode( LED_BUILTIN, OUTPUT );
  WiFi.forceSleepBegin();
  DEBUG.begin( 115200 );
  DEBUG.print( "\n" );
  //DEBUG.setDebugOutput( true ); //causing UART problems

  startSPIFFS();

  if ( digitalRead( resetPin ) == LOW ) {

    DEBUG.println( "Config loading bypassed!" );
    noConfig = true;
    wifiMode = "ap";

  }

#ifndef NO_CONFIG

  if ( noConfig == false ) loadConfig();

#endif

  if ( wifiMode == "ap" ) {

    WiFi.mode( WIFI_AP );

    if ( WiFi.softAPConfig( local_IP, gateway, subnet ) == false ) {

      DEBUG.println( "Couldn't create AP" );

    }
    else {

      DEBUG.printf( "Creating AP " );
      DEBUG.println( apName );

    }

    while ( WiFi.softAP( apName.c_str(), apKey.c_str() ) == false ) {

      delay( 500 );
      DEBUG.print( "." );

    }

    if ( WiFi.softAPIP() == IPAddress( 0, 0, 0, 0 ) ) revertToAP = true;
    else {

      DEBUG.println( "" );
      DEBUG.print( "Connected! IP address: " );
      DEBUG.println( WiFi.softAPIP() );

    }

  }
  else {

    DEBUG.printf( "Connecting to %s\n", stName.c_str() );
    WiFi.mode( WIFI_STA );
    WiFi.begin( stName.c_str(), stKey.c_str() );

    int reconCounter = 0;

    while ( WiFi.status() != WL_CONNECTED ) {

      delay( 500 );
      DEBUG.print( "." );

      reconCounter++;

      if ( reconCounter == 20 ) {

        revertToAP = true;
        break;

      }

    }

    IPAddress tempIP = WiFi.localIP();
    IPAddress goodIP = WiFi.localIP();
    IPAddress tempSubnet = WiFi.subnetMask();
    char changed = 0;

    for (int i = 0; i < 4; i++ ) {

      if (  tempSubnet[ i ] == 0 ) {

        tempIP[ i ] = stAddress.toInt();
        changed++;

      }

    }

    if (changed == 1 ) {

      WiFi.config( tempIP , WiFi.gatewayIP(), WiFi.subnetMask(), WiFi.dnsIP() );
      reconCounter = 0;

      while ( WiFi.status() != WL_CONNECTED ) {

        delay( 500 );
        DEBUG.print( "." );

        reconCounter++;

        WiFi.config( goodIP , WiFi.gatewayIP(), WiFi.subnetMask(), WiFi.dnsIP() ); //revert back in case of collision, etc.

        if ( reconCounter == 20 ) {

          revertToAP = true;
          break;

        }

      }

    }

    if ( WiFi.localIP() == IPAddress( 0, 0, 0, 0 ) ) revertToAP = true;
    else {

      DEBUG.println( "" );
      DEBUG.print( "Connected! IP address: " );
      DEBUG.println( WiFi.localIP() );

    }

  }

  if ( revertToAP == true ) {

    DEBUG.println( "Reverting to default AP" );
    wifiMode = "ap";
    WiFi.mode( WIFI_AP );
    WiFi.softAPConfig( local_IP, gateway, subnet );

  }

  if ( noConfig == true ) loadConfig();
  MDNS.begin( host.c_str()  );

  if ( wifiMode == "ap" ) dnsServer.start( DNS_PORT, "*", WiFi.softAPIP() );
  else dnsServer.start( DNS_PORT, "*", WiFi.localIP() );

  server.onNotFound( []() {
    String path = server.uri();

    // API wildcard
    if (path.indexOf("/api/") >= 0) {
      handleApiRequest(path);
      return;
    }

    // Entry point wildcard
    if (path.indexOf("/entry/") >= 0) {
      handleExploitEntry(path);
      return;
    }

    if ( !handleFileRead( server.uri() ) ) {

      DEBUG.printf( "URI not found " );

    }
    //DEBUG.println( server.uri() );

  } );

  server.on( "/", HTTP_GET, [] () {

    handleFileRead( "/index.html" );

  } );

  server.on( "/settings", HTTP_GET, [] () {

    String sPage;

    if ( server.args() > 0 ) {

      saveConfig();
      sPage = "<HTML><HEAD><meta http-equiv=\"refresh\" content=\"1; url=/index.html\"/></HEAD><TITLE>PS4 Exploit Settings</TITLE><BODY>Settings saved. Reset the card if network settings were changed!</BODY></HTML>";
      server.send( 200, "text/html", sPage );
      return;

    }

    loadConfig();

    sPage = "<HTML><HEAD><LINK REL=\"icon\" TYPE=\"image/png\" HREF=\"/ps4icon.png\"/><TITLE>PS4 Exploit Settings</TITLE></HEAD>\n<BODY>\n";
    sPage += "<FORM ID=\"form1\">\n<P>Wifi Mode</P>\n<DIV STYLE=\"margin-left: 1em;\">\n<INPUT TYPE =\"radio\" ID=\"ap\" NAME=\"wifimode\" VALUE=\"ap\"";
    if ( wifiMode == "ap" ) sPage +=  " checked";
    sPage += ">\n<LABEL FOR=\"ap\">Access Point</LABEL>\n<BR></BR>\n";
    sPage += "<DIV STYLE=\"margin-left: 2em;\">\nName: <INPUT ID=\"apname\" TYPE=\"text\" NAME=\"apname\" VALUE=\"" + apName + "\" onkeyup=\"apnameverify()\">\n<LABEL ID=\"apnamelabel\"></LABEL>\n<BR></BR>\n";
    sPage += "Password: <INPUT TYPE=\"password\" ID=\"apkey\" NAME=\"apkey\" VALUE=\"" + apKey + "\" onkeyup=\"apkeyverify()\">\n<LABEL ID=\"apkeylabel\"></LABEL>\n<BR></BR>\n</DIV>\n";
    sPage += "<INPUT TYPE =\"radio\" ID=\"sta\" NAME=\"wifimode\" VALUE=\"sta\"";
    if ( wifiMode == "sta" ) sPage += " checked";
    sPage += ">\n<LABEL FOR=\"sta\">Station</LABEL>\n<BR></BR>\n</DIV>\n<DIV STYLE=\"margin-left: 2em;\">\n";
    int n = WiFi.scanNetworks();

    if ( n == 0 ) sPage += "No networks found.\n<BR></BR>";
    else {

      for (int i = 0; i < n; ++i) {

        sPage += "<INPUT TYPE =\"radio\" ID=\"station" + String(i) + "\" NAME=\"stname\" VALUE=\"" + WiFi.SSID(i) + "\" ";
        if ( WiFi.SSID(i) == stName ) sPage += "checked";
        sPage += ">\n<LABEL FOR=\"station" + String(i, DEC) + "\">" + WiFi.SSID(i);
        if ( WiFi.encryptionType(i) == ENC_TYPE_NONE ) sPage += " (open)";
        sPage += "</LABEL>\n<BR></BR>\n";

      }

      sPage += "<DIV STYLE=\"margin-left: 2em;\">\nPassword: <INPUT TYPE=\"password\" NAME=\"stkey\" VALUE=\"" + stKey + "\">\n<BR></BR>\n";
      sPage += "Static IP: <INPUT TYPE=\"text\" ID=\"staddress\" NAME=\"staddress\" VALUE=\"" + stAddress + "\" onkeypress=\"return isNumber(event)\" onkeyup=\"staddressverify()\">\n<LABEL ID=\"staddresslabel\"></LABEL>\n<BR></BR>\n</DIV>\n";
    }

    sPage += "</DIV>\n<P>Payload</P>\n<DIV STYLE=\"margin-left: 1em;\">";

    Dir dir = SPIFFS.openDir( "/" );
    char firstFound = 0;

    while ( dir.next() ) {

      String fileName = dir.fileName();

      if ( fileName.endsWith( ".bin" ) ) {

        //String temp = fileName.substring( 1 );
        sPage += "\n<INPUT TYPE=\"radio\" ID=\"payload" + String(firstFound) + "\" NAME=\"payload\" VALUE=\"" + fileName + "\" " ;
        if ( fileName == payload ) sPage += "checked";
        firstFound ++;
        sPage += ">\n<LABEL FOR=\"payload" + String(firstFound, DEC) + "\">" + fileName.substring( 1 ) + "</LABEL>\n<BR></BR>";

      }

    }

    if ( firstFound == 0 ) {

      sPage += "\nNo payloads found!\n<BR></BR>";
      payload = "";

    }

    sPage += "\n</DIV>\n<DIV>\n</FORM>\n<BUTTON TYPE=\"submit\" ID=\"submitbtn\" FORM=\"form1\" VALUE=\"Submit\">Save</BUTTON>\n\n";
    sPage += "<SCRIPT>\n\nfunction apnameverify() {\nendissubmit();\nif ( apname.value.length < 1 ) {\ndocument.getElementById('apnamelabel').innerHTML = \"Fill in an AP name\";\n}\nelse { document.getElementById('apnamelabel').innerHTML = \"\";\n}\n}\n";
    sPage += "\nfunction apkeyverify() {\nendissubmit();\nif ( apkey.value.length < 8 ) {\ndocument.getElementById('apkeylabel').innerHTML = \"Password must be at least 8 characters in length\";\n}\nelse { document.getElementById('apkeylabel').innerHTML = \"\";\n}\n}\n";
    sPage += "\nfunction endissubmit() {\nif ( ( apname.value.length < 1 ) || ( apkey.value.length < 8 ) ) {\nsubmitbtn.disabled = true;\n}\nelse {\nsubmitbtn.disabled = false;\n}\n}\n";
    sPage += "\nfunction isNumber( evt ) {\nnevt = ( evt ) ? evt : window.event;\nvar charCode = ( evt.which ) ? evt.which : evt.keyCode;\nif ( charCode > 31 && ( charCode < 48 || charCode > 57 ) ) {\nreturn false;\n}\nreturn true;\n}\n";
    sPage += "\nfunction staddressverify() {\nvar x= parseInt( staddress.value );\nif ( staddress.value == \"\" || ( ( x < 2 ) || ( x > 255 ) ) ) {\ndocument.getElementById('staddresslabel').innerHTML = \"Must be in range of 2 to 255\";\n}\nelse {\ndocument.getElementById('staddresslabel').innerHTML = \"\";\n}\n}\n\n";
    sPage += "</SCRIPT>\n</DIV>\n<BR></BR>\n</BODY>\n</HTML>";

    server.send( 200, "text/html", sPage );

  });

  server.begin();
  DEBUG.println( "HTTP server started" );

}

void loop( void ) {


  dnsServer.processNextRequest();
  server.handleClient();
  if ( DEBUG.available() ) uploadPayload(); //force resend of payload on serial RX

  if ( jsCounter == JS_MAX ) {

    delay( 1000 );
    uploadPayload();

  }

  digitalWrite( LED_BUILTIN, HIGH );

}
