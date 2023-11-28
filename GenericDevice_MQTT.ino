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
 */
//

//  fauxmoESP 3.X.X: When using Arduino Core for ESP8266 v2.4.X, double check you are
//                   building the project with LwIP Variant set to "v1.4 Higher Bandwidth".
//                   You can change it from the Tools menu in the Arduino IDE or passing
//                   the -DPIO_FRAMEWORK_ARDUINO_LWIP_HIGHER_BANDWIDTH build flag to
//                   PlatformIO.
//
//
//  For ESP 8266 and SonOff modules including SonOff USB 5V
//                      Set board to: "Generic ESP8266 Module";  
//                      Select Flash Mode: “DOUT”;
//                      Flash size: 1M NO SPIFFS; 
//                      Built-in LED: 13;  
//                      CPU/Flash Freq 80MHz/40MHz
//                      Erase Flash: Only Sketch
//
//  For ESP 32 - Cam
//                      Select Board "ESP32 Wrover Module"
//                      Select the Partion Scheme "Huge APP (3MB No OTA)"  (We may need 512K OTA!!!)
//                      GPIO 0 must be connected to GND to upload a sketch
//                      After connecting GPIO 0 to GND, press the ESP32-CAM
//                             on-board RESET button to put your board in flashing mode
//
//  For TYWE3S - TreatLife Switch
//                      Set board to: "Generic ESP8266 Module";
//                      CPU Freq: 80MHz Crystal Freq: 26MHz
//                      Flash size: 2MB (FS 128k, OTA 960K); 
//                      Select Flash Mode: “DOUT”;
//                      Built-in LED: 4 or 5;  
//                      Flash Freq 40MHz
//                      Reset Method:  dtr (aka nodemcu)
//                      Debug/Debug Level:  Disable/OTA
//                      IwIP:  v2 Lower Memory
//                      VTables:  Flash
//                      Exceptions:  Legacy
//                      Erase Flash: Only Sketch
//                      EspressifFW: 2.2.1+119
//                      SSL Support:  All SSL Ciphers (most compatible)
//
//  For WP2 - Gosung Dual-Plug, WP3 (Smart Socket)  or   Amysen JSM-WF02 
//                      Set board to: "Generic ESP8266 Module";
//                      CPU Freq: 80MHz Crystal Freq: 26MHz
//                      Flash size: 1MB (FS NONE, OTA 502K); 
//                      Select Flash Mode: “DOUT”;
//                      Built-in LED: 1;     (13 for Amysen)
//                      Flash Freq 40MHz
//                      Reset Method:  dtr (aka nodemcu)
//                      Debug/Debug Level:  Disable/OTA
//                      IwIP:  v2 Lower Memory
//                      VTables:  Flash
//                      Exceptions:  Legacy
//                      Erase Flash: Only Sketch
//                      EspressifFW: 2.2.1+119
//                      SSL Support:  All SSL Ciphers (most compatible)
//
//  For ESP8266Mod - NodeMCU 1.0 - ESP12E Module 
//                      Set board to: "NodeMCU 1.0 - ESP12E Module;
//                      CPU Freq: 80MHz Crystal Freq: 26MHz
//                      Flash size: 4MB (FS 2GB, OTA 1019KB); 
//                      Select Flash Mode: “DOUT”;
//                      Built-in LED: 2 (LED_BUILTIN);  
//                      Flash Freq 40MHz
//                      Reset Method:  dtr (aka nodemcu)
//                      Debug/Debug Level:  Disable/OTA
//                      IwIP:  v2 Lower Memory
//                      VTables:  Flash
//                      Exceptions:  Legacy
//                      Erase Flash: Only Sketch
//                      EspressifFW: 2.2.1+119
//                      SSL Support:  All SSL Ciphers (most compatible)
//
//


#include <Arduino.h>
#include <Wire.h>

#ifdef ESP32
  #include <EEPROM.h> 
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #define BUTTON                          4
  #define FLASH                           4
  #define LED                             2
  #define RELAY                           5
  #define DEBOUNCING                      70
