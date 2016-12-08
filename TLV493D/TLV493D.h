#ifndef TLV493D_H
#define TLV493D_H

#include "Arduino.h"

class TLV493D
{
  public:

    TLV493D(const int pwrPin);
    ~TLV493D();

    void init(const int dataPinState = HIGH);
    void deinit();
    void update();
    
    static const byte m_bAddr1; // address1
    static const byte m_bAddr2; // address2

    byte m_bAddr; // address being currently used

    double m_dBx;
    double m_dBy;
    double m_dBz;
    double m_dTemp;

    double m_dPhi_xy;
    double m_dPhi_yz;
    double m_dPhi_xz;
    double m_dMag_2;

  private:

    int m_iPwrPin;
    byte m_aBuffer[10]; // store data from sensor read register

    static int decodeX(const int a, const int b);
    static int decodeY(const int a, const int b);
    static int decodeZ(const int a, const int b);
    static int decodeT(int a, const int b);

    static double convertToMag(const int a);
    static double convertToCelsius(const int a);
};

#endif
