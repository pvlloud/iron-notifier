#include <Arduino.h>
#include "ESP8266.h"

#define SSID        "ssid"
#define PASSWORD    "password"
#define HOST_NAME   "46.101.95.228"
#define HOST_PORT   (80)

#define GREEN_LED 5
#define RED_LED 6

int lastStatus = '0';

ESP8266 wifi(Serial);

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);
  wifi.setOprToStationSoftAP();
  wifi.joinAP(SSID, PASSWORD);
  wifi.disableMUX();
  delay(1000);
}

void blink_pin(int pin, int blinkDelay, int blinks) {
  for (int i = 0; i <= blinks; ++i){
    digitalWrite(pin, LOW);
    delay(blinkDelay);
    digitalWrite(pin, HIGH);
    delay(blinkDelay);
    digitalWrite(pin, LOW);
  }
}

void shut_leds() {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void red_status() {
  shut_leds();
  blink_pin(RED_LED, 70, 10);
  digitalWrite(RED_LED, HIGH);
}

void green_status() {
  shut_leds();
  blink_pin(GREEN_LED, 200, 3);
  digitalWrite(GREEN_LED, HIGH);
}

void yellow_status() {
  shut_leds();
  for (int i = 0; i <= 1; ++i){
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    delay(50);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    delay(50);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
  }
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
}

int filter_input(int input) {
  if(input == '0') {
    return '0';    
  }
  if(input == '1') {
    return '1';    
  }
  if(input == '2') {
    return '2';    
  }
  return lastStatus;
}

void loop() {
  int newStatus = filter_input(Serial.read());
  uint8_t buffer[1024] = {0};
  wifi.createTCP(HOST_NAME, HOST_PORT);
  char *hello = "GET /mock HTTP/1.1\r\nHost: 46.101.95.228\r\nConnection: close\r\n\r\n";
  wifi.send((const uint8_t*)hello, strlen(hello));
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
  if (len > 0) {
    newStatus = (char)buffer[len - 1];
    Serial.println(newStatus);
    if (newStatus != lastStatus) {
      lastStatus = newStatus;
      switch(lastStatus){
        case '0':
          green_status();
        break;
        case '1':
          yellow_status();
        break;
        case '2':
          red_status();
        break;
      }
    }
  }
  wifi.releaseTCP();
  delay(7000);
}
