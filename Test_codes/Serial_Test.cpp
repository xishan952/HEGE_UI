#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    Serial.println("HEGE Control Panel Started");
}

void loop()
{
    Serial.println("Running...");
    delay(1000);
}