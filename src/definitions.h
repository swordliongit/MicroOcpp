// definitions.h

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Arduino.h>

extern String error_code_1;
extern String error_code_2;
extern String error_code_3;
extern String error_code_4;
extern String error_code_5;
extern String error_code_6;
extern String error_code_7;
extern String error_code_8;
extern String error_code_9;
extern String error_code_10;
extern String error_code_11;
extern String error_code_12;
extern String error_code_13;
extern String error_code_14;
extern String error_code_15;
extern String error_code_16;
extern String error_code_17;
extern String error_code_18;
extern String error_code_19;
extern String error_code_20;
extern String error_code_21;

extern int sensor_1;
extern int sensor_2;
extern int sensor_3;
extern int sensor_4;
extern int sensor_5;
extern int sensor_6;
extern int sensor_7;
extern int sensor_8;
extern int sensor_9;
extern int sensor_10;
extern int sensor_11;

extern double sensor_1_value;
extern double sensor_2_value;
extern double sensor_3_value;
extern int sensor_4_value;
extern int sensor_5_value;
extern int sensor_6_value;
extern int sensor_7_value;
extern int sensor_8_value;
extern int sensor_9_value;
extern int sensor_10_value;
extern int sensor_11_value;
extern int sensor_12_value;
extern int sensor_13_value;
extern int sensor_14_value;

extern String device_status;
extern String device_status_1;
extern String device_status_2;
extern String device_status_3;
extern String device_status_4;
extern String device_status_5;
extern String device_status_6;
extern String device_status_7;
extern String device_status_8;
extern String device_status_9;
extern String device_status_10;
extern String device_status_11;
extern String device_status_12;
extern String device_status_13;
extern String device_status_14;
extern String device_status_15;
extern String device_status_16;

extern int relay_1;
extern int relay_2;
extern int relay_3;
extern int relay_4;
extern int relay_5;
extern int relay_6;
extern int relay_7;
extern int relay_8;
extern int relay_9;
extern int relay_10;
extern int relay_11;

extern String relay_1_cloud_value;
extern String relay_2_cloud_value;
extern String relay_3_cloud_value;
extern String relay_4_cloud_value;
extern String relay_5_cloud_value;
extern String relay_6_cloud_value;
extern String relay_7_cloud_value;
extern String relay_8_cloud_value;
extern String relay_9_cloud_value;
extern String relay_10_cloud_value;
extern String relay_11_cloud_value;
extern String relay_12_cloud_value;
extern String relay_13_cloud_value;
extern String relay_14_cloud_value;
extern String relay_15_cloud_value;
extern String relay_16_cloud_value;


extern float voltage_1;
extern float current_1;
extern float power_1;
extern float energy_1;
extern float frequency_1;
extern float pf_1;

// Read the data from the sensorpzem sensor 2
extern float voltage_2;
extern float current_2;
extern float power_2;
extern float energy_2;
extern float frequency_2;
extern float pf_2;

// Read the data from the pzem sensor 3
extern float voltage_3;
extern float current_3;
extern float power_3;
extern float energy_3;
extern float frequency_3;
extern float pf_3;


extern int plug_in_but_no_charge_counter;

extern float package_price_1;
extern float package_price_2;
extern float package_price_3;
extern int start_charge_data_counter;
extern int plug_in_but_no_charge_counter;
extern unsigned long no_charging_counter;
extern unsigned long idle_no_charging_counter;
extern int charge_state_from_module;

extern float partner_total_payment;
extern unsigned long charge_counter;

extern int wifi_strength;

extern String swser1_serial_string;

extern double sum_of_power;

extern String charge_cost;

#endif // DEFINITIONS_H
