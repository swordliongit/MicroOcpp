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
#include <MicroOcpp/Core/Time.h>
#include <MicroOcpp/Model/Model.h>

// #define STASSID "SWORDLION"
// #define STAPSK "doomslayer98"
// #define OCPP_HOST "192.168.93.216"
// #define OCPP_PORT 9000
// #define OCPP_URL "ws://192.168.93.216:9000/CP_1"
// #define STASSID "HUNLAR"
// #define STAPSK "hun145368"
#define STASSID "ARTINSYSTEMS"
#define STAPSK "Artin2023Artin"
// #define OCPP_HOST "178.157.15.196"
#define OCPP_HOST "ocpp.petroo.dev"
// #define OCPP_PORT 12415
#define OCPP_PORT 6012
// #define OCPP_URL "ws://178.157.15.196:12415/CP_1"
#define OCPP_URL "ws://ocpp.petroo.dev:6012/ocpp/16/00004464CE8C"

/*
 CHARGING INTEGRATION
*/
TaskHandle_t Task1;

void loop2(void* param);
void energy_Analyzer_Relay_1();
void charge_Time_plug_Out_Status();
void idle_Timer();
void check_Money_Finish();
void read_Energy_Serial();
void read_Sensors();
void charge_State();
void control_Relays();


float voltage_1 = 0;
float current_1 = 0;
float power_1 = 0;
float energy_1 = 0;
float frequency_1 = 0;
float pf_1 = 0;

// Read the data from the sensorpzem sensor 2
float voltage_2 = 0;
float current_2 = 0;
float power_2 = 0;
float energy_2 = 0;
float frequency_2 = 0;
float pf_2 = 0;

// Read the data from the pzem sensor 3
float voltage_3 = 0;
float current_3 = 0;
float power_3 = 0;
float energy_3 = 0;
float frequency_3 = 0;
float pf_3 = 0;

String error_code_1 = "0";
String error_code_2 = "0";
String error_code_3 = "0";
String error_code_4 = "0";
String error_code_5 = "0";
String error_code_6 = "0";
String error_code_7 = "0";
String error_code_8 = "0";
String error_code_9 = "0";
String error_code_10 = "0";
String error_code_11 = "0";
String error_code_12 = "0";
String error_code_13 = "0";
String error_code_14 = "0";
String error_code_15 = "0";
String error_code_16 = "0";
String error_code_17 = "0";
String error_code_18 = "0";
String error_code_19 = "0";
String error_code_20 = "0";
String error_code_21 = "0";

int sensor_1 = 100;
int sensor_2 = 100;
int sensor_3 = 100;
int sensor_4 = 39; //bit1
int sensor_5 = 34; //bit2
int sensor_6 = 12; //bit3
int sensor_7 = 35;
int sensor_8 = 100;
int sensor_9 = 33;
int sensor_10 = 100;
int sensor_11 = 100;

double sensor_1_value = 0;
double sensor_2_value = 0;
double sensor_3_value = 0;
int sensor_4_value = 0;
int sensor_5_value = 0;
int sensor_6_value = 0;
int sensor_7_value = 0;
int sensor_8_value = 0;
int sensor_9_value = 0;
int sensor_10_value = 0;
int sensor_11_value = 0;
int sensor_12_value = 0;
int sensor_13_value = 0;
int sensor_14_value = 0;

String device_status = "passive";
String device_status_1 = "passive";
String device_status_2 = "passive";
String device_status_3 = "passive";
String device_status_4 = "passive";
String device_status_5 = "passive";
String device_status_6 = "passive";
String device_status_7 = "passive";
String device_status_8 = "passive";
String device_status_9 = "passive";
String device_status_10 = "passive";
String device_status_11 = "passive";
String device_status_12 = "passive";
String device_status_13 = "passive";
String device_status_14 = "passive";
String device_status_15 = "passive";
String device_status_16 = "passive";

int relay_1 = 14;
int relay_2 = 27;
int relay_3 = 26;
int relay_4 = 2;
int relay_5 = 100;
int relay_6 = 100;
int relay_7 = 100;
int relay_8 = 100;
int relay_9 = 100;
int relay_10 = 100;
int relay_11 = 100;

String relay_1_cloud_value = "0";
String relay_2_cloud_value = "0";
String relay_3_cloud_value = "0";
String relay_4_cloud_value = "0";
String relay_5_cloud_value = "0";
String relay_6_cloud_value = "0";
String relay_7_cloud_value = "0";
String relay_8_cloud_value = "0";
String relay_9_cloud_value = "0";
String relay_10_cloud_value = "0";
String relay_11_cloud_value = "0";
String relay_12_cloud_value = "0";
String relay_13_cloud_value = "0";
String relay_14_cloud_value = "0";
String relay_15_cloud_value = "0";
String relay_16_cloud_value = "0";

