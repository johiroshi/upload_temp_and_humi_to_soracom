/*************************************************** 
  This is a library for the SHT31 Digital Humidity & Temp Sensor

  For more details about the SHT31 and using this library
  http://cactus.io/hookups/sensors/temperature-humidity/sht31/hookup-arduino-to-sensirion-SHT31-temp-humidity-sensor

 ****************************************************/


#include "cactus_io_SHT31.h"

cactus_io_SHT31::cactus_io_SHT31() {

	_i2caddr = SHT31_I2C_ADDR;
    
    tempcal = 0;

}

cactus_io_SHT31::cactus_io_SHT31(uint8_t addr) {

	_i2caddr = addr;
    
    tempcal = 0;

}

void cactus_io_SHT31::setTempCal(float tcal)
{
	tempcal = tcal;
}

boolean cactus_io_SHT31::begin() {
  Wire.begin();
  // _i2caddr = i2caddr;					// REMOVE
  reset();
  //return (readStatus() == 0x40);
  return true;
}

float cactus_io_SHT31::getTemperature_C(void) {
	if (! readTempHumidity()) return NAN;

    temperature_C = temperature_C + tempcal;
    
	return temperature_C;
}

float cactus_io_SHT31::getTemperature_F(void) {
    temperature_F = temperature_C * 1.8 + 32;
    return temperature_F;
}

float cactus_io_SHT31::getHumidity(void) {
  if (! readTempHumidity()) return NAN;

  return humidity;
}

float cactus_io_SHT31::getDewPoint(void) {
    float k;
    k = log(humidity/100) + (17.62 * temperature_C) / (243.12 + temperature_C);
    return 243.12 * k / (17.62 - k);
}


uint16_t cactus_io_SHT31::readStatus(void) {
  writeCommand(SHT31_READSTATUS);
  Wire.requestFrom(_i2caddr, (uint8_t)3);
  uint16_t stat = Wire.read();
  stat <<= 8;
  stat |= Wire.read();
  return stat;
}

void cactus_io_SHT31::reset(void) {
  writeCommand(SHT31_SOFTRESET);
  delay(10);
}

void cactus_io_SHT31::heater(boolean heater) {
  if (heater)
    writeCommand(SHT31_HEATEREN);
  else
    writeCommand(SHT31_HEATERDIS);
}


boolean cactus_io_SHT31::readTempHumidity(void) {
  uint8_t readbuffer[6];

  writeCommand(SHT31_MEAS_HIGHREP);
  
  delay(500);
  Wire.requestFrom(_i2caddr, (uint8_t)6);
  if (Wire.available() != 6) 
    return false;
  for (uint8_t i=0; i<6; i++) {
    readbuffer[i] = Wire.read();
  //  Serial.print("0x"); Serial.println(readbuffer[i], HEX);
  }
  uint16_t ST, SRH;
  ST = readbuffer[0];
  ST <<= 8;
  ST |= readbuffer[1];

  if (readbuffer[2] != crc8(readbuffer, 2)) return false;

  SRH = readbuffer[3];
  SRH <<= 8;
  SRH |= readbuffer[4];

  if (readbuffer[5] != crc8(readbuffer+3, 2)) return false;

  double stemp = ST;
  stemp *= 175;
  stemp /= 0xffff;
  stemp = -45 + stemp;
  temperature_C = stemp;
  
  double shum = SRH;
  shum *= 100;
  shum /= 0xFFFF;
  
  humidity = shum;
  
  return true;
}

void cactus_io_SHT31::writeCommand(uint16_t cmd) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(cmd >> 8);
  Wire.write(cmd & 0xFF);
  Wire.endTransmission();  
}

// crc calculator taken from SHT31 specification guide
uint8_t cactus_io_SHT31::crc8(const uint8_t *data, int len)
{

  const uint8_t POLYNOMIAL(0x31);
  uint8_t crc(0xFF);
  
  for ( int c = len; c; --c ) {
      crc ^= *data++;

      for ( int i = 8; i; --i ) {
			crc = ( crc & 0x80 )
			? (crc << 1) ^ POLYNOMIAL
			: (crc << 1);
      }
  }
  return crc;
}

/*********************************************************************/
