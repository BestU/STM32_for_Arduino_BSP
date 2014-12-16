#ifndef dht_h  
#define dht_h  
  
#if ARDUINO < 100  
#include <WProgram.h>  
#else  
#include <Arduino.h>  
#endif  
  
#define DHT11 11
#define DHT22 22
  
class DHT  
{  
public:
	void begin(uint8_t pin, uint8_t type);
	double humidity;  
    double temperature; 
    boolean read(void);

private:
    int read11(uint8_t pin);  
    int read22(uint8_t pin);    
    uint8_t _pin, _type;  
    uint8_t bits[5];  // buffer to receive data  
    int read(uint8_t pin);  
};  
#endif 
