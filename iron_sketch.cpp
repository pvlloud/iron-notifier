#include <Arduino.h>

#define GREEN_LED 5
#define RED_LED 6

int lastStatus = '0';

void setup() {
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    Serial.begin(9600);
    digitalWrite(GREEN_LED, HIGH);
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