int plug_in_but_no_charge_counter = 0;

float package_price_1 = 0.55;
float package_price_2 = 0.9;
float package_price_3 = 1.4;
int start_charge_data_counter = 0;
unsigned long no_charging_counter = 0;
unsigned long idle_no_charging_counter = 0;
int charge_state_from_module = 0;

float partner_total_payment = 0.0;
unsigned long charge_counter = 0;

int wifi_strength = 0;

String swser1_serial_string = "";

double sum_of_power = 0;

String charge_cost = "0";


/********************************
********************************/

unsigned long previousMillis_post_req_auth = 0;
const long interval_post_req_auth = 1000;
int counter = 0;
bool rfidCardDetected = false;

bool is_plugged = true;
bool last_plugged_status = false;

bool active = false;
bool running = false;
bool lastActiveState = false;
bool lastRunningState = false;
String lastState = "";

float energyInput = 0.0f;

String SERIAL2 = "";

int connector_id = 1;
int transaction_id = 10;
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


    //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
    xTaskCreatePinnedToCore(
        loop2,   /* Task function. */
        "Task1", /* name of task. */
        10000,   /* Stack size of task */
        NULL,    /* parameter of the task */
        1,       /* priority of the task */
        &Task1,  /* Task handle to keep track of created task */
        1);      /* pin task to core 1 */
}

// void serial2_get_data() {
//     if (Serial2.available() > 0) {
//         raw_serial2 = Serial2.readStringUntil('\n');
//         Serial.println("raw_serial2: " + raw_serial2);
//         if (raw_serial2.indexOf("p_") >= 0 && raw_serial2.indexOf("!") >= 0) {
//             screen_test_string =
//                 raw_serial2.substring(raw_serial2.indexOf("p_") + 2,
//                                       raw_serial2.indexOf("!"));
//             Serial.println("screen_test_string: " + screen_test_string);
//             yield();
//         }
//         if (raw_serial2.indexOf("pst_") >= 0 && raw_serial2.indexOf("!") >= 0) {
//             data_from_serial2 =
//                 raw_serial2.substring(raw_serial2.indexOf("pst_") + 4,
//                                       raw_serial2.indexOf("!"));
//             Serial.println("data_from_serial2: " + data_from_serial2);
//             // screen_test_string = data_from_serial2;
//             yield();
//         }
//         // change_wifi_Command();
//     }
// }

float GetEnergyValues() {
    return energyInput;

    // if (screen_test_string == "energy") {
    // }
}

typedef std::function<void(String)> TimeResponseCallback;
String getTimeFromServer() {
    String currentTimeStr; // String to store the current time

    sendRequest(
        "Heartbeat",
        []() -> std::unique_ptr<DynamicJsonDocument> {
            size_t capacity = JSON_OBJECT_SIZE(2); // Adjust capacity as needed
            auto res = std::unique_ptr<DynamicJsonDocument>(
                new DynamicJsonDocument(capacity));
            JsonObject request = res->to<JsonObject>();
            // request["connectorId"] = connector_id;
            // request["transactionId"] = transaction_id;
            return res;
        },
        [&currentTimeStr](JsonObject response) -> void {
            // Handle the server's response here
            const char* currentTime = response["currentTime"];
            Serial.println(F("\n\n[main] TIME FROM SERVER\n\n"));
            Serial.println(F(currentTime));
            Serial.println(F("\n\n[main] TIME FROM SERVER\n\n"));
            currentTimeStr = String(currentTime);
            Serial.println(F("\n\n[main] TIME FROM SERVER\n\n"));
            Serial.println(currentTimeStr);
            Serial.println(F("\n\n[main] TIME FROM SERVER\n\n"));
        });
    return currentTimeStr;
}

