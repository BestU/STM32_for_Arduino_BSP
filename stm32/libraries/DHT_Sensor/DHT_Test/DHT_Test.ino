
#include "dht.h"

#define DHTPIN   A0     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22

DHT dht;

void setup()
{
  dht.begin(DHTPIN, DHTTYPE);
  Serial.begin(9600);
}

void loop()
{
  boolean status;
  
  status = dht.read();
  
  if (status == true) {
    Serial.print("Humidity: "); 
    Serial.print(dht.humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(dht.temperature);
    Serial.println(" *C");
    
  } else {
    Serial.println("Failed to read from DHT");
  }
  delay(1000);
}