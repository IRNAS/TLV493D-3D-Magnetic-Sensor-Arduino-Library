#include <I2C.h> // http://dsscircuits.com/articles/arduino-i2c-master-library
#include "math.h"
#include "TLV493D.h"

const byte TLV493D::m_bAddr1 = 0x5E; // address1
const byte TLV493D::m_bAddr2 = 0x1F; // address2

// returns true angle in radians between 0 and 2PI
double TLV493D::atan2_remaped(double x, double y)
{
	//if((x == 0.0) && (y == 0.0)) Serial.println("Error in atan2 function. Division by zero.");

	if((x == 0.0) && (y == 0.0)) return 0.0;
	
	else if((x > 0.0) && (y == 0.0)) return 0.0;

	else if((x > 0.0) && (y > 0.0)) return atan(y / x);

	else if((x == 0.0) && (y > 0.0)) return M_PI_2;

	else if((x < 0.0) && (y > 0.0)) return M_PI + atan(y / x);

	else if((x < 0.0) && (y == 0.0)) return M_PI;

	else if((x < 0.0) && (y < 0.0)) return M_PI + atan(y / x);

	else if((x == 0.0) && (y < 0.0)) return 3.0 * M_PI_2;

	else if((x > 0.0) && (y < 0.0)) return 2.0 * M_PI + atan(y / x);
}

TLV493D::TLV493D(const int pwrPin) :
  m_iPwrPin(pwrPin),
  m_bAddr(0),
  m_dBx(0.0),
  m_dBy(0.0),
  m_dBz(0.0),
  m_dTemp(0.0),
  m_dPhi_xy(0.0),
  m_dPhi_yz(0.0),
  m_dPhi_xz(0.0),
  m_dMag_2(0.0)
{
  // clear read buffer
  for (int i = 0; i < 10; i++)
  {
    m_aBuffer[i] = 0x00;
  }

  // initalize power pin
  pinMode(m_iPwrPin, OUTPUT);
  // power down the sensor
  digitalWrite(m_iPwrPin, LOW);
}

TLV493D::~TLV493D()
{
  // power down the sensor
  deinit();
}

void TLV493D::init(const int dataPinState)
{
  // setup data pin (A4 = I2C DATA)
  pinMode(A4, OUTPUT);
  // voltage on data pin determins sensor address at power up
  digitalWrite(A4, dataPinState);

  // power on the sensor
  digitalWrite(m_iPwrPin, HIGH);
  // wait a little so that the sensor gets the right address
  delay(1);
  
  I2c.begin(); // begin I2c communication
  I2c.timeOut(100);
  // choose the right address
  m_bAddr = (dataPinState == HIGH) ? m_bAddr1 : m_bAddr2;
  // configure the sensor to start measuring
  I2c.write(m_bAddr, 0x00, 0x05);
}

void TLV493D::deinit()
{
  // stop communicating
  I2c.end();
  // power down the sensor
  digitalWrite(m_iPwrPin, LOW);
}

bool TLV493D::update()
{
  // read sensor registers and store them in rbuffer
  I2c.read(m_bAddr, 7);
  
  for (int i = 0; i < 7; i++)
  {
    m_aBuffer[i] = I2c.receive();
  }

  if (m_aBuffer[3] & B00000011 != 0)
  {
    // if bits are not 0, TLV is still reading Bx, By, Bz, or T
    //Serial.println("TLV493D data read error!");
	return false;
  }
  else
  {
    // decode read data
    int x = decodeX(m_aBuffer[0], m_aBuffer[4]);
    int y = decodeY(m_aBuffer[1], m_aBuffer[4]);
    int z = decodeZ(m_aBuffer[2], m_aBuffer[5]);
    int t = decodeT(m_aBuffer[3], m_aBuffer[6]);

	// claculate magnetic field components and temperature
    m_dBx = convertToMag(x);
    m_dBy = convertToMag(y);
    m_dBz = convertToMag(z);
    m_dTemp = convertToCelsius(t);

	// calculate angles and magnitude
    m_dPhi_xy = atan2_remaped(m_dBx, m_dBy);
    m_dPhi_yz = atan2_remaped(m_dBy, m_dBz);
    m_dPhi_xz = atan2_remaped(m_dBx, m_dBz);
    m_dMag_2 = m_dBx * m_dBx + m_dBy * m_dBy + m_dBz * m_dBz;
	
	return true;
  }
}


//-- Buffer Decode Routines --//
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


double TLV493D::convertToMag(const int a)
{
  return a * 0.098;
}

double TLV493D::convertToCelsius(const int a)
{
  return (a - 320) * 1.1;
}
