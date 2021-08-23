#include "MQ7.h"
#include "cactus_io_SHT31.h"

const int AOUTpin=0;//the AOUT pin of the CO sensor goes into analog pin A0 of the arduino
const int DOUTpin=8;//the DOUT pin of the CO sensor goes into digital pin D8 of the arduino

MQ7 mq7(A0, 5.0);
cactus_io_SHT31 sht31;

void setup() {

  if (!sht31.begin())
  {
    Serial.println("Count not find sensor. Check wiring and I2C address");
    while (1) delay(1);
  }

  Serial.begin(9600);             // シリアル通信の準備をする
  while (!Serial);                  // 準備が終わるのを待つ

  pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino

}

void loop() {

  float Humidity, Temperature, Pressure, AbsoluteHumidity;
  
  Humidity = sht31.getHumidity();
  Temperature = sht31.getTemperature_C();

  // 飽和水蒸気圧(hPa)の計算
  Pressure = 6.11 * pow(10 ,(7.5 * Temperature / (Temperature + 237.3)));
  
  // 絶対湿度(水蒸気量(g/m3))
  AbsoluteHumidity = 217 * Pressure / (Temperature + 273.15) * Humidity / 100;
  
  Serial.print(sht31.getTemperature_C());
  Serial.print(","); 
  Serial.print(AbsoluteHumidity);
  Serial.print(","); 
  Serial.print(mq7.getPPM());
  Serial.println("");
  delay(500);

}
