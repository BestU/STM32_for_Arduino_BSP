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
 
const int pinSound = A0;       //Grove Sound connect to A0
const int pinLed = 7;          //Grove Led connect to D7
const int pinRelay = 8;        //Grove Relay connect to D8

int thresholdvalue = 800;

void setup() {
    // Configure the LED's pin for output signals.
    pinMode(pinLed, OUTPUT);
    // Configure the relay's pin for output signals.
    pinMode(pinRelay, OUTPUT);
}

void loop() {
    // Read the value of the Sound sensor. The light sensor is an analog sensor.
    int sensorValue = analogRead(pinSound);

    // If the measured sound level is above the threshold, blink the LED..
    if(sensorValue > thresholdvalue)
    {
        digitalWrite(pinLed, HIGH);
        digitalWrite(pinRelay, HIGH);
        delay(1000);
        digitalWrite(pinLed, LOW);
        digitalWrite(pinRelay, LOW);
    }
}
