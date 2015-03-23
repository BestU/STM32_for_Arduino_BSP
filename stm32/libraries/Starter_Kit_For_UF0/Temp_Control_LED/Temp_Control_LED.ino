/*
 Copyright (c) 2015 BestU.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <Wire.h>
#include "rgb_lcd.h"

const int pinTemp = A0;           //Grove Temp Sensor connect to A0
const int pinLed = 7;             //Grove Led connect to D7
const int pinRelay = 8;           //Grove Relay connect to D8

// Define the B-value of the thermistor.
// This value is a property of the thermistor used in the Grove - Temperature Sensor,
// and used to convert from the analog value it measures and a temperature value.
const int B = 3975;

int thresholdMax = 21;
int thresholdMin = 19;

rgb_lcd lcd;

void setup() {
    // Configure the LED's pin for output signals.
    pinMode(pinLed, OUTPUT);
    // Configure the relay's pin for output signals.
    pinMode(pinRelay, OUTPUT);
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("Temp 0 C");
}

void loop() {
    // Get the (raw) value of the temperature sensor.
    int val = analogRead(pinTemp);
    
    val = val * 3300 / 5000;

    // Determine the current resistance of the thermistor based on the sensor value.
    float resistance = (float)(1023-val)*10000/val;

    // Calculate the temperature based on the resistance value.
    float temperature = 1/(log(resistance/10000)/B+1/298.15)-273.15;

    // If the measured sound level is above the threshold, blink the LED..
    if(temperature > thresholdMax)
    {
        digitalWrite(pinLed, HIGH);
        digitalWrite(pinRelay, HIGH);
    }
    if (temperature < thresholdMin) {
        digitalWrite(pinLed, LOW);
        digitalWrite(pinRelay, LOW);
    }
    lcd.setCursor(0, 0);
    lcd.print("Temp ");
    lcd.print(temperature);
    lcd.print(" C");
    delay(1000);
}
