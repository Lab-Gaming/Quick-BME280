#include "BME280.h"
#include <Wire.h>

BME280::BME280() {}

bool BME280::I2Cread(uint8_t addr,  uint8_t* data,  uint8_t len) {
  uint8_t pos = 0;

  Wire.beginTransmission(addr);
  Wire.requestFrom(addr, len);

  while(Wire.available() && pos<len) {
    data[pos] = Wire.read();
    pos++;
  }
  return pos == len;
}

bool BME280::I2Cwrite(uint8_t addr,  uint8_t* data, uint8_t len) {
  Wire.beginTransmission(addr);
  Wire.write(data, len);
  Wire.endTransmission();

  return true; // TODO: Chech return values from wire calls.
}

bool BME280::begin() {

	bool ret;
	uint8_t data[2];
	uint8_t len=0;

	I2Cwrite(BME_ADDR, &ID_ADDR, 1);
	ret = I2Cread(BME_ADDR, data, 1);

	if(ret) {


		if(data[0] == 0x60) {


			data[0] = CTRL_HUM_ADDR;
			data[1] = DEFAULT_HUM_SETTINGS;
			I2Cwrite(BME_ADDR, data, 2);

			data[0] = CTRL_MEAS_ADDR;
			data[1] = DEFAULT_PT_SETTINGS;
			I2Cwrite(BME_ADDR, data, 2);


			I2Cwrite(BME_ADDR, &TEMP_DIG_ADDR, 1);
			ret = I2Cread(BME_ADDR, m_dig, TEMP_DIG_LENGTH);
			len += TEMP_DIG_LENGTH;
			if(!ret) {return ret;}


			I2Cwrite(BME_ADDR, &PRESS_DIG_ADDR, 1);
			ret = I2Cread(BME_ADDR, m_dig+len, PRESS_DIG_LENGTH);
			len += PRESS_DIG_LENGTH;
			if(!ret) {return ret;}


			I2Cwrite(BME_ADDR, &HUM_DIG_ADDR1, 1);
			ret = I2Cread(BME_ADDR, m_dig+len, HUM_DIG_ADDR1_LENGTH);
			len += HUM_DIG_ADDR1_LENGTH;
			if(!ret) {return ret;}

			HUM_DIG_ADDR2;
			I2Cwrite(BME_ADDR, &HUM_DIG_ADDR2, 1);
			I2Cread(BME_ADDR, m_dig+len, HUM_DIG_ADDR2_LENGTH);
			if(!ret) {return ret;}

		} else {return 0;}
	} else {
		ret = 0;
	}
	return ret;
}

int32_t BME280::calcTemp(int32_t raw, int32_t* t_fine) {
	int32_t var1, var2;
    uint16_t dig_T1 = (m_dig[1] << 8) | m_dig[0];
    int16_t   dig_T2 = (m_dig[3] << 8) | m_dig[2];
    int16_t   dig_T3 = (m_dig[5] << 8) | m_dig[4];
    var1 = ((((raw >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((raw >> 4) - ((int32_t)dig_T1)) * ((raw >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    *t_fine = var1 + var2;
	return (*t_fine * 5 + 128) >> 8;
}
uint32_t BME280::calcPres(int32_t raw, int32_t t_fine) {

	   int64_t var1, var2, pressure;

	   uint16_t dig_P1 = (m_dig[7]   << 8) | m_dig[6];
	   int16_t   dig_P2 = (m_dig[9]   << 8) | m_dig[8];
	   int16_t   dig_P3 = (m_dig[11] << 8) | m_dig[10];
	   int16_t   dig_P4 = (m_dig[13] << 8) | m_dig[12];
	   int16_t   dig_P5 = (m_dig[15] << 8) | m_dig[14];
	   int16_t   dig_P6 = (m_dig[17] << 8) | m_dig[16];
	   int16_t   dig_P7 = (m_dig[19] << 8) | m_dig[18];
	   int16_t   dig_P8 = (m_dig[21] << 8) | m_dig[20];
	   int16_t   dig_P9 = (m_dig[23] << 8) | m_dig[22];

	   var1 = (int64_t)t_fine - 128000;
	   var2 = var1 * var1 * (int64_t)dig_P6;
	   var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
	   var2 = var2 + (((int64_t)dig_P4) << 35);
	   var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
	   var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
	   if (var1 == 0) { return -1; }                                                         // Don't divide by zero.
	   pressure   = 1048576 - raw;
	   pressure = (((pressure << 31) - var2) * 3125)/var1;
	   var1 = (((int64_t)dig_P9) * (pressure >> 13) * (pressure >> 13)) >> 25;
	   var2 = (((int64_t)dig_P8) * pressure) >> 19;
	   pressure = ((pressure + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);

	   return ((uint32_t)pressure);
}

uint32_t BME280::calcHum(int32_t raw, int32_t t_fine) {
	   int32_t var1;
	   uint8_t   dig_H1 =   m_dig[24];
	   int16_t dig_H2 = (m_dig[26] << 8) | m_dig[25];
	   uint8_t   dig_H3 =   m_dig[27];
	   int16_t dig_H4 = (m_dig[28] << 4) | (0x0F & m_dig[29]);
	   int16_t dig_H5 = (m_dig[30] << 4) | ((m_dig[29] >> 4) & 0x0F);
	   int8_t   dig_H6 =   m_dig[31];

	   var1 = (t_fine - ((int32_t)76800));
	   var1 = (((((raw << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * var1)) +
	   ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)dig_H6)) >> 10) * (((var1 *
	   ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
	   ((int32_t)dig_H2) + 8192) >> 14));
	   var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
	   var1 = (var1 < 0 ? 0 : var1);
	   var1 = (var1 > 419430400 ? 419430400 : var1);
	   return ((uint32_t)(var1 >> 12));
}


void BME280::getRaw() {

}

void BME280::getData(float *array, uint8_t arrLen) {
	if(arrLen<3) {return;}
	uint8_t data[8];
	uint32_t p, t, h;
	int32_t t_f;

	I2Cwrite(BME_ADDR, &PRESS_ADDR, 1);
	I2Cread(BME_ADDR, data, 8);
	t = ((uint32_t)data[3] << 12 | (uint32_t)data[4]<<4) | data[5] >> 4;
	

	p = ((uint32_t)data[0] << 12 | (uint32_t)data[1]<<4) | data[2] >> 4;
	

	h = ((uint32_t)data[6] << 8) | data[7];
	array[0] = calcTemp(t, &t_f)/100.0;
	array[1] = calcPres(p, t_f)/256.0;
	array[2] = calcHum(h, t_f)/1024.0;
}

void BME280::setSettings(uint8_t PT_Settings, uint8_t Hum_Settings, uint8_t config) {
	uint8_t data[2];

	data[0] = CTRL_HUM_ADDR;
	data[1] = Hum_Settings;
	I2Cwrite(BME_ADDR, data, 2);

	data[0] = CTRL_MEAS_ADDR;
	data[1] = PT_Settings;
	I2Cwrite(BME_ADDR, data, 2);

	data[0] = CONFIG_ADDR;
	data[1] = config;
	I2Cwrite(BME_ADDR, data, 2);
}

uint8_t BME280::getStatus() {
	uint8_t *data;
	*data = STATUS_ADDR;
	I2Cwrite(BME_ADDR, data, 1);
	I2Cread(BME_ADDR, data, 1);
	return *data;
}

void BME280::softReset() {
	uint8_t data[2] = {RESET_ADDR, RESET_COMMAND};
	I2Cwrite(BME_ADDR, data, 2);
}