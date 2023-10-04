// matth-x/MicroOcpp
// Copyright Matthias Akstaller 2019 - 2023
// MIT License

#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;
#elif defined(ESP32)
#include <WiFi.h>
#else
#error only ESP32 or ESP8266 supported at the moment
#endif

#include <MicroOcpp.h>

#define STASSID "ARTINSYSTEMS"
#define STAPSK "Artin2023Artin"

#define OCPP_HOST "192.168.100.44"
#define OCPP_PORT 9000
#define OCPP_URL "ws://192.168.100.44:9000/CP_1"

unsigned long previousMillis_post_req_auth = 0;
const long interval_post_req_auth = 1000;
int counter = 0;
bool rfidCardDetected = false;

bool is_plugged = false;
bool last_plugged_status = false;

String raw_serial2 = "";
String data_from_serial2 = "6";
char screen_test_char[] = "test";
String screen_test_string = "XSarj";

//
//  Settings which worked for my SteVe instance:
//
// #define OCPP_HOST "my.instance.com"
// #define OCPP_PORT 80
// #define OCPP_URL "ws://my.instance.com/steve/websocket/CentralSystemService/esp-charger"

void setup() {
    /*
     * Initialize Serial and WiFi
     */

    Serial.begin(115200);
    Serial2.begin(9600);
    Serial.print(F("[main] Wait for WiFi: "));

#if defined(ESP8266)
    WiFiMulti.addAP(STASSID, STAPSK);
    while (WiFiMulti.run() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
#elif defined(ESP32)
    WiFi.begin(STASSID, STAPSK);
    while (!WiFi.isConnected()) {
        Serial.print('.');
        delay(1000);
    }
#else
#error only ESP32 or ESP8266 supported at the moment
#endif

    Serial.println(F(" connected!"));

    /*
     * Initialize the OCPP library
     */
    mocpp_initialize(OCPP_HOST,
                     OCPP_PORT,
                     OCPP_URL,
                     "My Charging Station",
                     "My company name");

    /*
     * Integrate OCPP functionality. You can leave out the following part if your EVSE doesn't need it.
     */
    setEnergyMeterInput([]() {
        // take the energy register of the main electricity meter and return the value in watt-hours
        return 0.f;
    });

    setSmartChargingCurrentOutput([](float limit) {
        // set the SAE J1772 Control Pilot value here
        Serial.printf(
            "[main] Smart Charging allows maximum charge rate: %.0f\n",
            limit);
    });

    setConnectorPluggedInput([]() {
        // return true if an EV is plugged to this EVSE
        return false;
    });

    //... see MicroOcpp.h for more settings
}

void serial2_get_data() {
    if (Serial2.available() > 0) {
        raw_serial2 = Serial2.readStringUntil('\n');
        Serial.println("raw_serial2: " + raw_serial2);
        if (raw_serial2.indexOf("p_") >= 0 && raw_serial2.indexOf("!") >= 0) {
            screen_test_string =
                raw_serial2.substring(raw_serial2.indexOf("p_") + 2,
                                      raw_serial2.indexOf("!"));
            Serial.println("screen_test_string: " + screen_test_string);
            yield();
        }
        if (raw_serial2.indexOf("pst_") >= 0 && raw_serial2.indexOf("!") >= 0) {
            data_from_serial2 =
                raw_serial2.substring(raw_serial2.indexOf("pst_") + 4,
                                      raw_serial2.indexOf("!"));
            Serial.println("data_from_serial2: " + data_from_serial2);
            // screen_test_string = data_from_serial2;
            yield();
        }
        // change_wifi_Command();
    }
}

void loop() {
    // serial2_get_data();

    /*
     * Do all OCPP stuff (process WebSocket input, send recorded meter values to Central System, etc.)
     */
    mocpp_loop();

    /*
     * Energize EV plug if OCPP transaction is up and running
     */
    if (ocppPermitsCharge()) {
        // OCPP set up and transaction running. Energize the EV plug here
    } else {
        // No transaction running at the moment. De-energize EV plug
    }

    /*
     * Use NFC reader to start and stop transactions
     */

    // Time management variables
    // static unsigned long chargingStartTime = 0;
    // static unsigned long chargingDuration = 10000;        // Charging duration in milliseconds (10 seconds)
    // static unsigned long intervalBetweenCharges = 20000;  // Interval between charges in milliseconds (20 seconds)
    // static bool chargingInProgress = false;

    // unsigned long currentMillis = millis();

    String idTag = "0123456789ABCD";

    if (raw_serial2 == "plugged") {
        is_plugged = true;
    } else if (raw_serial2 == "unplugged") {
        is_plugged = false;
    }

    is_plugged = true;

    if (is_plugged == true && last_plugged_status == false) {
        last_plugged_status = true;
        setConnectorPluggedInput([]() {
            // return true if an EV is plugged to this EVSE
            return true;
        });
        // // Begin a new transaction
        // Serial.printf("[main] Begin Transaction with idTag %s\n",
        //               idTag.c_str());
        // auto ret = beginTransaction(idTag.c_str());

        // Serial2.println("beginTransaction");

        // if (ret) {
        //     Serial.println(F("[main] Transaction initiated. OCPP lib will send "
        //                      "a StartTransaction when"
        //                      "ConnectorPlugged Input becomes true and if the "
        //                      "Authorization succeeds"));
        // } else {
        //     Serial.println(F("[main] No transaction initiated"));
        // }
    } else if (is_plugged == false && last_plugged_status == true) {
        last_plugged_status = false;
        setConnectorPluggedInput([]() {
            // return true if an EV is plugged to this EVSE
            return false;
        });
    }

    // Check if it's time to stop the charging cycle
    if (/* end transaction? */ false) {
        // End the transaction
        Serial.println(F("[main] End transaction after charging"));

        setConnectorPluggedInput([]() {
            // return true if an EV is plugged to this EVSE
            return false;
        });

        endTransaction();
    }

    // Use NFC reader logic here to detect RFID card, similar to your previous code
    // ...
}

//... see MicroOcpp.h for more possibilities
