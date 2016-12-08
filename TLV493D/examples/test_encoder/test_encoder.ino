#include "TLV493D.h"


TLV493D senzor(2);


void setup()
{
  Serial.begin(115200); // Begin serial connection for debug.
  senzor.init(HIGH); // Initilize sensor.
}


void loop()
{
  delay(1); // Wait time between reads.
  
  // Read sensor registers and store in buffer.
  senzor.update();

  Serial.print(senzor.m_dBx);
  Serial.print(";");//\t");
  Serial.print(senzor.m_dBy);
  Serial.print(";");//\t");
  Serial.print(senzor.m_dBz);
  Serial.print(";");//\t");
  Serial.println(senzor.m_dTemp);

  Serial.print(senzor.m_dPhi_xy);
  Serial.print(";");//\t");
  Serial.print(senzor.m_dPhi_yz);
  Serial.print(";");//\t");
  Serial.print(senzor.m_dPhi_xz);
  Serial.print(";");//\t");
  Serial.println(senzor.m_dMag_2);
}
