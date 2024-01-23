#include "definitions.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

String esp_camera_local_ip = "";
String last_esp_camera_local_ip = "";

SoftwareSerial swSer;
SoftwareSerial swSer_1;

//23 ve 5 pini
void esp_to_charge_module_communication() {
    if (swSer_1.available() > 0) {
        swser1_serial_string = swSer_1.readStringUntil('\n');
        Serial.println("Serial From Charge Module: " + swser1_serial_string);
        yield();
    }
}

void swSer_send_Data(String swSer_String) {
    swSer.println("p_" + swSer_String + "!");
}

void swSer_send_Data_Pstatus(String swSer_String) {
    swSer.println("pst_" + swSer_String + "!");
}