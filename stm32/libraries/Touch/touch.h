
#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "variant.h"


#define THRESHOLD 50

typedef enum 
{
	TOUCH_RELEASE,
	TOUCH_PRESSED
}TouchStatus;

typedef enum
{
	TOUCH_KEY1,
	TOUCH_KEY2,
	TOUCH_KEY3
}ToutchKey;

typedef struct 
{
	short fixed_value;
	short current_value;
	TouchStatus status;
	unsigned short enable_mask;
}PortStatus;
typedef struct 
{
	PortStatus key[3];
	char index;
}ScanTrace;





class TOUCH 
{
public:
	void begin(void);
	void scan(void);
	TouchStatus getstatus(ToutchKey key);
private:
	void scan_channel(int index);
	int wait_eoc(void);
	
	ScanTrace _trace;
};



#endif /* __TOUCH_H__ */
