#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastReadTime = 0;
const unsigned long readInterval = 2000; // every 2 secs

void setup()
{
  Serial.begin(9600);
  dht.begin();
}

void loop()
{
  unsigned long now = millis();

  if (now - lastReadTime >= readInterval)
  {
    lastReadTime = now;

    float temp = dht.readTemperature();
    float humid = dht.readHumidity();

    Serial.print("[");
    Serial.print(millis());
    Serial.print("ms] ");

    if (!isnan(temp) && !isnan(humid))
    {
      Serial.print("Temperature: ");
      Serial.print(temp);
      Serial.print(" C ");
      Serial.print("Humidity: ");
      Serial.print(humid);
      Serial.println("%");
    }
    else
    {
      Serial.println("Sensor read failed");
    }
  }
}