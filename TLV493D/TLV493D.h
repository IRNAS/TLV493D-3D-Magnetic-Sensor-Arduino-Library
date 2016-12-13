#ifndef TLV493D_H
#define TLV493D_H

#include "Arduino.h"

/*! \class TLV493D
	\brief Class for communication with TLV493D 3D magnetic sensor.
*/
class TLV493D
{
  public:
	
	// Constructor
    TLV493D(const int pwrPin);
	
	// Destructor
    ~TLV493D();

	// Initializes the sensor
    void init(const int dataPinState = HIGH);
	
    // Powers off the sensor
	void deinit();
	
	// Measures new data
    bool update();
    
    static const byte m_bAddr1; ///< Sensor address1
    static const byte m_bAddr2; ///< Sensor address2

    byte m_bAddr; ///< Address being currently used

    double m_dBx; ///< Magnetic field X component
    double m_dBy; ///< Magnetic field Y component
    double m_dBz; ///< Magnetic field Z component
    double m_dTemp; ///< Temperature of the sensor

    double m_dPhi_xy; ///< Angle in radians between magnetic field X and Y components
    double m_dPhi_yz; ///< Angle in radians between magnetic field Y and Z components
    double m_dPhi_xz; ///< Angle in radians between magnetic field X and Z components
    double m_dMag_2; ///< Square of the magnitude of magnetic field flux density

  private:

    int m_iPwrPin; ///< Sensor is powered from this Arduino pin
    byte m_aBuffer[10]; ///< Storage for data from sensor read register

	// Returns true angle in radians between 0 and 2PI
	static double atan2_remaped(double x, double y);
	
	// Decode Bx
    static int decodeX(const int a, const int b);
	// Decode By
    static int decodeY(const int a, const int b);
	// Decode Bz
    static int decodeZ(const int a, const int b);
	// Decode temperature
    static int decodeT(int a, const int b);

	// Used for calculating size of each magnetic field components
    static double convertToMag(const int a);
	// Calculate temperature in degrees Celsius
    static double convertToCelsius(const int a);
};

#endif
