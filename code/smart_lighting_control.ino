#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-Fi details
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT broker
const char* mqtt_server = "YOUR_MQTT_BROKER_IP";
const int mqtt_port = 1883;

// MQTT topic
const char* topic_light = "topic/light";

// Potentiometer pin
const int potPin = A0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect_mqtt() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      // Connected
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }

  client.loop();

  int potValue = analogRead(potPin);        // 0 to 1023
  int brightness = map(potValue, 0, 1023, 0, 100);

  char message[10];
  sprintf(message, "%d", brightness);

  client.publish(topic_light, message);

  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print(" | Brightness: ");
  Serial.println(brightness);

  delay(1000);
}
