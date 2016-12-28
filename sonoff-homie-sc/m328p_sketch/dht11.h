#ifndef  _DHT11_H_
#define  _DHT11_H_
#include "global.h"

extern uint8_t dht_humidity;
extern uint8_t dht_temperature;

bool getSensorData(uint8_t pin);

#endif

