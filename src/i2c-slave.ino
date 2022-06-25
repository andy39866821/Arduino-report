#include <Wire.h>
void setup()
{
    Wire.begin(7);
    Wire.onReceive(receiveEvent);
    Serial.begin(9600);
}

void loop()
{
    delay(100);
}

void receiveEvent(int numBytes)
{
    while (Wire.available())
    {
        char ch = Wire.read();
        Serial.print(ch);
    }
}