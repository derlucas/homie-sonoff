#ifndef   _GLOBAL_H_
#define   _GLOBAL_H_

#include "Arduino.h"
#include <inttypes.h>

// comment in/out if you want to use a MH-Z14 CO2 Sensor
// connect the Sensor to GND and +3,3V
// connect Sensor RX to 13 (ISP header Pin 1)
// connect Sensor TX to 12 (ISP header Pin 3)
#define CO2_ENABLED


typedef struct _sensorDev {
  union {
    uint32_t total;
    int32_t temp_humi_total[2];
  };
  
  union {
    uint16_t average_value;
    int8_t temp_humi_average[2];
  };
  
  int8_t level;
  int8_t last_level;
  int pin;
} sensorDev;

#ifdef CO2_ENABLED
extern sensorDev sensor_dev[5];
#else
extern sensorDev sensor_dev[4];
#endif

#endif
