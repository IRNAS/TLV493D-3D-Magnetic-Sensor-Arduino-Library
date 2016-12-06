#include "TLV493D.h"

// Variable Declaration
byte delaytime = 1; // time to wait before next read

TLV493D senzor;

//--- Begin Setup ---//
void setup()
{
  Serial.begin(115200); // Begin serial connection for debug.
  senzor.init();
}
//--- End of Setup --//


//--- Begin Main Program Loop --//
void loop()
{
  delay(delaytime); // wait time between reads.
  // Read sensor registers and store in rbuffer
  senzor.update();

  Serial.print(senzor.Bx);
  Serial.print(";");//\t");
  Serial.print(senzor.By);
  Serial.print(";");//\t");
  Serial.print(senzor.Bz);
  Serial.print(";");//\t");
  Serial.println(senzor.Temp);

  Serial.print(senzor.phi_xy);
  Serial.print(";");//\t");
  Serial.print(senzor.phi_yz);
  Serial.print(";");//\t");
  Serial.print(senzor.phi_xz);
  Serial.print(";");//\t");
  Serial.println(senzor.mag_2);
}
//-- End of Main Program Loop --//