void sendMeterValues(int connector_id, int transaction_id) {
    sendRequest(
        "MeterValues",
        [connector_id,
         transaction_id]() -> std::unique_ptr<DynamicJsonDocument> {
            // will be called to create the request once this operation is being sent out
            size_t capacity = JSON_OBJECT_SIZE(
                460); // for calculating the required capacity, see https://arduinojson.org/v6/assistant/
            auto res = std::unique_ptr<DynamicJsonDocument>(
                new DynamicJsonDocument(capacity));
            JsonObject request = res->to<JsonObject>();
            request["connectorId"] = connector_id;
            request["transactionId"] = transaction_id;
            JsonArray meterValues = request.createNestedArray("meterValue");
            JsonObject meterValueSample = meterValues.createNestedObject();

            MicroOcpp::Clock clock;
            MicroOcpp::Timestamp ocppNow = clock.now();
            char ocppNowJson[JSONDATE_LENGTH + 1] = {'\0'};
            ocppNow.toJsonString(ocppNowJson, JSONDATE_LENGTH + 1);
            meterValueSample["timestamp"] = ocppNowJson;

            JsonArray sampledValue =
                meterValueSample.createNestedArray("sampledValue");
            JsonObject sampledValueItem = sampledValue.createNestedObject();
            sampledValueItem["value"] = String(GetEnergyValues());
            sampledValueItem["context"] = "Sample.Periodic";
            sampledValueItem["measurand"] = "Energy.Active.Import.Register";
            sampledValueItem["unit"] = "Wh";
            return res;
        },
        [](JsonObject response) -> void {
            // will be called with the confirmation response of the server
            //  const char *status = response["idTagInfo"]["status"];
            //  int transactionId = response["transactionId"];
        });
}

void loop() {
    const char* idTag = "00004464CE8C";
    int connector_id = 1;
    int transaction_id = 10;

    bool error = false;
    // Serial2.println("denemeeee");
    // if (Serial2.available()) {
    //     String receivedData = Serial2.readStringUntil('\n');
    //     SERIAL2 = receivedData;
    //     Serial.println("Received data:" + SERIAL2);
    //     if (SERIAL2.indexOf("plugged") == 0) {
    //         // Serial.println(screen_test_string);
    //         Serial.println("PLUGGED IN!");
    //         is_plugged = true;
    //     } else if (SERIAL2.indexOf("unplugged") == 0) {
    //         Serial.println("UNPLUGGED!");
    //         is_plugged = false;
    //     }
    //     yield();
    // }
    // if (counter % 70000 == 0) {
    //     is_plugged = true;
    //     Serial.println("PLUGGED IN!");
    // }
    // if (counter % 200000 == 0 && (counter)) {
    //     is_plugged = false;
    //     Serial.println("UNPLUGGED!");
    // }


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
     * Plug Check
     */


    if (is_plugged == true && last_plugged_status == false) {
        last_plugged_status = true;
        setConnectorPluggedInput([]() {
            // return true if an EV is plugged to this EVSE
            return true;
        });

        // sendMeterValues(connector_id, transaction_id);
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
        Serial.println(F("[main] Unplugging!!!"));
        setConnectorPluggedInput([]() {
            // return true if an EV is plugged to this EVSE
            return false;
        });

        Serial.println(F("[main] End transaction after charging"));
        endTransaction();
    }

    /*
     * State Check
     */

    active = isTransactionActive();
    running = isTransactionRunning();

    // Determine the current state
    String currentState;
    if (active && running) {
        currentState = "running";
        lastActiveState = true;
        lastRunningState = true;

        // simulate energy input
        ++energyInput;
    } else if (active && !running) {
        currentState = "preparing";
        lastActiveState = true;
        lastRunningState = false;
    } else if (!active && running) {
        currentState = "running/stoptxawait";
        lastActiveState = false;
        lastRunningState = true;
    } else if (!active && !running) {
        if (lastActiveState == false && lastRunningState) {
            currentState = "finished"; // or "aborted"

            energyInput = 0.0f;
        } else {
            currentState = "idle";
        }
    } else {
        currentState = "unknown"; // Handle unexpected states
    }

    // Print the state only if it has changed from the previous state
    if (currentState != lastState) {
        // sendMeterValues(connector_id, transaction_id);
        Serial.println(F("\n\n"));
        Serial.println(active);
        Serial.println(running);
        Serial.println(F("\n\n"));
        Serial.print(F("[main] State: "));
        Serial.println(currentState);
        Serial.println(F("\n\n"));

        // Send a message to Serial2 based on the current state, for the charger esp to read
        String message = "p_" + currentState + "!";
        Serial2.println(message);

        // Update the lastState variable
        lastState = currentState;
    }

    if (error == true) {
        endTransaction(idTag, "Charging error");
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
    ++counter;
}

//... see MicroOcpp.h for more possibilities

void loop2(void* param) {
    for (;;) {
        // Serial.println("In loop 2");
        energy_Analyzer_Relay_1();
        charge_Time_plug_Out_Status();
        idle_Timer();
        check_Money_Finish();
        read_Energy_Serial();
        read_Sensors();
        charge_State();

        if (charge_state_from_module == 1) {
            control_Relays();
        }
    }
}
