/*************************************************** 
  This is a library for the SHT31 Digital Humidity & Temp Sensor

  For more details about the SHT31 and using this library
  http://cactus.io/hookups/sensors/temperature-humidity/sht31/hookup-arduino-to-sensirion-SHT31-temp-humidity-sensor

 ****************************************************/

#include "Arduino.h"
#include "Wire.h"

#define SHT31_I2C_ADDR			   0x44
#define SHT31_MEAS_HIGHREP_STRETCH 0x2C06
#define SHT31_MEAS_MEDREP_STRETCH  0x2C0D
#define SHT31_MEAS_LOWREP_STRETCH  0x2C10
#define SHT31_MEAS_HIGHREP         0x2400
#define SHT31_MEAS_MEDREP          0x240B
#define SHT31_MEAS_LOWREP          0x2416
#define SHT31_READSTATUS           0xF32D
#define SHT31_CLEARSTATUS          0x3041
#define SHT31_SOFTRESET            0x30A2
#define SHT31_HEATEREN             0x306D
#define SHT31_HEATERDIS            0x3066

class cactus_io_SHT31 {
 public:
  
	cactus_io_SHT31(void);						// uses the default 0x44 address and no cal

    cactus_io_SHT31(uint8_t);						// use when using 0x45 address

	void setTempCal(float);						// we can set a calibration ofsset for the temperature. 
												// this offset is in degrees celsius

	float getHumidity(void);
    float getTemperature_C(void);
    float getTemperature_F(void);
    float getDewPoint(void);


	boolean begin();
	
	
	void reset(void);
	void heater(boolean);
	uint8_t crc8(const uint8_t *data, int len);

 private:

    float	 humidity;
	float    tempcal;							// stores the temp offset calibration
    float    temperature_C;                     // stores temperature in celsius
	float    temperature_F;						// stores temperature in fahrenheit


	uint16_t readStatus(void);

	boolean readTempHumidity(void);
	void writeCommand(uint16_t cmd);

	uint8_t _i2caddr;
	boolean readData(void);
};

