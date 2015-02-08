
#include <touch.h>

TOUCH touch;
int delaylegnth = 30;
int stop = 0;

void setup() {  
  //establish motor direction toggle pins
  pinMode(12, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(13, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  
  //establish motor brake pins
  pinMode(9, OUTPUT); //brake (disable) CH A
  pinMode(8, OUTPUT); //brake (disable) CH B

  touch.begin();  
}

void loop(){
  touch.scan();
  if (touch.getstatus(TOUCH_KEY1) == TOUCH_PRESSED) {
    stop = 0;
    if (delaylegnth > 35) delaylegnth -= 5;
  }
  if (touch.getstatus(TOUCH_KEY2) == TOUCH_PRESSED) {
    stop = 1;
  }
  if (touch.getstatus(TOUCH_KEY3) == TOUCH_PRESSED) {
    stop = 0;
    if (delaylegnth < 100) delaylegnth += 5;    
  }
  if (stop == 0) {
    moto();
  } 
}

void moto(void)
{
  digitalWrite(9, LOW);  //ENABLE CH A
  digitalWrite(8, HIGH); //DISABLE CH B

  digitalWrite(12, HIGH);   //Sets direction of CH A
  analogWrite(3, 255);   //Moves CH A
  
  delay(delaylegnth);
  
  digitalWrite(9, HIGH);  //DISABLE CH A
  digitalWrite(8, LOW); //ENABLE CH B

  digitalWrite(13, LOW);   //Sets direction of CH B
  analogWrite(11, 255);   //Moves CH B
  
  delay(delaylegnth);
  
  digitalWrite(9, LOW);  //ENABLE CH A
  digitalWrite(8, HIGH); //DISABLE CH B

  digitalWrite(12, LOW);   //Sets direction of CH A
  analogWrite(3, 255);   //Moves CH A
  
  delay(delaylegnth);
    
  digitalWrite(9, HIGH);  //DISABLE CH A
  digitalWrite(8, LOW); //ENABLE CH B

  digitalWrite(13, HIGH);   //Sets direction of CH B
  analogWrite(11, 255);   //Moves CH B
  
  delay(delaylegnth);  
}

