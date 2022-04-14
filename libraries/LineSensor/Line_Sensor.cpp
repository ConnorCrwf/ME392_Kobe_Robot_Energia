#include "Line_Sensor.h"
#include "SimpleRSLK.h"

uint16_t sensorVal[LS_NUM_SENSORS];
uint16_t sensorCalVal[LS_NUM_SENSORS];
uint16_t sensorMaxVal[LS_NUM_SENSORS];
uint16_t sensorMinVal[LS_NUM_SENSORS];

uint8_t lineColor = DARK_LINE;

void setupLineSensor()
{
    clearMinMax(sensorMinVal, sensorMaxVal);
    floorCalibration();
}

void floorCalibration() {
    for (int x = 0; x < 100; x++) {
        readLineSensor(sensorVal);
        setSensorMinMax(sensorVal, sensorMinVal, sensorMaxVal);
    }
}

uint32_t readLineSensor_Kobe()
{
    readLineSensor(sensorVal);
    readCalLineSensor(sensorVal, sensorCalVal, sensorMinVal, sensorMaxVal, lineColor);
    return getLinePosition(sensorCalVal, lineColor);
}