#else  //8266:  Generic, Sonoff, ESP 12, TYWE3S
  #include <EEPROM.h>
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #if 0
    #define SERIALn                     Serial  // Sonoff  If GPIO01/03 is used, Serial cannot be used (Use Serial1)
    #define BUTTON_STATE                     0  // Sonoff
    #define BUTTON                           0  // Sonoff
    #define FLASH                            0  // Sonoff
    #define LED                             13  // Sonoff
    #define RELAY                           12  // Sonoff
    #define STATUS                          14  // Sonoff free GPIO 
    #define DEBOUNCING                      30
  #elif 0
    #define SERIALn                     Serial  // TreatLife  If GPIO01/03 is used, Serial cannot be used (Use Serial1)
    #define BUTTON_STATE                     0  // TreatLife
    #define BUTTON                          13  // TreatLife
    #define FLASH                           13  // TreatLife
    #define LED                              4  // TreatLife
    #define RELAY                           12  // TreatLife
    #define GPIO14                          14  // TreatLife
    #define GPIO15                          15  // TreatLife
    #define DEBOUNCING                      30
  #elif 0
    #define SERIALn                    Serial1  // Amysen JSM-WF02 2AOT8JSM-WF02 If GPIO01/03 is used, Serial cannot be used (Use Serial1)
    #define BUTTON_STATE                     0  // Amysen JSM-WF02
    #define BUTTON                           1  // Amysen JSM-WF02
    #define FLASH                       BUTTON  // Amysen JSM-WF02
    #define LED                             13  // Amysen JSM-WF02
    #define RELAY                           14  // Amysen JSM-WF02
    #define DEBOUNCING                      30
  #elif 0
    #define SERIALn                    Serial1  // Gosund WP2  If GPIO01/03 is used, Serial cannot be used (Use Serial1)
    #define BUTTON_STATE                     0  // Gosund WP2
    #define BUTTON                           5  // Gosund WP2
    #define BUTTON2                          0  // Gosund WP2
    #define FLASH                            5  // Gosund WP2
    #define LED                              2  // Gosund WP2
    #define LED2                             4  // Gosund WP2
    #define RELAY                           12  // Gosund WP2
    #define RELAY2                          14  // Gosund WP2
    #define DEBOUNCING                      30
   #elif 1
    #define SERIALn                    Serial1  // Gosund WP3  If GPIO01/03 is used, Serial cannot be used (Use Serial1)
    #define BUTTON_STATE                     0  // Gosund WP3
    #define BUTTON                           4  // Gosund WP3
    #define FLASH                            4  // Gosund WP3
    #define LED                             12  // Gosund WP3
    #define LED2                            12  // Gosund WP3
    #define RELAY                           14  // Gosund WP3
    #define DEBOUNCING                      30
 #elif 0
    #define SERIALn                     Serial  // ESP 12N  OR  NodeMCU 1.0 - ESP12E Module  If GPIO01/03 is used, Serial cannot be used (Use Serial1)
    #define BUTTON_STATE                     0  // ESP 12N  OR  NodeMCU 1.0 - ESP12E Module
    #define BUTTON                          13  // ESP 12N  OR  NodeMCU 1.0 - ESP12E Module
    #define FLASH                            0  // ESP 12N  OR  NodeMCU 1.0 - ESP12E Module
    #define LED                              2  // ESP 12N  OR  NodeMCU 1.0 - ESP12E Module
    #define RELAY                           14  // ESP 12N  (pin 12)  OR  NodeMCU 1.0 - ESP12E Module  (pin 14 -- or many others)
    #define DEBOUNCING                      30
  #endif
#endif

#include <PubSubClient.h>
#include <ESPAsyncWebServer.h>
#include <fauxmoESP.h>

#include "ConfigDevice_AP.h"

extern "C" {
#include "user_interface.h"
}

#define IR_PRESENT                      false
#define DAILY_RESTART                   false
#define WATCHDOG_RESTART                false

