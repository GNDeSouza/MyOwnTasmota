/*
 *  Copyright (C) 2019 
 *  Created by G. N. DeSouza <Gui@DeSouzas.name>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation. Meaning:
 *          keep this copyright notice,
 *          do  not try to make money out of it,
 *          it's distributed WITHOUT ANY WARRANTY,
 *          yada yada yada...
 *
 *  You can get a copy of the GNU General Public License by writing to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1335 USA or at
 *  https://www.gnu.org/licenses/gpl-3.0.html
 *
 *
 */

#ifdef ESP32
#else
   #include <ESP8266mDNS.h>
#endif

#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#define     EE_STR     30

AsyncWebServer server(80);

char saved_ssid[EE_STR];
char saved_pswd[EE_STR];
char saved_devc[EE_STR];
char saved_ip[EE_STR];
char saved_gwy[EE_STR];
char saved_msk[EE_STR];
char saved_mqtt[EE_STR];

const char* ssid     = "ViGIR-GND-AP";
const char* password = "123456789";

String ssid_ap;
String pswd_ap;
String devc_ap;
String ip_ap;
String gwy_ap;
String msk_ap;
String mqtt_ap;

// HTML web page to handle multiple input fields
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
   <title>ESP Input Form (GND)</title>
   <meta name="viewport" content="width=device-width, initial-scale=2">
   </head><body>
   <h2>GenericDevice<h2>
   <h4>Configure your device</h4>
   <form action="/action_page">
   SSID_Name:<br>
   <input type="text" name="ssid_name" value="ViGIR">
   <br>
   SSID_Passwd:<br>
   <input type="text" name="ssid_pswd" value="Password">
   <br>
   Device_Name:<br>
   <input type="text" name="devc_name" value="Test">
   <br>
   Device_IP:<br>
   <input type="text" name="devc_ip" value="10.14.1.210">
   <br>
   Device_GWY:<br>
   <input type="text" name="devc_gwy" value="10.14.1.1">
   <br>
   Device_Mask:<br>
   <input type="text" name="devc_msk" value="255.255.255.0">
   <br>
   MQTT_IP:<br>
   <input type="text" name="mqtt_ip" value="10.14.1.58">
   <br><br>
   <input type="submit" value="Submit">
   </form>
</body></html>)rawliteral";


void notFound(AsyncWebServerRequest *request) {
   request->send(404, "text/plain", "Not found");
}


IPAddress local_IP_AP(192,168,1,1);
IPAddress gateway_AP(192,168,1,1);
IPAddress subnet_AP(255,255,255,0);

IPAddress str2ip (char *str) {
   unsigned char i, j=0;
   IPAddress ip = (0,0,0,0);

   for (i=0; i<16; i++) {
      while ((str[i] != '\0') && (str[i] != '.')) {
         ip[j] = ip[j]*10 + (str[i] - '0');
         i++; }
      if (str[i] == '\0') break;
      j++; } 
   return ip; }



/*****************************************
 ****************   SETUP  ***************
 *****************************************/

