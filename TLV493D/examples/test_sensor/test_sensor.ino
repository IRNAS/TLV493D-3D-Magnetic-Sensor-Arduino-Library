#include "Wire.h"
#include "TLV493D.h"

TLV493D sensor1;
TLV493D sensor2;

const int sensor1_pwr_pin = A2;
const int sensor2_pwr_pin = A3;
const int i2c_sda = A4;


void setup()
{
  Serial.begin(115200);      // Begin serial connection for debug.

  pinMode(sensor1_pwr_pin, OUTPUT);
  pinMode(sensor2_pwr_pin, OUTPUT);
  pinMode(i2c_sda, OUTPUT);

  digitalWrite(sensor1_pwr_pin, LOW);
  digitalWrite(sensor2_pwr_pin, LOW);
  digitalWrite(i2c_sda, LOW);

  delay(100);

  //init sensor1
  digitalWrite(sensor1_pwr_pin, HIGH);
  digitalWrite(i2c_sda, LOW); //0x1F
  Serial.println("Starting sensor 1");
  delay(500);

  //init sensor2
  digitalWrite(sensor2_pwr_pin, HIGH);
  digitalWrite(i2c_sda, HIGH); //0x5E
  Serial.println("Starting sensor 2");
  delay(500);

  Wire.begin(); // Begin I2C wire communication

  //initialize sensor 1
  Serial.print("Initializing sensor 1: 0x");
  Serial.println(sensor1.init(LOW), HEX);

  //initialize sensor 2
  Serial.print("Initializing sensor 2: 0x");
  Serial.println(sensor2.init(HIGH), HEX);
}


void loop()
{
  delay(200); // wait time between reads.

  Serial.println("Updating sensor 1.");
  uint8_t read_status1 = sensor1.update();
  if (read_status1 != 0)
  {
    // If bits are not 0, TLV is still reading Bx, By, Bz, or T
    Serial.print("Data read error: 0x");
    Serial.println(read_status1, HEX);
  }
  else
  {
    Serial.print(sensor1.m_dBx);
    Serial.print("\t");
    Serial.print(sensor1.m_dBy);
    Serial.print("\t");
    Serial.print(sensor1.m_dBz);
    Serial.print("\t");
    Serial.println(sensor1.m_dTemp);

    Serial.print(sensor1.m_dPhi_xy);
    Serial.print("\t");
    Serial.print(sensor1.m_dPhi_yz);
    Serial.print("\t");
    Serial.print(sensor1.m_dPhi_xz);
    Serial.print("\t");
    Serial.println(sensor1.m_dMag_2);
  }


  Serial.println("Updating sensor 2.");
  uint8_t read_status2 = sensor2.update();
  if (read_status2 != 0)
  {
    // If bits are not 0, TLV is still reading Bx, By, Bz, or T
    Serial.print("Data read error: 0x");
    Serial.println(read_status2, HEX);
  }
  else
  {
    Serial.print(sensor2.m_dBx);
    Serial.print("\t");
    Serial.print(sensor2.m_dBy);
    Serial.print("\t");
    Serial.print(sensor2.m_dBz);
    Serial.print("\t");
    Serial.println(sensor2.m_dTemp);

    Serial.print(sensor2.m_dPhi_xy);
    Serial.print("\t");
    Serial.print(sensor2.m_dPhi_yz);
    Serial.print("\t");
    Serial.print(sensor2.m_dPhi_xz);
    Serial.print("\t");
    Serial.println(sensor2.m_dMag_2);
  }
}
