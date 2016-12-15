/*   Infinenon 3D Magnetic I2C
     TLV493D
     by Mark J. Hughes
     for AllAboutCircuits.com
     20160817
*/

#ifndef TLV493D_h
#define TLV493D_h

#include "Arduino.h"

class TLV493D
{
  public:

    uint8_t addr1 = 0x5E;
    uint8_t addr2 = 0x1F;
    
    uint8_t addr; // default address of magnetic sensor 0x5E or 0x1F
    uint8_t rbuffer[10];       // store data from sensor read registers
    uint8_t wbuffer[4];        // store data for sensor write registers.
    uint8_t debugcounter;      // variable for debug counter
    uint8_t delaytime = 1;     // time to wait before next read.  Delay will increase with errors.

    /*
    	Mode 1 is the second write register
    	Mode1_Int   Bxxxxx1xx  Interrupt Enable "1" / Disable "0"
    	Mode1_Fast  Bxxxxxx1x  Fast Mode Enable "1" / Disable "0" must be 0 for power down
    	Mode1_Low   Bxxxxxxx1  Low Power Mode Enable "1" / Disable "0"

    	Mode 2 is the fourth write register
    	Mode2_T     B1xxxxxxx  Temperature Measurement Enable "1" / Disable "0"
    	Mode2_LP    Bx1xxxxxx  LP Period "1" = 12ms / "0"=100ms
    	Mode2_PT    Bxx1xxxxx  Parity test Enable "1" / Disable "0"
    */

    //     Example settings for Ultra-Low Power, Low Power, Fast Mode, and Power Down.
    //                        Reg 1      Reg 2      Reg 3      Reg 4
    const uint8_t ulpm[4] = { 0b00000000, 0b00000001, 0b00000000, 0b10000000 }; // ultra low power mode
    const uint8_t lpm[4]  = { 0b00000000, 0b00000001, 0b00000000, 0b11000000 }; // low power mode //<--
    const uint8_t fm[4]   = { 0b00000000, 0b00000010, 0b00000000, 0b10000000 }; // fast mode (unsupported)
    const uint8_t pd[4]   = { 0b00000000, 0b00000001, 0b00000000, 0b00000000 }; // power down mode.

    int x;
    int y;
    int z;
    int t;

    uint8_t init(int pwrPinLevel);
    uint8_t update();

    int decodeX(int a, int b);
    int decodeY(int a, int b);
    int decodeZ(int a, int b);
    int decodeT(int a, int b);

    // Used for calculating size of each magnetic field components
    static double convertToMag(const int a);
    // Calculate temperature in degrees Celsius
    static double convertToCelsius(const int a);

    double atan2_remaped(double x, double y);
};

#endif