void setupAP(){
   // Serial port for debugging purposes
   Serial.print("Setting AP (Access Point)…");

   // Remove the password parameter, if you want the AP (Access Point) to be open
   // WiFi.softAP(ssid, password);
   WiFi.softAP(ssid);
   delay(5000);
   // The use of config after softAP and delays seem to resolve an exception/abort
   // problem with ESP32s-Cam in AP mode
   WiFi.softAPConfig(local_IP_AP, gateway_AP, subnet_AP);
   delay(5000);

   IPAddress IP = WiFi.softAPIP();
   Serial.print("AP IP address: ");
   Serial.println(IP);

   //===============================================================
   // This routine is executed when you open the AP's IP in browser
   //===============================================================
   // Send web page with input fields to client
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send_P(300, "text/html", index_html);
   });

   //===============================================================
   // This routine is executed when you press submit
   //===============================================================
   //form action is handled here
   server.on("/action_page", HTTP_GET, [] (AsyncWebServerRequest *request){
     ssid_ap = request->arg("ssid_name");
     pswd_ap = request->arg("ssid_pswd");
     devc_ap = request->arg("devc_name");
     ip_ap = request->arg("devc_ip");
     gwy_ap = request->arg("devc_gwy");
     msk_ap = request->arg("devc_msk");
     mqtt_ap = request->arg("mqtt_ip");

     String s = "<h2> Configured: ssid_name " + ssid_ap + "<br>" +
                    "            ssid_pswd " + pswd_ap + "<br>" +
                    "            devc_name " + devc_ap + "<br>" +
                    "            devc_ip   " + ip_ap + "<br>" +
                    "            devc_gwy  " + gwy_ap + "<br>" +
                    "            devc_msk  " + msk_ap + "<br>" +
                    "            mqtt_ip   " + mqtt_ap + "<br>" +
                    "<a href='/'> Go Back </a> </h2>"; //Send web page

     request->send(300, "text/html", s); //Send web page
   });

   server.onNotFound(notFound);

   // Start server
   server.begin();

   // Port defaults to 8266
      ArduinoOTA.setPort(8266);

   // Hostname defaults to esp8266-[ChipID]
      ArduinoOTA.setHostname("ViGIR-GND-0001");

   // No authentication by default
   // ArduinoOTA.setPassword((const char *)"123");

   // Password can be set with it's md5 value as well
   // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
   // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

   ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
         type = "sketch"; }
      else { // U_FS
         type = "filesystem"; }

      // NOTE: if updating FS this would be the place to unmount FS using FS.end()
      Serial.println("Start updating " + type); });
   ArduinoOTA.onEnd([]() {
      Serial.println("\r\nEnd"); });
   ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
   ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
         Serial.println("Auth Failed\r\n"); }
      else if (error == OTA_BEGIN_ERROR) {
         Serial.println("Begin Failed\r\n"); }
      else if (error == OTA_CONNECT_ERROR) {
         Serial.println("Connect Failed\r\n"); }
      else if (error == OTA_RECEIVE_ERROR) {
         Serial.println("Receive Failed"); }
      else if (error == OTA_END_ERROR) {
         Serial.println("End Failed\r\n"); } });
  ArduinoOTA.begin();}


/*****************************************
 ****************  LoopAP  ***************
 *****************************************/
 
void loopAP() {
   IPAddress ip = (0,0,0,0);

   ArduinoOTA.handle();

   if (ssid_ap.length() == 0) {
      Serial.println("Waiting...\r\n");
      delay(2000); }
   else {
      Serial.print("WiFi Name:");
      Serial.println(ssid_ap);

      Serial.print("WiFi Password:");
      Serial.println(pswd_ap);

      Serial.print("Device Name:");
      Serial.println(devc_ap);

      ssid_ap.toCharArray(saved_ssid, ssid_ap.length()+1);  // +1 to include \0
      pswd_ap.toCharArray(saved_pswd, pswd_ap.length()+1);
      devc_ap.toCharArray(saved_devc, devc_ap.length()+1);
      ip_ap.toCharArray(saved_ip, ip_ap.length()+1);
      gwy_ap.toCharArray(saved_gwy, gwy_ap.length()+1);
      msk_ap.toCharArray(saved_msk, msk_ap.length()+1);
      mqtt_ap.toCharArray(saved_mqtt, mqtt_ap.length()+1);

      ip = str2ip(saved_ip);
      Serial.print("\r\nDevice IP:");
      Serial.println(saved_ip);
      Serial.printf("  %d, %d, %d, %d\r\n", ip[0], ip[1], ip[2], ip[3]);
      ip = str2ip(saved_gwy);
      Serial.print("\r\nDevice GWY:");
      Serial.println(saved_gwy);
      Serial.printf("  %d, %d, %d, %d\r\n", ip[0], ip[1], ip[2], ip[3]);
      ip = str2ip(saved_msk);
      Serial.print("\r\nDevice MSK:");
      Serial.println(saved_msk);
      Serial.printf("  %d, %d, %d, %d\r\n", ip[0], ip[1], ip[2], ip[3]);
      ip = str2ip(saved_mqtt);
      Serial.print("\r\nDevice MQTT:");
      Serial.println(saved_mqtt);
      Serial.printf("  %d, %d, %d, %d\r\n", ip[0], ip[1], ip[2], ip[3]);

      EEPROM.put(0, saved_ssid);
      EEPROM.put(1*EE_STR, saved_pswd);
      EEPROM.put(2*EE_STR, saved_devc);
      EEPROM.put(3*EE_STR, saved_ip);
      EEPROM.put(4*EE_STR, saved_gwy);
      EEPROM.put(5*EE_STR, saved_msk);
      EEPROM.put(6*EE_STR, saved_mqtt);
      EEPROM.commit();

      // server.handleClient();          //Handle client requests
      delay(2000);
      Serial.println("Preparing to reboot in Device mode\r\n");
      ESP.restart(); } }
