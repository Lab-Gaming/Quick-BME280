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
getRaw()
```

* Data processed by Bosch's algorithm - 
```c++
getData(float *array, uint8_t arrLen)
```
If the user would need to know the status of the device before taking a reading,
the 
```c++
getStatus()
```
method is available.


If for any reason the sensor would hang us or cease working as expected,
the user can soft-reset the device using the
```c++
softReset()
```
method 

If the user would like to use an official Red Labs BME280 breakout 
the pinout is as follows:
|P1|P2|P3|P4|
|---|---|---|---|
|3V3|GND|SCL|SDA|

Red Labs 2023
