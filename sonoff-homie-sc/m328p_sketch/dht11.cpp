#include "dht11.h"

uint8_t dht_humidity = 0;
uint8_t dht_temperature = 0;
uint8_t timerout_flag = 0;

static void dhtStart(uint8_t pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delayMicroseconds(2);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delay(18);
  digitalWrite(pin, HIGH);
  delayMicroseconds(40);
  pinMode(pin, INPUT);
}

static uint8_t dhtRead(uint8_t pin) {
  uint8_t i = 0;
  uint8_t timeout_flag;
  uint8_t data_temp = 0;
  uint8_t sensor_data = 0;
  for(i = 0; i < 8; i++) {
    timerout_flag = 2;
    while(digitalRead(pin) == 0 && timeout_flag++);
    delayMicroseconds(30);
    data_temp = 0;
    
    if(digitalRead(pin) == 1) {
      data_temp = 1;
    }
  
    timeout_flag = 2;
  
    while(digitalRead(pin) == 1 && timerout_flag++);
  
    if(timeout_flag == 1) {
      break;
    }
  
    sensor_data <<= 1;
    sensor_data |= data_temp;
  }
  
  return sensor_data;
}

static bool dhtRecive(uint8_t pin) {
	uint8_t i = 0;
	uint8_t data_buf[5] = {0};
	uint16_t data_temp = 0;
	static uint8_t temp_index = 0;
	static int8_t temp_humi[2][5] = {0};
	static float temp_total = 0;
	static float humi_total = 0;
	
  if(digitalRead(pin) == 0) {
    timerout_flag = 2;

    while(digitalRead(pin) == 0 && timerout_flag++);

    if(timerout_flag == 1) {
      return false;
    }
    
    timerout_flag = 2;
    
    while(digitalRead(pin) == 1 && timerout_flag++);
    
    if(timerout_flag == 1) {
      return false;
    }
    
    for(i = 0; i < 5; i++) {
      data_buf[i] = dhtRead(pin);
    
      if(i < 4) {
        data_temp += data_buf[i];
      }
    }
    
    if(data_temp == data_buf[4]) {
      dht_humidity = data_buf[0];;
      dht_temperature = data_buf[2];
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
      return true;
    } else {
      return false;
    }
  } else {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    return false;
  }
}

bool getSensorData(uint8_t pin) {
  dhtStart(pin);
  
  if(dhtRecive(pin)) {
    return true;
  } else {
    return false;
  }
}

