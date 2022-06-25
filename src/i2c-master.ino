#include <Wire.h>

void setup()
{
    Wire.begin();
}

void loop()
{
    Wire.beginTransmission(7);
    Wire.write("test\n");
    Wire.endTransmission();
    delay(100)
}