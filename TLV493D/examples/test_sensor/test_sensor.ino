#include "TLV493D.h"


/*! \var TLV493D sensor
	\brief Sensor instance. Sensor powered from Arduino pin 2.
*/
TLV493D sensor(2);


/*! \fn void setup()
	\brief Arduino setup function.
	\return Nothing.
*/
void setup()
{
  Serial.begin(115200); // Begin serial connection for debug.
  sensor.init(HIGH); // Initilize sensor.
}


/*! \fn void loop()
	\brief Arduino loop function.
	\return Nothing.
*/
void loop()
{
  delay(1); // Wait time between reads.
  
  // Read sensor registers and store in buffer.
  sensor.update();

  // Dump all measured data.
  
  Serial.print(sensor.m_dBx); // magnetic field X component
  Serial.print(";");//\t");
  Serial.print(sensor.m_dBy); // magnetic field Y component
  Serial.print(";");//\t");
  Serial.print(sensor.m_dBz); // magnetic field Z component
  Serial.print(";");//\t");
  Serial.println(sensor.m_dTemp); // temperature of the sensor

  Serial.print(sensor.m_dPhi_xy); // angle in radians between magnetic field X and Y components
  Serial.print(";");//\t");
  Serial.print(sensor.m_dPhi_yz); // angle in radians between magnetic field Y and Z components
  Serial.print(";");//\t");
  Serial.print(sensor.m_dPhi_xz); // angle in radians between magnetic field X and Z components
  Serial.print(";");//\t");
  Serial.println(sensor.m_dMag_2); // square of the value of magnetic field flux density
}
