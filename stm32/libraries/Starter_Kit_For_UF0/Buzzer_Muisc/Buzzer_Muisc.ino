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

#include "pitches.h"
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;
int speakerPin = 8;                  // Grove Buzzer connect to D8
 
void setup() {
  // iterate over the notes of the melody:
  pinMode(speakerPin, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Happy birthday");
  lcd.setCursor(0, 1);
  lcd.print("to you !");
}
 
void loop() {
  play();//Play the music.
  delay(300);//Pause for a while.
}
 
// notes in the melody:
int melody[] = {
NOTE_G4,//5  
NOTE_G4,//5
NOTE_A4,//6
NOTE_G4,//5
NOTE_C5,//1.
NOTE_B4,//7
0,
NOTE_G4,//5
NOTE_G4,//5
NOTE_A4,//6
NOTE_G4,//5
NOTE_D5,//2.
NOTE_C5,//1.
0,
NOTE_G4,//5
NOTE_G4,//5
NOTE_G5,//5.
NOTE_E5,//3.
NOTE_C5,//1.
NOTE_B4,//7
NOTE_A4,//6
0,
NOTE_F5,//4.
NOTE_F5,//4.
NOTE_E5,//3.
NOTE_C5,//1.
NOTE_D5,//2.
NOTE_C5,//1.
0,
};
 
 
int noteDurations[] = {
  8,8,4,4,4,4,
  4,
  8,8,4,4,4,4,
  4,
  8,8,4,4,4,4,2,
  8,
  8,8,4,4,4,2,
  4,
};
 
void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tone);
    }
}
 
void play()
{
    for (int thisNote = 0; thisNote < 29; thisNote++) {
 
    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    Serial.println(melody[thisNote]);
    if (melody[thisNote])
    {
      playTone(1000000/melody[thisNote],noteDuration);
    }
    else
    {
      delay(noteDuration);
    }
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 0.30;
    delay(pauseBetweenNotes);    
  }
}
 
