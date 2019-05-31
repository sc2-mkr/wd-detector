#include "ArduinoJson.h"
#include "WdAlertMessage.hpp"

WdAlertMessagePublisher::WdAlertMessagePublisher(PubSubClient mqttClient) : _mqttClient(mqttClient)
{
}

void WdAlertMessagePublisher::publishAlertMessage(WdAlertMessage message)
{
    StaticJsonDocument<256> doc;
    char buffer[512];
    doc["boardId"] = message.boardId;
    doc["speed"] = message.speed;
    size_t n = serializeJson(doc, buffer);
    _mqttClient.publish(TOPIC, buffer, n);
}

WdAlertMessage parse_message(byte *payload, unsigned int length)
{
    WdAlertMessage message;
    return message;
}