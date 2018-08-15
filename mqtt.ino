#include <AWS_IOT.h>
#include <WiFi.h>

AWS_IOT iot;

char WIFI_SSID[]="io";
char WIFI_PASSWORD[]="password";
char HOST_ADDRESS[]="a1gf6an104qasm.iot.ap-southeast-2.amazonaws.com";
char CLIENT_ID[]="angus-client";
char TOPIC_NAME[]="angus-topic";

void setup() {
    Serial.begin(115200);
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected to WiFi");

    if(iot.connect(HOST_ADDRESS, CLIENT_ID) == 0) {
        Serial.println("Connected to AWS");
        delay(1000); // avoid MQTT_CLIENT_NOT_IDLE_ERROR when subscribing
        if(iot.subscribe(TOPIC_NAME, mySubCallBackHandler) == 0) {
            Serial.println("Subscribe Successful");
        } else {
            Serial.println("iot.subscribe failed");
            while(1);
        }
    } else {
        Serial.println("iot.connect failed");
        while(1);
    }

}

int tick=0, msgCount=0, msgReceived = 0;
char payload[512];
char rcvdPayload[512];

void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}

void loop() {
    if(msgReceived == 1) {
        msgReceived = 0;
        Serial.print("Received Message:");
        Serial.println(rcvdPayload);
    }
    vTaskDelay(1000 / portTICK_RATE_MS); 
}
