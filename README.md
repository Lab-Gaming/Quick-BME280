# Quick-BME280
## For all your high speed measurment needs

This is a lightweight and quick library for interfacing with the BME280 sensor vis I2C..

## How to use the library

Initiate the sensor using the ```BME280.begin()``` function. 
This will initiate the sensor with default settings. 

Note **The default address is ```0x66```** 

The user can change the setting using the 
```c++
setSettings(uint8_t PT_Settings, uint8_t Hum_Settings, uint8_t config)
```
allowing the editing of the registers essential of desired BME280 operation.

### PT_Settings register:
|B0|B1|B2|B3|B4|B5|B6|B7|
|---|---|---|---|---|---|---|---|
|tempRes|tempRes|tempRes|presRes|presRes|presRes|presRes|

~~Add other regiesrters~~

The user can select 2 data output formats:
* Raw sensor data - 
```c++
getRaw()```
* Data processed by Bosch's algorithm - 
```c++
getData(float *array, uint8_t arrLen)```

