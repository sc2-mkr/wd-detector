#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "WdAlertMessage.hpp"

#define CLIENT_ID "wd-detector-0"
const char *wifi_ssid = "DESKTOP-CV4ODF2 8098"; //SSID of your Wi-Fi router
const char *wifi_password = "0580R7l:";
IPAddress mqtt_server(192, 168, 137, 1);
uint16_t mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
WdAlertMessagePublisher wdAlertMessagePublisher(client);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(CLIENT_ID))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void onMessageReceived(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1')
  {
    digitalWrite(BUILTIN_LED, LOW); // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  }
  else
  {
    digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off by making the voltage HIGH
  }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(onMessageReceived);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  WdAlertMessage message;
  message.speed = 50;
  message.boardId = 10;
  wdAlertMessagePublisher.publishAlertMessage(message);
  // // Once connected, publish an announcement...
  // client.publish("outTopic", "hello world");
  // // ... and resubscribe
  // client.subscribe("inTopic");
  // long now = millis();
  // if (now - lastMsg > 2000)
  // {
  //   lastMsg = now;
  //   ++value;
  //   snprintf(msg, 50, "hello world #%ld", value);
  //   Serial.print("Publish message: ");
  //   Serial.println(msg);
  //   client.publish("outTopic", msg);
}