#if IR_PRESENT == true
#include <SparkFun_AK9750_Arduino_Library.h>
#endif

#define FORCE_AP                        false
#define SERIAL_BAUDRATE                 115200
#define OFF                             0
#define ON                              1
static char lightoff = '0';
static char lighton =  '1';
String str_dev_cmd;
String str_dev_light;
String str_dev_status;
String str_dev_timestamp;
String str_dev_mqtt_client;

const char mqtt_server[30] = "10.14.1.58";
WiFiClient espGenericPresenceClient;
PubSubClient mqtt_client(espGenericPresenceClient);

long last_attempt=0, num_attempts=0, time_stamp = 0, button_count=0;

// Timers auxiliar variables
long firstConnect = millis();
long now = millis();
long watchdog = millis();
char currState = OFF;
long lastDetection = 0;
long lastStatusReport = 0;
int  lastCommand = 0;

fauxmoESP fauxmo;
char messageTemp[300];

#if IR_PRESENT == true
AK9750 movementSensor; //Hook object to the library
#endif
int ir1, ir2, ir3, ir4, pir1=0, pir2=0, pir3=0, pir4=0, maxir;
#define  IRThreshold     -100
int threshold = IRThreshold;
float tempF;


void reset_device(boolean apmode) {
    for (int i=0; i<10; i++) {
       if (apmode == true) SERIALn.println("Preparing to reboot in AP mode\r\n");
       else SERIALn.println("Preparing to reboot in normal mode\r\n");
       digitalWrite(LED,0);
       delay(500);
       digitalWrite(LED,1);
       delay(500); }
    if (apmode == true) {
       saved_ssid[0] = '\0';
       EEPROM.put(0, saved_ssid);
       EEPROM.commit();
       EEPROM.end();
       delay(500); }
    ESP.restart(); }


void turnOnLight(int lc) {
   char  onstate = ON;
   SERIALn.printf("Turning on the %s Light...\r\n", saved_name);
   if ((saved_type[0] != 'S') && (saved_type[0] != 's')) {
      digitalWrite(LED, LOW);
      digitalWrite(RELAY, LOW);
      if (strtol(saved_type, NULL, 10) != 0)
         delay(strtol(saved_type, NULL, 10));
      else
         delay(200); }
   digitalWrite(LED, HIGH);
   digitalWrite(RELAY, HIGH);
   EEPROM.put(10*EE_STR, onstate);
   EEPROM.commit();
   sprintf(messageTemp, "%c", lighton);
   if (mqtt_client.connected()) {
      mqtt_client.publish(str_dev_cmd.c_str(), messageTemp); }
   SERIALn.printf(" %d ... done!\r\n ", currState);
   lastCommand = lc;
   watchdog = millis(); }


void turnOffLight(int lc) {
   char  offstate = OFF;
   SERIALn.printf("Turning off the %s Light...\r\n", saved_name);
   if ((saved_type[0] != 'S') && (saved_type[0] != 's')) {
      digitalWrite(LED, HIGH);
      digitalWrite(RELAY, HIGH);
      if (strtol(saved_type, NULL, 10) != 0)
         delay(strtol(saved_type, NULL, 10));
      else
         delay(200); }
   digitalWrite(LED, LOW);
   digitalWrite(RELAY, LOW);
   EEPROM.put(10*EE_STR, offstate);
   EEPROM.commit();
   sprintf(messageTemp, "%c", lightoff);
   if (mqtt_client.connected()) {
      mqtt_client.publish(str_dev_cmd.c_str(), messageTemp); }
   SERIALn.printf(" %d ... done!\r\n ", currState);
   lastCommand = lc;
   watchdog = millis(); }


