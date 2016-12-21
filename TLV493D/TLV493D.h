/*   Infinenon 3D Magnetic I2C
     TLV493D
     by Mark J. Hughes
     for AllAboutCircuits.com
     20160817
*/

#ifndef TLV493D_H
#define TLV493D_H

#include "Arduino.h"

/*! \class TLV493D
	\brief Class for communication with TLV493D 3D magnetic sensor.
*/
class TLV493D
{
  public:

    static const byte m_bAddr1; ///< Sensor address1
    static const byte m_bAddr2; ///< Sensor address2

    double m_dBx; ///< Magnetic field X component
    double m_dBy; ///< Magnetic field Y component
    double m_dBz; ///< Magnetic field Z component
    double m_dTemp; ///< Temperature of the sensor

    double m_dPhi_xy; ///< Angle in radians between magnetic field X and Y components
    double m_dPhi_yz; ///< Angle in radians between magnetic field Y and Z components
    double m_dPhi_xz; ///< Angle in radians between magnetic field X and Z components
    double m_dMag_2; ///< Square of the magnitude of magnetic field flux density

  private:

    byte m_bAddr; ///< Address being currently used

    uint8_t m_rbuffer[10]; ///< store data from sensor read registers
    uint8_t m_wbuffer[4];  ///< store data for sensor write registers.

    /*
    	Mode 1 is the second write register:
    	Mode1_Int   Bxxxxx1xx  Interrupt Enable "1" / Disable "0"
    	Mode1_Fast  Bxxxxxx1x  Fast Mode Enable "1" / Disable "0" must be 0 for power down
    	Mode1_Low   Bxxxxxxx1  Low Power Mode Enable "1" / Disable "0"

    	Mode 2 is the fourth write register:
    	Mode2_T     B1xxxxxxx  Temperature Measurement Enable "1" / Disable "0"
    	Mode2_LP    Bx1xxxxxx  LP Period "1" = 12ms / "0"=100ms
    	Mode2_PT    Bxx1xxxxx  Parity test Enable "1" / Disable "0"
    */

    //     Example settings for Ultra-Low Power, Low Power, Fast Mode, and Power Down.
    //                        Reg 1      Reg 2      Reg 3      Reg 4
    const uint8_t m_ulpm[4] = { B0000000, B0000001, B0000000, B10000000 }; ///< Ultra low power mode
    const uint8_t m_lpm[4]  = { B0000000, B0000001, B0000000, B11000000 }; ///< Low power mode //<--
    const uint8_t m_fm[4]   = { B0000000, B0000010, B0000000, B10000000 }; ///< Fast mode (unsupported)
    const uint8_t m_pd[4]   = { B0000000, B0000001, B0000000, B00000000 }; ///< Power down mode.


  public:

    // Constructor
    TLV493D();

    // Destructor
    ~TLV493D();

    // Initializes the sensor
    uint8_t init(int pwrPinLevel);

    // Deinitializes the sensor
    void deinit();

    // Reads new data from the sensor.
    uint8_t update();


  private:

    // Decodes data in read buffer
    static int decodeX(int a, int b);
    static int decodeY(int a, int b);
    static int decodeZ(int a, int b);
    static int decodeT(int a, int b);

    // Used for calculating size of each magnetic field components
    static double convertToMag(const int a);
    // Calculate temperature in degrees Celsius
    static double convertToCelsius(const int a);

    // Calculates angle in radians between 0 and 2PI
    static double atan2_remaped(double x, double y);

    // Print content of all registers
    void dump_registers();
};

#endif
