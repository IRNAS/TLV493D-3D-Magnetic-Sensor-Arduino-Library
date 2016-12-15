/*   Infinenon 3D Magnetic I2C
     TLV493D
     by Mark J. Hughes
     for AllAboutCircuits.com
     20160817
*/

#include "TLV493D.hpp"
#include "Arduino.h"
#include "Wire.h"


/*! \fn double TLV493D::atan2_remaped(double x, double y)
	\brief Returns true angle in radians between 0 and 2PI.
    \param x X coordinate.
	\param y Y coordinate.
	\return Angle in radians between 0 and 2PI.
*/
double TLV493D::atan2_remaped(double x, double y)
{
  //if((x == 0.0) && (y == 0.0)) Serial.println("Error in atan2 function. Division by zero.");

  if ((x == 0.0) && (y == 0.0)) return 0.0;

  else if ((x > 0.0) && (y == 0.0)) return 0.0;

  else if ((x > 0.0) && (y > 0.0)) return atan(y / x);

  else if ((x == 0.0) && (y > 0.0)) return M_PI_2;

  else if ((x < 0.0) && (y > 0.0)) return M_PI + atan(y / x);

  else if ((x < 0.0) && (y == 0.0)) return M_PI;

  else if ((x < 0.0) && (y < 0.0)) return M_PI + atan(y / x);

  else if ((x == 0.0) && (y < 0.0)) return 3.0 * M_PI_2;

  else if ((x > 0.0) && (y < 0.0)) return 2.0 * M_PI + atan(y / x);
}


uint8_t TLV493D::init(int pwrPinLevel)
{
  /* Read all registers, although only interested in configuration data
     stored in rbuffers 7,8,9, as 0-6 might be empty or invalid at the moment.
  */

  addr = (pwrPinLevel == HIGH) ? addr1 : addr2;

  uint8_t n_bytes_returned = Wire.requestFrom(addr, sizeof(rbuffer));
  //Serial.print("Bytes returned: ");
  //Serial.println(n_bytes_returned);
  if(n_bytes_returned != sizeof(rbuffer)) return 0x05;

  for (int i = 0; i < sizeof(rbuffer); i++)
  {
    rbuffer[i] = Wire.read();
  }

  // Write Register 0H is non configurable.  Set all bits to 0
  wbuffer[0] = B00000000;

  // Read Register 7H 6:3 -> Write Register 1H 6:3
  wbuffer[1] = rbuffer[7] & B01111000;

  // Read Register 8H 7:0 -> Write Register 2H 7:0
  wbuffer[2] = rbuffer[8];

  // Read Register 9H 4:0 -> Write Register 3H 4:0 (Mod2)
  wbuffer[3] = rbuffer[9] & B00001111;

  // Set Power Mode (ulpm, lpm, fm, pd)
  for (int i = 0; i < sizeof(wbuffer); i++)
  {
    wbuffer[i] |= lpm[i];
  }

  Wire.beginTransmission(addr);

  uint8_t n_bytes_written;
  for (int i = 0; i < sizeof(wbuffer); i++)
  {
    n_bytes_written = Wire.write(wbuffer[i]);
    //Serial.print("Bytes written: ");
    //Serial.println(n_bytes_written);
    if(n_bytes_written != 1) return 0x06;
  }

  uint8_t write_status = Wire.endTransmission();
  //Serial.print("Write status: 0x");
  //Serial.println(write_status, HEX);

  return write_status;
}


uint8_t TLV493D::update()
{
  // Read sensor registers and store in rbuffer
  uint8_t n_bytes_returned = Wire.requestFrom(addr, sizeof(rbuffer));
  //Serial.print("Bytes returned: ");
  //Serial.println(n_bytes_returned);

  for (int i = 0; i < sizeof(rbuffer); i++)
  {
    rbuffer[i] = Wire.read();
  }

  // Goto decode functions below
  x = decodeX(rbuffer[0], rbuffer[4]);
  y = decodeY(rbuffer[1], rbuffer[4]);
  z = decodeZ(rbuffer[2], rbuffer[5]);
  t = decodeT(rbuffer[3], rbuffer[6]);

  if (rbuffer[3] & B00000011 != 0)
  {
    // If bits are not 0, TLV is still reading Bx, By, Bz, or T
    return 0x01;
  }
  else
  {
    return 0x00;
  }
}





/*! \fn int TLV493D::decodeX(const int a, const int b)
	\brief Decode Bx.
    \param a Register value 1.
	\param b Register value 2.
	\return Decoded register value.
*/
int TLV493D::decodeX(const int a, const int b)
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


/*! \fn int TLV493D::decodeY(const int a, const int b)
	\brief Decode By.
    \param a Register value 1.
	\param b Register value 2.
	\return Decoded register value.
*/
int TLV493D::decodeY(const int a, const int b)
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


/*! \fn int TLV493D::decodeZ(const int a, const int b)
	\brief Decode Bz.
    \param a Register value 1.
	\param b Register value 2.
	\return Decoded register value.
*/
int TLV493D::decodeZ(const int a, const int b)
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


/*! \fn int TLV493D::decodeT(const int a, const int b)
	\brief Decode temperature.
    \param a Register value 1.
	\param b Register value 2.
	\return Decoded register value.
*/
int TLV493D::decodeT(int a, const int b)
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


/*! \fn double TLV493D::convertToMag(const int a)
	\brief Used for calculating size of each magnetic field components.
    \param a Decoded register value.
	\return Calculated magnetic field coordinate.
*/
double TLV493D::convertToMag(const int a)
{
  return a * 0.098;
}


/*! \fn double TLV493D::convertToCelsius(const int a)
	\brief Calculate temperature in degrees Celsius.
    \param a Decoded register value.
	\return Calculated temperature value.
*/
double TLV493D::convertToCelsius(const int a)
{
  return (a - 320) * 1.1;
}