void check_reset_button(int blink_count, int max_count) {
    if ((digitalRead(BUTTON) == BUTTON_STATE) || (digitalRead(FLASH) == BUTTON_STATE)) {
       SERIALn.printf("Button Pressed: %d \r\n", button_count);
       button_count++;
       if (button_count > blink_count) digitalWrite(LED,(button_count/(blink_count/2)) % 2); }
    else {
       //SERIALn.printf("Button Released: %d\r\n", button_count);
       if (button_count > DEBOUNCING) {      //  for debouncing 
          if (currState == ON) {
             turnOffLight(7);
             currState = OFF; }
          else {
             turnOnLight(8);
             currState = ON; } }
        if (button_count >= (max_count/2)) {  // Reset the device
          reset_device(false); }
        button_count = 0; }

    if (button_count >= max_count) {
       reset_device(true); } }


// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
void cmdGenericPresence(String topic, byte* message, unsigned int length) {
    long dt;

    dt = millis();
    // Ignores any spurious message in the first 10 seconds after reboot/reconnecting
    if ((dt - firstConnect) < 20000) {
       SERIALn.print("Ignoring Spurious MQTT msg\r\n");
       return; }

    for (int i = 0; i < length; i++) {
       //SERIALn.print((char)message[i]);
       messageTemp[i] = (char)message[i];
       messageTemp[i+1] = 0;
       message[i] = 0; } // erase buffer

    if (topic==str_dev_light) {
       SERIALn.print("[MQTT] Received topic ");
       SERIALn.print(topic);
       SERIALn.printf(": %s  (raw msge: %d bytes long %s)\r\n", (messageTemp[0] == '0') ? "OFF" : "ON", length, messageTemp);
       if (messageTemp[0] == '0') {        // OFF
            turnOffLight(1);
            #if IR_PRESENT == false
            currState = OFF;
            #endif
            }
       else if (messageTemp[0] == '1') {
            turnOnLight(2);
            #if IR_PRESENT == false
            currState = ON;
            #endif
            }
       else if (messageTemp[0] == 'A') {
            reset_device(true); }
       else if (messageTemp[0] == 'R') {
            reset_device(false); }
       else SERIALn.print("Received MQTT Spurious Message...\r\n");
       threshold = atoi(&messageTemp[2]); }
    else SERIALn.println("Wrong Topic\r\n");
    SERIALn.println(); }


// This functions connects your ESP8266 to your MQTT broker
boolean mqttConnect() {
    if (!WiFi.isConnected()) {
       if (WiFi.status() != WL_CONNECTED) {
          // WiFi.reconnect(); // reconnect or setup?
          wifiSetup(); // setup or just reconnect?
          SERIALn.printf("[WIFI] Connecting to %s ", WiFi.SSID().c_str());
          SERIALn.print("*"); }
       if (WiFi.isConnected()) {
          // By default, fauxmoESP creates it's own webserver on the defined port
          // The TCP port must be 80 for gen3 devices (default is 1901)
          // This has to be done before the call to enable()
          fauxmo.createServer(true); // not needed, this is the default value
          fauxmo.setPort(80); // This is required for gen3 devices

          // You have to call enable(true) once you have a WiFi connection
          // You can enable or disable the library at any moment
          // Disabling it will prevent the devices from being discovered and switched
          fauxmo.enable(true); }
       else return false; }

    SERIALn.print("Attempting MQTT connection...");
    // Attempt to connect

    mqtt_client.setServer(saved_mqtt, 1883);
    mqtt_client.setCallback(cmdGenericPresence);

    if (mqtt_client.connect(str_dev_mqtt_client.c_str())) {
         SERIALn.println("connected\r\n");
         // Subscribe or resubscribe to a topic
         mqtt_client.subscribe(str_dev_light.c_str()); }
    else {
         SERIALn.print("failed, rc=");
         SERIALn.print(mqtt_client.state());
         SERIALn.println(" try again in 5 seconds\r\n"); }

    return mqtt_client.connected(); }



/*****************************************
 ****************   WiFi   ***************
 *****************************************/

IPAddress local_IP(10,14,1,210);
IPAddress gateway(10,14,1,1);
IPAddress subnet(255,255,255,0);
  
