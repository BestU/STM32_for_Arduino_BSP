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

const int pinLight = A0;       //Grove Light Sensor connect to A0
const int pinLed = 7;          //Grove LED connect to D7
const int pinRelay = 8;        //Grove Relay connect to D8

int thresholdvalue = 400;

void setup() {
    // Configure the LED's pin for output signals.
    pinMode(pinLed, OUTPUT);
    // Configure the relay's pin for output signals.
    pinMode(pinRelay, OUTPUT);

}

void loop() {
    // Read the value of the light sensor. The light sensor is an analog sensor.
    int sensorValue = analogRead(pinLight);

    // Turn the LED on if the sensor value is below the threshold.
    if(sensorValue < thresholdvalue)
    {
        digitalWrite(pinLed, HIGH);
        digitalWrite(pinRelay, HIGH);
    }
    else
    {
        digitalWrite(pinLed, LOW);
        digitalWrite(pinRelay, LOW);
    }
}
