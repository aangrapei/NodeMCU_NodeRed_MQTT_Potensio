/*
 * ==============================NODEMCU MQTT NODE-RED=================================
 * 
 * ===================================REMOTE LED=======================================
 * 
 * ==================================AANG RAPE'I=======================================
 * 
 * ==================================27-01-2021========================================
 * 
 * ============================= KONEKSI PIN DAN LED ==============
`* ANODA LED 1 KE PIN D5 -- DISERI DENGAN RESISTOR 330 OHM -- KATODA LED 1 KE GROUND
`* ANODA LED 2 KE PIN D6 -- DISERI DENGAN RESISTOR 330 OHM -- KATODA LED 2 KE GROUND
`* ANODA LED 3 KE PIN D7 -- DISERI DENGAN RESISTOR 330 OHM -- KATODA LED 3 KE GROUND
 */
#include <ESP8266WiFi.h> //Library  ESP8266
#include <PubSubClient.h> //Library MQTT

#define potPin A0
#define ledPin D3

int nilai;
//Setup WIFI
const char*   ssid        = "MODEM"; //Nama SSID Wifi yang akan diakses!
const char*   pass        = "tarungjaya"; //Password Wifi

//Setup MQTT broker
const char*   mqtt_server = "broker.hivemq.com"; //Server MQTT/Broker
const int     mqtt_port   = 1883; //MQTT Port
const char*   mqttuser    = ""; //MQTT Username 
const char*   mqttpass    = ""; //MQTT Password
String        clientId    = "ESP8266Client-"; //MQTT ClientID



WiFiClient espclient;
PubSubClient client(espclient);

//Fungsi Menyambungkan ke Wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


//Fungsi Menyambungkan Ulang ke MQTT Broker
void reconnect() {
  while (!client.connected()) {
    clientId += String(random(0xffff), HEX);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(mqtt_server);
    if (client.connect(clientId.c_str(), mqttuser, mqttpass )) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

//Fungsi ini dijalankan sekali ketika NodeMCU mulai start
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);  
}

//Fungsi ini dijalakan berulang-ulang selama NodeMCU menyala
void loop(){
//ANALOG READ POTENSIO
nilai = analogRead(potPin);
 
 if (!client.connected()) { //menyambungkan kembali jika terputus dengan MQTT Broker
    reconnect();
 }
 
 if(!client.loop()){ //menyambungkan kembali menerima pesan jika terputus dengan MQTT Broker
    client.connect("AangESP8266Client",  mqttuser, mqttpass );
 }


client.publish("dari_alat", String(nilai).c_str(), false); //Publish/kirim pesan ke MQTT broker
delay(200);

}
