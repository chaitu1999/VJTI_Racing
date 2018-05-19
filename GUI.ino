#include <Azande.h>
#include <SimpleDHT.h>
define_text_event(eventSensorState1, "Hall Sensor State", , 64);
define_int_event(eventRPM, "RPM", 8, "rpm", , , );
define_text_event(eventSensorState2, "Sensor State", , 64);
define_int_event(eventHumidity, "Humidity", 8, "%", , , );
define_int_event(eventTemperature, "Temperature", 10, "C", , , );
Azande azande;
int pinDHT22 = 2;
SimpleDHT22 dht22;
unsigned char loopCounter_1;      
unsigned char readErrorCounter_1;
unsigned char loopCounter_2;      
unsigned char readErrorCounter_2;
volatile byte half_revolutions_1;
unsigned int rpm_1;
unsigned long timeold_1;

void setup() 
{
  // Setup variables
  loopCounter_1 = 0;
  readErrorCounter_1 = 0;
  loopCounter_2 = 0;
  readErrorCounter_2 = 0;
  Serial.begin(19200);
  attachInterrupt(digitalPinToInterrupt(3), magnet_detect_1, RISING);
  azande.begin();
  azande.add(eventSensorState1);
  azande.add(eventRPM);
   azande.add(eventSensorState2);
  azande.add(eventHumidity);
  azande.add(eventTemperature);
  
}

void rpm_sensor_1() 
{
  loopCounter_1++;
  if ((loopCounter_1 > 12) || (readErrorCounter_1 > 0)) // Execute every 12 x 100ms, or if Read Error do a retry.
  {
    loopCounter_1 = 0;
    
    if (half_revolutions_1 < 0) 
    {
      // Failed to read from sensor.
      readErrorCounter_1++;

      if (readErrorCounter_1 > 5) // Failed to read 5 times.
      {
        readErrorCounter_1 = 0;
        azande.send(eventSensorState1, "Read Error");
      }
    }
    else // Read Success
    {
      readErrorCounter_1 = 0;
      azande.send(eventSensorState1, "OK");
      rpm_1 = 30*1000/(millis() - timeold_1)*half_revolutions_1;
      timeold_1 = millis();
      half_revolutions_1 = 0;
      azande.send(eventRPM, rpm_1);
    }
  }
  
  delay(10);
  azande.readStream(); // Let Azande handle the new data.
}
void magnet_detect_1()
 {
   half_revolutions_1++;
 }

 void temp_sensor_1() 
{
  byte temperature = 0;
  byte humidity = 0;

  loopCounter_2++;
  if ((loopCounter_2 > 12) || (readErrorCounter_2 > 0)) // Execute every 12 x 100ms, or if Read Error do a retry.
  {
    loopCounter_2 = 0;
    
    if (dht22.read(pinDHT22, &temperature, &humidity, NULL) != SimpleDHTErrSuccess) 
    {
      // Failed to read from sensor.
      readErrorCounter_2++;

      if (readErrorCounter_2 > 10) // Failed to read 10 times.
      {
        readErrorCounter_2 = 0;
        azande.send(eventSensorState2, "Read Error");
      }
    }
    else // Read Success
    {
      readErrorCounter_2 = 0;
      azande.send(eventSensorState2, "OK");
      azande.send(eventHumidity, humidity);
      azande.send(eventTemperature, temperature);
    }
  }
  
  delay(10);
  azande.readStream(); // Let Azande handle the new data.
}

void loop()
{
  temp_sensor_1();
  rpm_sensor_1();
}


