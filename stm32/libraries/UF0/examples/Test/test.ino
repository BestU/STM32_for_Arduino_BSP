
//UF0 board test example
#include "touch.h"

int error_flag = 0;
TOUCH touch;

const unsigned char pin[][2] = {
{ 2,  3},
{ 4,  5},
{ 6,  7},
{ 8,  9},
{10, 11},
{12, 13},
{14, 15},
{16, 17},
{18, 19},
{20, 21}
};

void set_pinmode(int out, int in)
{
  pinMode(out, OUTPUT);
  pinMode(in, INPUT);
}

int pin_test(int out, int in)
{
  digitalWrite(out, HIGH);
  if (digitalRead(in) != HIGH) 
  {
    return 1;
  }
  digitalWrite(out, LOW);
  if (digitalRead(in) != LOW)
  {
    return 1;
  }
  return 0;
}

void setup()
{
  int i;

  Serial.begin(9600);  // start serial for output
  pinMode(22, OUTPUT); //configurate LED pin
  digitalWrite(22, LOW);
  error_flag = 0;
  Serial.println("Start GPIO Test!");
  for ( i = 0; i < sizeof(pin) / 2; i++)
  {
    set_pinmode(pin[i][0], pin[i][1]);
    if (pin_test(pin[i][0], pin[i][1]))
    {
       Serial.print("PIN ");
       Serial.print(pin[i][0]);
       Serial.print("  PIN ");
       Serial.print(pin[i][1]);
       Serial.println("Test error!");
       error_flag = 1;
       continue;
    }
    else
    {
       Serial.print("PIN ");
       Serial.print(pin[i][0]);
       Serial.print("  PIN ");
       Serial.print(pin[i][1]);
       Serial.println("Test OK!");
    }
  }
  while (error_flag) {
    delay(100);
	digitalWrite(22, LOW);
	delay(100);
	digitalWrite(22, HIGH);
  }
  touch.begin();
}

void loop()
{
  touch.scan();
	
  if (touch.getstatus(TOUCH_KEY1) == TOUCH_PRESSED) {
    digitalWrite(22, LOW);
  } else if (touch.getstatus(TOUCH_KEY2) == TOUCH_PRESSED) {
    digitalWrite(22, LOW);
  } else if (touch.getstatus(TOUCH_KEY3) == TOUCH_PRESSED) {
    digitalWrite(22, LOW);
  } else {
    digitalWrite(22, HIGH);
  }
  delay(100);
}
