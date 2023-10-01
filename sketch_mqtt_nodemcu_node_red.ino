#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "anuradha";
const char* password = "dse24tr6";

const char* mqtt_server = "91.121.93.94";   //  "test.mosquitto.org" ip address

WiFiClient espClient22;
PubSubClient client(espClient22);

// Lamp - LED - GPIO 4 = D2 on ESP-12E NodeMCU board
const int led = 4;

void setup_wifi() {
  delay(10);

  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - NodeMCU IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageInfo;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageInfo += (char)message[i];
  }
  Serial.println();

  if (topic == "room/light") {
    Serial.print("Changing Room Light to ");
    if (messageInfo == "on") {
      digitalWrite(led, HIGH);
      Serial.print("On");
    } else if (messageInfo == "off") {
      digitalWrite(led, LOW);
      Serial.print("Off");
    }
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("ESP8266Client22")) {
      Serial.println("connected");
      client.subscribe("room/light");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if (!client.loop())
    client.connect("ESP8266Client22");
}
