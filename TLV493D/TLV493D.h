#ifndef TLV493D_H
#define TLV493D_H

#include "Arduino.h"

class TLV493D
{
  public:

    TLV493D(const int pwrPin);
    ~TLV493D();

	// initialize sensor
    void init(const int dataPinState = HIGH);
    // power off sensor
	void deinit();
	// measure new data
	// returns true if data available
    bool update();
    
    static const byte m_bAddr1; // address1
    static const byte m_bAddr2; // address2

    byte m_bAddr; // address being currently used

    double m_dBx; // magnetic field X component
    double m_dBy; // magnetic field Y component
    double m_dBz; // magnetic field Z component
    double m_dTemp; // temperature of the sensor

    double m_dPhi_xy; // angle in radians between magnetic field X and Y components
    double m_dPhi_yz; // angle in radians between magnetic field Y and Z components
    double m_dPhi_xz; // angle in radians between magnetic field X and Z components
    double m_dMag_2; // square of the value of magnetic field flux density

  private:

    int m_iPwrPin; // sensor powered from this Arduino pin
    byte m_aBuffer[10]; // store data from sensor read register

	static double atan2_remaped(double x, double y); // returns true angle in radians between 0 and 2PI
	
    static int decodeX(const int a, const int b);
    static int decodeY(const int a, const int b);
    static int decodeZ(const int a, const int b);
    static int decodeT(int a, const int b);

    static double convertToMag(const int a); // used for calculating size of each magnetic field components
    static double convertToCelsius(const int a); // calculate temperature in deg. Celsius
};

#endif
