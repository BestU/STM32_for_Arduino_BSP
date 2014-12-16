

// inslude the TOUCH library:
#include <touch.h>


TOUCH touch;
void setup() {
	touch.begin();
	Serial.begin(9600);
}

void loop() {
	touch.scan();
	
	if (touch.getstatus(TOUCH_KEY1) == TOUCH_PRESSED) {
		Serial.println("Touch KEY1 Pressed !");
	}
	if (touch.getstatus(TOUCH_KEY2) == TOUCH_PRESSED) {
		Serial.println("Touch KEY2 Pressed !");
	}
	if (touch.getstatus(TOUCH_KEY2) == TOUCH_PRESSED) {
		Serial.println("Touch KEY2 Pressed !");
	}
	

}


