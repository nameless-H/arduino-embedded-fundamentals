#include <Arduino.h>

enum LedState { LED_OFF, LED_ON };
LedState currentState = LED_OFF;

unsigned long lastTransitionTime = 0;
const unsigned long blinkInterval = 1000; // ms
const int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long now = millis();

  if (now - lastTransitionTime >= blinkInterval) {
    lastTransitionTime = now;

    switch (currentState) {
      case LED_OFF:
        currentState = LED_ON;
        digitalWrite(ledPin, HIGH);
        break;
      case LED_ON:
        currentState = LED_OFF;
        digitalWrite(ledPin, LOW);
        break;
    }
  }
}