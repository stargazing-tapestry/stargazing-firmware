#include <AWS_IOT.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 50
#define LED_STRIP_PIN 19

AWS_IOT iot;

char WIFI_SSID[]="io";
char WIFI_PASSWORD[]="password";
char HOST_ADDRESS[]="a1gf6an104qasm.iot.ap-southeast-2.amazonaws.com";
char CLIENT_ID[]="angus-client";
char TOPIC_NAME[]="angus-topic";

#define LED_BUILTIN 22

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    strip.begin();
    strip.show();

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

int16_t frame_count[NUM_LEDS];
uint32_t color[NUM_LEDS];

void mySubCallBackHandler (char *topicName, int payloadLen, char *payload) {
  for (int16_t i=0; i<payloadLen; i+=5) {
    int16_t led = ((uint8_t)payload[i+0] << 8) + (uint8_t)payload[i+1];
    color[led] = strip.Color(payload[i+2], payload[i+3], payload[i+4]);
    frame_count[led] = 40;
  }
}

// The ESP32 Arduino environment runs loop() repeatedly, in a FreeRTOS task.
void loop() {
  for (int16_t i=0; i<NUM_LEDS; i++) {
    if (frame_count[i] > 0) frame_count[i]--;
    if (frame_count[i] > 0) {
      strip.setPixelColor(i, color[i]);
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
  }
  portDISABLE_INTERRUPTS();
  strip.show();
  portENABLE_INTERRUPTS();
  delay(50);
}