void wifiSetup() {
    // Set WIFI module to STA mode and IP
    WiFi.mode(WIFI_STA);
    if ((saved_ip[0] == '\0') || ((saved_ip[0] >= '0') && (saved_ip[0] <= '9'))) {
       local_IP = str2ip(saved_ip);
       gateway = str2ip(saved_gwy);
       subnet = str2ip(saved_msk); }

    WiFi.config(local_IP, gateway, subnet);
    #ifdef ESP32
       WiFi.setHostname(saved_name);    // ESP 32
    #else
       WiFi.hostname(saved_name);        // 8266
    #endif

    wifi_station_set_hostname(saved_name);  //sdk, non-Arduino function


    // Connect
    SERIALn.printf("[WIFI] Connecting to %s ", saved_ssid);
    WiFi.begin(saved_ssid, saved_pswd);

    // save SSID/password for next reconnection
    // "false" = don't write to flash unless SSID/passord has changed
    //WiFi.persistent(false);

    SERIALn.println();

    WiFi.setAutoReconnect(true);
    if (WiFi.status() == WL_CONNECTED) {
       // Connected!
       SERIALn.printf("\r\nESP Board MAC Address:  ");
       SERIALn.println(WiFi.macAddress());
       SERIALn.printf("\r\n[WIFI] STATION Mode, SSID: %s, IP address: %s\r\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str()); } }



/*****************************************
 ****************   SETUP  ***************
 *****************************************/

void setup() {
    // Inputs/Outputs
    #ifdef STATUS
       pinMode(STATUS, INPUT_PULLUP);
    #endif
    #ifdef GPIO14
       pinMode(GPIO14, INPUT_PULLUP);
    #endif
    #ifdef GPIO15
       pinMode(GPIO15, INPUT_PULLUP);
    #endif
    pinMode(RELAY, OUTPUT);
    //digitalWrite(RELAY, LOW);
    pinMode(LED, OUTPUT);
    //digitalWrite(LED, LOW);
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(FLASH, INPUT_PULLUP);

    lastCommand = 0;

    delay(10000);   // wait for SERIALn handler on Mac after power switch of device

    // Init SERIALn port and clean garbage
    SERIALn.begin(SERIAL_BAUDRATE, SERIAL_8N1);//, SERIAL_TX_ONLY);
    while (!SERIALn) {
       delay(500); } // wait for serial port to connect.
    SERIALn.println("Ready\r\n");
    SERIALn.println();

    EEPROM.begin(512);

    EEPROM.get(0, saved_ssid);
    EEPROM.get(1*EE_STR, saved_pswd);
    EEPROM.get(2*EE_STR, saved_name);
    EEPROM.get(3*EE_STR, saved_type);
    EEPROM.get(4*EE_STR, saved_ip);
    EEPROM.get(5*EE_STR, saved_gwy);
    EEPROM.get(6*EE_STR, saved_msk);
    EEPROM.get(7*EE_STR, saved_mqtt);

    EEPROM.get(10*EE_STR, currState);

    if (currState == ON) {
       digitalWrite(RELAY, HIGH);
       digitalWrite(LED, HIGH); }
    else {
       digitalWrite(RELAY, LOW);
       digitalWrite(LED, LOW); }

    SERIALn.printf("Saved in EEPROM: %s %s %s %s %s %s %s %s\r\n",
           saved_ssid, saved_pswd, saved_name, saved_type, saved_ip, saved_gwy, saved_msk, saved_mqtt); 

    if ((saved_ssid[0] == '\0') || (saved_ssid[0] < '0') || (saved_ssid[0] > 'z') || (FORCE_AP == true)) {
       SERIALn.println("AP Mode Setup w/ OTA...\r\n");
       setupAP();
       return; }

    if ((saved_name[0] == '\0') || (saved_name[0] < '0') || (saved_name[0] > 'z')) {
       String("Test").toCharArray(saved_name, 5); }

    str_dev_cmd = "home/" + String(saved_name) + "/cmd";
    str_dev_light = "home/" + String(saved_name) + "/Light";
    str_dev_status = "home/" + String(saved_name) + "/status";
    str_dev_timestamp = "home/" + String(saved_name) + "/timestamp";
    str_dev_mqtt_client = String(saved_name) + "Client";

    if ((saved_mqtt[0] == '\0') || (saved_mqtt[0] < '0') || (saved_mqtt[0] > 'z')) {
       String(mqtt_server).toCharArray(saved_mqtt, sizeof(mqtt_server)+1) ; }

    // Wifi
    wifiSetup();
    // Wait for IP
    int count=0;
    char clk[5] = "|/-\\" ;
    while (WiFi.status() != WL_CONNECTED) {
        SERIALn.printf("%c\r",clk[count % 4]);
        delay(10);
        check_reset_button(1200, 5000);
        if ((count++) > 7000) {
           SERIALn.println("\r\n\r\n\rRebooting...");
           reset_device(false);} }
    
    if (WiFi.status() == WL_CONNECTED) {
       WiFi.setAutoReconnect(true);
       // Connected!
       SERIALn.printf("\r\nESP Board MAC Address:  ");
       SERIALn.println(WiFi.macAddress());
       SERIALn.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\r\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str()); }
    else {
       SERIALn.printf("\n Failed to get an IP\n"); }

    mqttConnect();

    #if IR_PRESENT == true
    // AK975X IR
    SERIALn.println("AK975X Read Setup\r\n");
    Wire.begin(0,2);
    //Turn on sensor
    if (movementSensor.begin() == false) {
       SERIALn.println("Device not found. Check wiring.\r\n");
       while (1); }
    #endif

    // By default, fauxmoESP creates it's own webserver on the defined port
    // The TCP port must be 80 for gen3 devices (default is 1901)
    // This has to be done before the call to enable()
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices

    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.enable(true);

    // Fauxmo
    fauxmo.addDevice(saved_name);

    // fauxmoESP 2.0.0 has changed the callback signature to add the device_id,
    // this way it's easier to match devices to action without having to compare strings.
    //fauxmo.onMessage([](unsigned char device_id, const char * device_name, bool state) {
    //    SERIALn.printf("[FAUX] Device #%d (%s) state (%d): %s\r\n", device_id, device_name, state, state ? "ON" : "OFF");
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        SERIALn.printf("[FAUX] Device #%d (%s) state: %s value: %d\r\n", device_id, device_name, state ? "ON" : "OFF", value);
        //if (device_id == 0)   // This app has only one device
        //if (strcmp(device_name, saved_name)==0)
        if (state == OFF) {
           turnOffLight(3);
           currState = OFF; }
        else if (state == OFF) {
           turnOnLight(4);
           currState = ON; }
        else if (state == 'A') {
            reset_device(true); }
        else if (state == 'R') {
            reset_device(false); }
    } ); }

