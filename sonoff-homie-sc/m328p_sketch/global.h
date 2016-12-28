#ifndef   _GLOBAL_H_
#define   _GLOBAL_H_

#include "Arduino.h"
#include <inttypes.h>


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

extern sensorDev sensor_dev[4];


#endif
