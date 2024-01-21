/*
 * bme280.h
 *
 *  Created on: Nov 25, 2023
 *      Author: Denys Derihlazov
 *      git: https://github.com/denysderihlazov
 */

#ifndef BME280_H_
#define BME280_H_

#include "i2c.h"

void BME280_Init();

void BME280_ReadCalibrationData(uint32_t adc_T, uint32_t adc_P, uint32_t adc_H);

void BME280_Config();

#endif /* BME280_H_ */