#if IR_PRESENT == true
int IRDetect() {
    int detected = false;
    if (movementSensor.available())  {
       ir1 = movementSensor.getIR1();
       ir2 = movementSensor.getIR2();
       ir3 = movementSensor.getIR3();
       ir4 = movementSensor.getIR4();
       tempF = movementSensor.getTemperatureF();

       movementSensor.refresh(); //Read dummy register after new data is read

       //Note: The observable area is shown in the silkscreen.
       //If sensor 2 increases first, the human is on the left
       if (ir2 > threshold) { //(abs(ir2 - pir2) > threshold) {
          // SERIALn.print("Human Detected from Left \r\n");
          detected = true; }
       if (ir4 > threshold) { //(abs(ir4 - pir4) > threshold) {
          // SERIALn.print("Human Detected from Right \r\n");
          detected = true; }
       if (ir1 > threshold) { //(abs(ir1 - pir1) > threshold) {
          // SERIALn.print("Human Detected from Top \r\n");
          detected = true; }
       if (ir3 > threshold) { //(abs(ir3 - pir3) > threshold) {
          // SERIALn.print("Human Detected from Botton \r\n");
          detected = true; } }
    SERIALn.printf("Human Detected %d %d %d %d\r\n", ir1, ir2, ir3, ir4);
    pir1 = ir1; pir2 = ir2; pir3 = ir3; pir4 = ir4;
    return detected; }
