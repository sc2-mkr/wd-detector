#define TOPIC "wd-detector/alert"
#include "PubSubClient.h"

struct WdAlertMessage
{
    long boardId;
    double speed;
};

class WdAlertMessagePublisher
{
public:
    WdAlertMessagePublisher(PubSubClient mqtt_client);
    void publishAlertMessage(WdAlertMessage message);

private:
    PubSubClient _mqttClient;
};

WdAlertMessage
parse_message(byte *payload, unsigned int length);