#include <AWS_IOT.h>
#include <WiFi.h>

AWS_IOT iot;

char WIFI_SSID[]="io";
char WIFI_PASSWORD[]="password";
char HOST_ADDRESS[]="a1gf6an104qasm.iot.ap-southeast-2.amazonaws.com";
char CLIENT_ID[]="angus-client";
char TOPIC_NAME[]="angus-topic";

#define LED_BUILTIN 22

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

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

    digitalWrite(LED_BUILTIN, HIGH);
}

void mySubCallBackHandler (char *topicName, int payloadLen, char *payload) {
    // If message is "0", turn off
    // Else if message is "1", turn on
    if (payloadLen > 0) {
      if (payload[0] == '0') {
          digitalWrite(LED_BUILTIN, HIGH);
      } else if (payload[0] == '1') {
          digitalWrite(LED_BUILTIN, LOW);
      }
    }
}

// The ESP32 Arduino environment runs loop() repeatedly, in a FreeRTOS task. We don't need it.
void loop() {
    vTaskDelay(1000 / portTICK_RATE_MS); 
}