#endif



/*****************************************
 ****************   LOOP  ****************
 *****************************************/

void loop() {
    long dt;
    static char publish_msg[200];

    if ((saved_ssid[0] == '\0') || (saved_ssid[0] < '0') || (saved_ssid[0] > 'z') || (FORCE_AP == true)) {
       SERIALn.println("AP Mode Loop w/OTA...\r\n");
       loopAP();
       return; }

    //SERIALn.println(saved_name);
    //SERIALn.println(saved_mqtt);

    //dt = millis();
    //// wait for connection at the first time it loops
    //if ((dt - firstConnect) < 2000) {
    //   SERIALn.print("Loop1!\r\n");
    //   return; }

    // Since fauxmoESP 2.0 the library uses the "compatibility" mode by
    // default, this means that it uses WiFiUdp class instead of AsyncUDP.
    // The later requires the Arduino Core for ESP8266 staging version
    // whilst the former works fine with current stable 2.3.0 version.
    // But, since it's not "async" anymore we have to manually poll for UDP
    // packets
    fauxmo.handle();

    //SERIALn.print("Loop2!\r\n");

    now = millis();
    #if IR_PRESENT == true
    if (IRDetect() == false) {  // if Generic is off
       if (currState == ON) {
          if ((now - lastDetection) > 10000) {
             turnOffLight(5);
             currState = OFF; }  }  }
    if (IRDetect() == true) {  // if Generic is on
       if (currState == OFF) {
          if ((now - lastDetection) < 500) {
             turnOnLight(6);
             currState = ON; } }
       lastDetection = now; }
    #endif

    // If your device state is changed by any other means (MQTT, physical button,...)
    // you can instruct the library to report the new state to Alexa on next request:
    // fauxmo.setState(ID_YELLOW, true, 255);

    now = millis();
    #if DAILY_RESTART
       if (now >= 86400000) reset_device(false);   // daily restart
    #endif

    #if WATCHDOG_RESTART
       if ((now-watchdog) >= 1800000) { // every half hour
          if (currState == ON) {
             turnOffLight(9);
             delay(3000);
             turnOnLight(10); }
          else {
             turnOnLight(10);
             delay(3000);
             turnOffLight(9); } }
    #endif

    //SERIALn.print("Loop3!\r\n");
    if ((!mqtt_client.connected()) || (!WiFi.isConnected())) {
       dt = millis();
       firstConnect = millis();
       check_reset_button(1200, 5000);
       //check_reset_button(2, 6);
       if ((dt - last_attempt) >= 4000) {
          last_attempt = dt ;
          if (mqttConnect()) {
             firstConnect = millis();
             last_attempt = 0; }
          else if (num_attempts++ > 20) {
             SERIALn.println("\r\n\r\n\rRebooting...");
             reset_device(false);} } }
    else {
       check_reset_button(1200, 5000);    
       mqtt_client.loop();
       now = millis();
       #ifdef STATUS
          currState = digitalRead(STATUS);
       #endif
       // Publishes GenericLight door status every 30 seconds
       if ((now - lastStatusReport) > 30000) {
          lastStatusReport = now;
          maxir = max(max(max(ir1, ir2), ir3), ir4);
          // Publishes Device Light status
          sprintf(publish_msg, "%c", (currState == ON) ? lighton : lightoff);
          mqtt_client.publish(str_dev_status.c_str(), publish_msg);
          sprintf(publish_msg, "%d %d %d %d %d %f %d %d %d", time_stamp++, ir1, ir2, ir3, ir4, tempF, maxir, threshold, lastCommand);
          mqtt_client.publish(str_dev_timestamp.c_str(), publish_msg); } }
}
