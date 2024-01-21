#include "bme280.h"
#include "stdint.h"

// Uncomment it if you are using "WaveShare ILI9341 SPI 240×320, General 2.4inch LCD"
// imports for LCD screen
//#include "stm32f4xx_hal.h"
//#include "GUI_Paint.h"
//#include "DEV_Config.h"
//#include "LCD_2inch4.h"

#define BME280_REG_CHIP_ID 0xD0 // Register of chip ID
#define BME280_REG_CHIP_ID_DEFAULT 0x60 // Chip ID to compare
/**IIC address is 0x77 when pin SDO is high */
/**IIC address is 0x76 when pin SDO is low */
#define BME280_ADDRESS (0x77 << 1) // Device I2C address
#define BME280_DATA_START 0xF7 // Start address for burst read
#define BME280_DATA_LENGTH 8 // Bytes to read temperature, humidity, and pressure

#define SEA_LEVEL_PRESSURE    1015.0f

uint32_t rawPressure;
uint32_t rawTemperature;
uint16_t rawHumidity;

// Each BME280 sensor has its own calibration data. It's necessary to use the following formulas if you want highly accurate results.
void BME280_ReadCalibrationData(uint32_t adc_T, uint32_t adc_P, uint32_t adc_H)
{
    uint8_t calibrationData1[26];
    HAL_I2C_Mem_Read(&hi2c2, BME280_ADDRESS, 0x88, I2C_MEMADD_SIZE_8BIT, calibrationData1, 26, HAL_MAX_DELAY);

    uint8_t calibrationData2[7];
    HAL_I2C_Mem_Read(&hi2c2, BME280_ADDRESS, 0xE1, I2C_MEMADD_SIZE_8BIT, calibrationData2, 7, HAL_MAX_DELAY);

	// Compensation parameters
		// Temperature
		unsigned short dig_T1 = (calibrationData1[1] << 8) | calibrationData1[0];
		signed short dig_T2 = (calibrationData1[3] << 8) | calibrationData1[2];
		signed short dig_T3 = (calibrationData1[5] << 8) | calibrationData1[4];

		// Pressure
		unsigned short dig_P1 = (calibrationData1[7] << 8) | calibrationData1[6];
		signed short dig_P2 = (calibrationData1[9] << 8) | calibrationData1[8];
		signed short dig_P3 = (calibrationData1[11] << 8) | calibrationData1[10];
		signed short dig_P4 = (calibrationData1[13] << 8) | calibrationData1[12];
		signed short dig_P5 = (calibrationData1[15] << 8) | calibrationData1[14];
		signed short dig_P6 = (calibrationData1[17] << 8) | calibrationData1[16];
		signed short dig_P7 = (calibrationData1[19] << 8) | calibrationData1[18];
		signed short dig_P8 = (calibrationData1[21] << 8) | calibrationData1[20];
		signed short dig_P9 = (calibrationData1[23] << 8) | calibrationData1[22];

		// Humidity
		unsigned char dig_H1 = calibrationData1[24];
		signed short dig_H2 = (calibrationData2[1] << 8) | calibrationData2[0];
		unsigned char dig_H3 = calibrationData2[2];

		// dig_H4 and dig_H5 are a bit tricky as they are spread across bytes
		signed short dig_H4 = (calibrationData2[3] << 4) | (calibrationData2[4] & 0x0F);
		signed short dig_H5 = (calibrationData2[5] << 4) | (calibrationData2[4] >> 4);
		signed char dig_H6 = calibrationData2[6];

		// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
		// t_fine carries fine temperature as global value
		int32_t t_fine;
		int32_t temp1, temp2, T;
		temp1 = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
		temp2 = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
		t_fine = temp1 + temp2;
		T = (t_fine * 5 + 128) >> 8;
		float temperatureCelsius = T / 100.0f;

		char tempString[32];
	    sprintf(tempString, "Temperature: %.2f C", temperatureCelsius);
	    // Uncomment it if you are using "WaveShare ILI9341 SPI 240×320, General 2.4inch LCD"
	    //Paint_DrawString_EN(0, 170, tempString, &Font16, RED, GREEN);

	    // Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
	    // Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
	    int64_t press1, press2, p;
	    press1 = ((int64_t)t_fine) - 128000;
	    press2 = press1 * press1 * (int64_t)dig_P6;
	    press2 = press2 + ((press1*(int64_t)dig_P5)<<17);
	    press2 = press2 + (((int64_t)dig_P4)<<35);
	    press1 = ((press1 * press1 * (int32_t)dig_P3)>>8) + ((press1 * (int64_t)dig_P2)<<12);
	    press1 = (((((int64_t)1)<<47)+press1))*((int64_t)dig_P1)>>33;
	    if (press1 == 0)
	    {
	    	return 0; // avoid exception caused by division by zero
	    }
	    p = 1048576 - adc_P;
	    p = (((p<<31)-press2)*3125)/press1;
	    press1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	    press2 = (((int64_t)dig_P8) * p) >> 19;
	    p = ((p + press1 + press2) >> 8) + (((int64_t)dig_P7)<<4);
		float metricPressure = (int64_t)p / 256.0f;
		char pressureString[64];

	    float hPaPressure = metricPressure / 100.0f;
	    sprintf(pressureString, "Pressure: %.2f hPa", hPaPressure);
	    // Uncomment it if you are using "WaveShare ILI9341 SPI 240×320, General 2.4inch LCD"
	    //Paint_DrawString_EN(0, 190, pressureString, &Font16, RED, GREEN);


	    // Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
	    // Output value of “47445” represents 47445/1024 = 46.333 %RH
	    int32_t v_x1_u32r;
	    v_x1_u32r = (t_fine - ((int32_t)76800));
	    v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r *((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
	   	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
	  	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

	    v_x1_u32r = (v_x1_u32r >> 12);
		float metricHumidity = v_x1_u32r / 1024.0f;

		char humidityString[32];
		sprintf(humidityString, "Humidity: %.2f%%", metricHumidity);
		// Uncomment it if you are using "WaveShare ILI9341 SPI 240×320, General 2.4inch LCD"
		//Paint_DrawString_EN(0, 210, humidityString, &Font16, RED, GREEN);
}

void BME280_Config()
{
	//set-up ctrl_hum register
	uint8_t ctrl_hum_value = 0x04; // oversampling ×8 for higher precision
	uint8_t ctrl_hum_address = 0xF2;
	uint8_t ctrl_hum_reg[2] = {ctrl_hum_address, ctrl_hum_value};
	HAL_I2C_Master_Transmit(&hi2c2, BME280_ADDRESS, ctrl_hum_reg, 2, HAL_MAX_DELAY);

	//set-up ctrl_meas register
	uint8_t ctrl_meas_value = 0x27; // 001 011 11
	uint8_t ctrl_meas_address = 0xF4;
	uint8_t ctrl_meas_reg[2] = {ctrl_meas_address, ctrl_meas_value};
	HAL_I2C_Master_Transmit(&hi2c2, BME280_ADDRESS, ctrl_meas_reg, 2, HAL_MAX_DELAY);

	//set-up config register
	uint8_t config_value = 0x10; // 00010000
	uint8_t config_address = 0xF5;
	uint8_t config_reg[2] = {config_address, config_value};
	HAL_I2C_Master_Transmit(&hi2c2, BME280_ADDRESS, config_reg, 2, HAL_MAX_DELAY);
}

void BME280_Init()
{
    uint8_t regAddress = BME280_REG_CHIP_ID;
    uint8_t chipID;
    uint8_t data[10];

    // Send first bit and wait for ACK
    if (HAL_I2C_Master_Transmit(&hi2c2, BME280_ADDRESS, &regAddress, 1, HAL_MAX_DELAY) == HAL_OK) {
    	// Check if BME280 ready for conversation and chip has address 0x60
        if (HAL_I2C_Master_Receive(&hi2c2, BME280_ADDRESS, &chipID, 1, HAL_MAX_DELAY) == HAL_OK) {
        	// Uncomment it if you are using "WaveShare ILI9341 SPI 240×320, General 2.4inch LCD"
        	// Paint_DrawString_EN(0, 0, "Connection successful.", &Font24, BLACK, WHITE);
        	printf("Connection successful.\n");

        	if (chipID == BME280_REG_CHIP_ID_DEFAULT) {
        		BME280_Config();

        		// burst read of all of the raw data (to get more info check "4. Data readout" Bosch BME280 documentation).
        	    if (HAL_I2C_Mem_Read(&hi2c2, BME280_ADDRESS, BME280_DATA_START, I2C_MEMADD_SIZE_8BIT, data, BME280_DATA_LENGTH, HAL_MAX_DELAY) == HAL_OK) {
        	    	rawPressure = ((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | (data[2] >> 4);
        	        rawTemperature = ((uint32_t)data[3] << 12) | ((uint32_t)data[4] << 4) | (data[5] >> 4);
        	        rawHumidity = ((uint32_t)data[6] << 8) | data[7];
        	    }
        		BME280_ReadCalibrationData(rawTemperature, rawPressure, rawHumidity);
        	}
        }
    } else {
    	// Uncomment it if you are using "WaveShare ILI9341 SPI 240×320, General 2.4inch LCD"
    	//Paint_DrawString_EN(0, 0, "Error! Can't get ACK.", &Font24, BLACK, WHITE);
    	printf("Error! Can't get ACK.\n");
    }
}
