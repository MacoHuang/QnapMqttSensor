#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
const char *serverIP    ="192.168.1.2";
const int serverPort   =21883;
const char *clientId    ="uno3_1583674315";
const char *username    ="559a47010cdf426aa1";
const char *password    ="r:87b27bbcf6460b1129";
const char *tempTopic   ="qiot/things/iot/uno3/temp";
const char *pressTopic  ="qiot/things/iot/uno3/press";
const char *otherTopic  ="qiot/things/iot/uno3/other";
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
EthernetClient ethClient;
PubSubClient client(serverIP, serverPort, ethClient);
unsigned long currentMillis = millis();
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("-----[sensor monitor started!]-----");
  ether_Init();
}

void loop() {
  // Connect to MQTT broker
  if (!client.connected()) {
    reconnectMQTT();
  }
  if (currentMillis - previousMillis >= 6000) {
    previousMillis = currentMillis;
    float tempV = analogRead(A0);
    float pressV = analogRead(A1);
    float otherV = analogRead(A2);
    String dataStr = "Temp: "+String(tempV)+", Press: "+String(pressV)+", Other: "+String(otherV);
    Serial.println(dataStr);
    String tempStr="{\"value\": "+String(tempV)+"}";
    String pressStr="{\"value\": "+String(pressV)+"}";
    String otherStr="{\"value\": "+String(otherV)+"}";
    client.publish(tempTopic, tempStr.c_str());
    client.publish(pressTopic, pressStr.c_str());
    client.publish(otherTopic, otherStr.c_str());
  }
  client.loop();
}

void ether_Init() {
  Serial.println("Initialize ethernet with DHCP: ");
  while (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP failed, 1.5s for try again.");
    delay(1500);
  }
  //print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
/*  for (byte thisByte=0; thisBye<4; thisByte++)
  {
    //print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
*/
}

void reconnectMQTT() {
  while (client.connected()){
  if (client.connect(clientId, username, password)) {
    Serial.println("MQTT client connected!");
  }else {
    Serial.print("MQTT client connection failed, rc=");
    Serial.print(client.state());
    Serial.println();
    delay(1500);
    }
  }
}
