

#ifndef TG_BME_280_H
#define TG_BME_280_H


#include <stdint.h>
   class BME280
   {
      private:
      
         inline static const uint8_t DEFAULT_PT_SETTINGS		= 0b00100101;
         inline static const uint8_t DEFAULT_HUM_SETTINGS	= 0b00000001;
         inline static const uint8_t BME_ADDR 		= 0x76;
         inline static const uint8_t CTRL_HUM_ADDR   = 0xF2;
         inline static const uint8_t CTRL_MEAS_ADDR  = 0xF4;
         inline static const uint8_t CONFIG_ADDR     = 0xF5;
         inline static const uint8_t PRESS_ADDR      = 0xF7;
         inline static const uint8_t TEMP_ADDR       = 0xFA;
         inline static const uint8_t HUM_ADDR        = 0xFD;
         inline static const uint8_t TEMP_DIG_ADDR   = 0x88;
         inline static const uint8_t PRESS_DIG_ADDR  = 0x8E;
         inline static const uint8_t HUM_DIG_ADDR1   = 0xA1;
         inline static const uint8_t HUM_DIG_ADDR2   = 0xE1;
         inline static const uint8_t ID_ADDR         = 0xD0;
         inline static const uint8_t STATUS_ADDR 	= 0xF3;
         inline static const uint8_t RESET_ADDR		= 0xE0;

         inline static const uint8_t TEMP_DIG_LENGTH         = 6;
         inline static const uint8_t PRESS_DIG_LENGTH        = 18;
         inline static const uint8_t HUM_DIG_ADDR1_LENGTH    = 1;
         inline static const uint8_t HUM_DIG_ADDR2_LENGTH    = 7;
         inline static const uint8_t DIG_LENGTH              = 32;
         inline static const uint8_t SENSOR_DATA_LENGTH      = 8;

         inline static const uint8_t RESET_COMMAND 			= 0xB6;

         uint8_t m_dig[32];

         bool I2Cwrite(uint8_t addr, uint8_t* data, uint8_t len);

         bool I2Cread(uint8_t addr, uint8_t* data, uint8_t len);

         uint8_t rawData[8];
         
         bool newData = 0;
      
      public:

         BME280();

         bool begin();

         void setSettings(uint8_t PT_Settings, uint8_t Hum_Settings, uint8_t config);
         
         void softReset();

         int32_t calcTemp(int32_t raw, int32_t* t_fine);
         uint32_t calcPres(int32_t raw, int32_t t_fine);
         uint32_t calcHum(int32_t raw, int32_t t_fine);

         uint8_t getStatus();

         void getRaw();
         void getData(float *array, uint8_t arrLen);
   };


#endif /* BME280_H_ */