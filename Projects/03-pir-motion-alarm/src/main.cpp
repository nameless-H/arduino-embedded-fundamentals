#include <Arduino.h>

enum AlarmState
{
  DISARMED,
  ARMING,
  ARMED,
  TRIGGERED,
  ALARM,
  COOLDOWN
};

AlarmState currentState = DISARMED;
AlarmState lastState = DISARMED;

const int ledPin = 9;
const int buttonPin = 3;
const int buzzerPin = 8;
const int pirPin = 2;

const unsigned long longPressThreshold = 1500; // ms
const unsigned long debounceDelay = 50;        // ms

int buttonState = HIGH;

unsigned long stateEntryTime = 0;

const unsigned long armingTime = 10000;   // ms
const unsigned long graceTime = 5000;     // ms
const unsigned long cooldownTime = 30000; // ms

void handleButton();
void readButton();
void readPirSensor();
void updateLED();
void updateBuzzer();
void updateFSM();

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(pirPin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  readButton();
  handleButton();
  updateFSM();
  updateLED();
  updateBuzzer();
}

void readButton()
{
  static int lastButtonReading = HIGH;
  static unsigned long lastDebounceTime = 0;

  int reading = digitalRead(buttonPin);

  if (reading != lastButtonReading)
  {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime >= debounceDelay)
  {
    buttonState = reading; // stable — trust it
  }

  lastButtonReading = reading;
}

void handleButton()
{
  static int prevButtonState = HIGH;
  static unsigned long pressTime = 0;
  static bool longPressHandled = false;

  if (buttonState == LOW && prevButtonState == HIGH)
  {
    pressTime = millis();
    longPressHandled = false;
  }

  unsigned long pressDuration = millis() - pressTime;

  if (buttonState == LOW && !longPressHandled && pressDuration >= longPressThreshold)
  {
    currentState = (currentState == DISARMED) ? ARMING : DISARMED;
    longPressHandled = true;
  }

  if (buttonState == HIGH && prevButtonState == LOW && !longPressHandled)
  {
    if (currentState == TRIGGERED || currentState == ALARM)
      currentState = COOLDOWN;
  }

  prevButtonState = buttonState;
}

void readPirSensor()
{
  static int lastPirState = LOW;
  int currentPirState = digitalRead(pirPin);

  if (currentPirState == HIGH && lastPirState == LOW)
  {
    currentState = TRIGGERED;
  }

  lastPirState = currentPirState;
}

void updateLED()
{
  static unsigned long lastToggle = 0;
  static bool ledOn = false;

  unsigned long onTime = 0;
  unsigned long offTime = 0;
  bool useAnalog = false;

  switch (currentState)
  {
  case DISARMED:
    onTime = 0;
    offTime = 0;
    break;
  case ARMING:
    onTime = 100;
    offTime = 100;
    break;
  case ARMED:
    onTime = 100;
    offTime = 1500;
    break;
  case TRIGGERED:
    onTime = 50;
    offTime = 50;
    break;
  case ALARM:
    onTime = 1;
    offTime = 0;
    break;
  case COOLDOWN:
    useAnalog = true;
    break;
  }

  if (useAnalog)
  {
    unsigned long elapsed = millis() - stateEntryTime;
    int brightness = map(elapsed, 0, cooldownTime, 255, 0);
    analogWrite(ledPin, constrain(brightness, 0, 255));
    return;
  }

  if (onTime == 0 && offTime == 0)
  {
    digitalWrite(ledPin, LOW);
    return;
  }

  unsigned long interval = ledOn ? onTime : offTime;
  if (millis() - lastToggle >= interval)
  {
    lastToggle = millis();
    ledOn = !ledOn;
    digitalWrite(ledPin, ledOn);
  }
}

void updateBuzzer()
{
  static unsigned long lastToggle = 0;
  static bool buzzerOn = false;

  if (currentState != ALARM)
  {
    noTone(buzzerPin);
    buzzerOn = false;
    return;
  }

  if (millis() - lastToggle >= 100)
  {
    lastToggle = millis();
    buzzerOn = !buzzerOn;
    buzzerOn ? tone(buzzerPin, 1000) : noTone(buzzerPin);
  }
}

void printState(AlarmState state) {
    switch(state) {
        case DISARMED:  Serial.print("DISARMED");  break;
        case ARMING:    Serial.print("ARMING");    break;
        case ARMED:     Serial.print("ARMED");     break;
        case TRIGGERED: Serial.print("TRIGGERED"); break;
        case ALARM:     Serial.print("ALARM");     break;
        case COOLDOWN:  Serial.print("COOLDOWN");  break;
    }
}

void updateFSM()
{

  if (currentState != lastState)
  {
    stateEntryTime = millis();
    printState(lastState);
    Serial.print(" \u2192 ");
    printState(currentState);
    Serial.println("");
    lastState = currentState;
  }

  switch (currentState)
  {
  case DISARMED:
    break;
  case ARMING:
    if (millis() - stateEntryTime >= armingTime)
    {
      currentState = ARMED;
    }
    break;
  case ARMED:
    readPirSensor();
    break;
  case TRIGGERED:
    if (millis() - stateEntryTime >= graceTime)
    {
      currentState = ALARM;
    }
    break;
  case ALARM:
    break;
  case COOLDOWN:
    if (millis() - stateEntryTime >= cooldownTime)
    {
      currentState = ARMED;
    }
    break;
  }
}