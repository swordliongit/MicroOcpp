// #include <HardwareSerial.h>

int counter = 0;

// bool plugged = false;

void setup() {
    Serial.begin(115200); //open serial via USB to PC on default port
    Serial2.begin(9600);  //open the other serial port
}

void loop() {

    Serial2.println("plugged");
    delay(15000);
    Serial2.println("unplugged");
    delay(15000);
}
