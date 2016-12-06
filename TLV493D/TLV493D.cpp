/*   Infinenon 3D Magnetic I2c
     TLV493D
     Initial sketch by Mark J. Hughes for AllAboutCircuits.com
     Modified and adapted for the OpenMagneticScanner by Trublion
     jeremy@trublion.org
*/

//--- Begin Includes ---//
#include <I2C.h> // http://dsscircuits.com/articles/arduino-i2c-master-library
#include "math.h"
#include "TLV493D.h"

// Variables
const byte TLV493D::addr = 0x5E; // default address of magnetic sensor 0x5E, 0x3E or 0X1F

TLV493D::TLV493D()
{
  Bx = 0.0;
  By = 0.0;
  Bz = 0.0;
  Temp = 0.0;

  phi_xy = 0.0;
  phi_yz = 0.0;
  phi_xz = 0.0;
  mag_2 = 0.0;

  for (int i = 0; i < 10; i++)
  {
    rbuffer[i] = 0x00;
  }
}

TLV493D::~TLV493D()
{
  deinit();
}

void TLV493D::init()
{
  I2c.begin(); // begin I2c communication
  I2c.timeOut(100);
  I2c.write(addr, 0x00, 0x05);
}

void TLV493D::deinit()
{
  I2c.end();
}

void TLV493D::update()
{
  // read sensor registers and store in rbuffer
  I2c.read(addr, 7);
  
  for (int i = 0; i < 7; i++)
  {
    rbuffer[i] = I2c.receive();
  }

  if (rbuffer[3] & B00000011 != 0)
  {
    // if bits are not 0, TLV is still reading Bx, By, Bz, or T
    Serial.println("TLV493D data read error!");
  }
  else
  {
    // Goto decode functions below
    int x = decodeX(rbuffer[0], rbuffer[4]);
    int y = decodeY(rbuffer[1], rbuffer[4]);
    int z = decodeZ(rbuffer[2], rbuffer[5]);
    int t = decodeT(rbuffer[3], rbuffer[6]);

    #ifdef TLV493D_PRINT_RAW_VALUES
      Serial.print("TLV493D debug:");
      Serial.print("\t");
      Serial.print(x);
      Serial.print("\t");
      Serial.print(y);
      Serial.print("\t");
      Serial.print(z);
      Serial.print("\t");
      Serial.println(t);
    #endif

    Bx = convertToMag(x);
    By = convertToMag(y);
    Bz = convertToMag(z);
    Temp = convertToCelsius(t);

    phi_xy = atan2(Bx, By);
    phi_yz = atan2(By, Bz);
    phi_xz = atan2(Bx, Bz);
    mag_2 = Bx*Bx + By*By + Bz*Bz;
  }
}

//-- Begin Buffer Decode Routines --//
int TLV493D::decodeX(int a, int b)
{
  /* Shift all bits of register 0 to the left 4 positions.  Bit 8 becomes bit 12.  Bits 0:3 shift in as zero.
     Determine which of bits 4:7 of register 4 are high, shift them to the right four places -- remask in case
     they shift in as something other than 0.  bitRead and bitWrite would be a bit more elegant in next version
     of code.
  */
  int ans = ( a << 4 ) | (((b & B11110000) >> 4) & B00001111);

  if ( ans >= 2048)
  {
    ans = ans - 4096;  // Interpret bit 12 as +/-
  }
  return ans;
}

int TLV493D::decodeY(int a, int b)
{
  /* Shift all bits of register 1 to the left 4 positions.  Bit 8 becomes bit 12.  Bits 0-3 shift in as zero.
     Determine which of the first four bits of register 4 are true.  Add to previous answer.
  */

  int ans = (a << 4) | (b & B00001111);
  if ( ans >= 2048)
  {
    ans = ans - 4096; // Interpret bit 12 as +/-
  }
  return ans;
}

int TLV493D::decodeZ(int a, int b)
{
  /* Shift all bits of register 2 to the left 4 positions.  Bit 8 becomes bit 12.  Bits 0-3 are zero.
     Determine which of the first four bits of register 5 are true.  Add to previous answer.
  */
  int ans = (a << 4) | (b & B00001111);
  if ( ans >= 2048)
  {
    ans = ans - 4096;
  }
  return ans;
}

int TLV493D::decodeT(int a, int b)
{
  /* Determine which of the last 4 bits of register 3 are true.  Shift all bits of register 3 to the left
     4 positions.  Bit 8 becomes bit 12.  Bits 0-3 are zero.
     Determine which of the first four bits of register 6 are true.  Add to previous answer.
  */
  int ans;
  a &= B11110000;
  ans = (a << 4) | b;
  if ( ans >= 2048)
  {
    ans -= 4096;
  }
  return ans;
}


double TLV493D::convertToMag(int a)
{
  return a * 0.098;
}

double TLV493D::convertToCelsius(int a)
{
  return (a - 320) * 1.1;
}
