/*   Infinenon 3D Magnetic I2c
     TLV493D
     Initial sketch by Mark J. Hughes for AllAboutCircuits.com
     Modified and adapted for the OpenMagneticScanner by Trublion
     jeremy@trublion.org
*/

//#define TLV493D_PRINT_RAW_VALUES // uncomment for DEBUG

class TLV493D
{
  public:

    TLV493D();
    ~TLV493D();

    void init();
    void deinit();
    void update();
    
    static const byte addr; // default address of magnetic sensor 0x5E, 0x3E or 0X1F

    double Bx;
    double By;
    double Bz;

    double Temp;

    double phi_xy;
    double phi_yz;
    double phi_xz;
    double mag_2;

  private:

    byte rbuffer[10]; // store data from sensor read register

    static int decodeX(int a, int b);
    static int decodeY(int a, int b);
    static int decodeZ(int a, int b);
    static int decodeT(int a, int b);

    static double convertToMag(int a);
    static double convertToCelsius(int a);
};
