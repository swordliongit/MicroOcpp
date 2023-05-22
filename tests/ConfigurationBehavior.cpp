#include <ArduinoOcpp.h>
#include <ArduinoOcpp/Core/OcppSocket.h>
#include <ArduinoOcpp/Core/OcppEngine.h>
#include <ArduinoOcpp/Core/OcppModel.h>
#include <ArduinoOcpp/Core/OcppMessage.h>
#include <ArduinoOcpp/Core/Configuration.h>
#include <ArduinoOcpp/Core/FilesystemUtils.h>
#include <ArduinoOcpp/Tasks/ChargeControl/ChargeControlService.h>
#include "./catch2/catch.hpp"
#include "./helpers/testHelper.h"

using namespace ArduinoOcpp;

class CustomStartTransaction : public OcppMessage {
private:
    const char *status;
public:
    CustomStartTransaction(const char *status) : status(status) { };
    const char *getOcppOperationType() override {return "StartTransaction";}
    void processReq(JsonObject payload) override {
        //ignore payload - result is determined at construction time
    }
    std::unique_ptr<DynamicJsonDocument> createConf() override {
        auto res = std::unique_ptr<DynamicJsonDocument>(new DynamicJsonDocument(JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(1)));
        auto payload = res->to<JsonObject>();
        payload["idTagInfo"]["status"] = status;
        payload["transactionId"] = 1000;
        return res;
    }
};

TEST_CASE( "Configuration Behavior" ) {

    //clean state
    auto filesystem = makeDefaultFilesystemAdapter(ArduinoOcpp::FilesystemOpt::Use_Mount_FormatOnFail);
    AO_DBG_DEBUG("remove all");
    FilesystemUtils::remove_all(filesystem, "");

    //initialize OcppEngine with dummy socket
    OcppEchoSocket echoSocket;
    OCPP_initialize(echoSocket, ChargerCredentials("test-runner1234"));

    auto engine = getOcppEngine();
    auto& checkMsg = engine->getOperationDeserializer();

    auto connector = engine->getOcppModel().getConnector(1);

    ao_set_timer(custom_timer_cb);

    loop();

    SECTION("StopTransactionOnEVSideDisconnect") {
        setConnectorPluggedInput([] () {return true;});
        startTransaction("mIdTag");
        loop();

        auto config = declareConfiguration<bool>("StopTransactionOnEVSideDisconnect", true);

        SECTION("set true") {
            *config = true;

            setConnectorPluggedInput([] () {return false;});
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::Available);
        }

        SECTION("set false") {
            *config = false;

            setConnectorPluggedInput([] () {return false;});
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::SuspendedEV);

            endTransaction();
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::Available);
        }
    }

    SECTION("StopTransactionOnInvalidId") {
        auto config = declareConfiguration<bool>("StopTransactionOnInvalidId", true);

        checkMsg.registerOcppOperation("StartTransaction", [] () {return new CustomStartTransaction("Invalid");});

        SECTION("set true") {
            *config = true;

            beginTransaction_authorized("mIdTag");
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::Available);
        }

        SECTION("set false") {
            *config = false;

            beginTransaction_authorized("mIdTag");
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::SuspendedEVSE);

            endTransaction();
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::Available);
        }
    }

    SECTION("AllowOfflineTxForUnknownId") {
        auto config = declareConfiguration<bool>("AllowOfflineTxForUnknownId", true);
        auto authorizationTimeout = ArduinoOcpp::declareConfiguration<int>("AO_AuthorizationTimeout", 1);
        *authorizationTimeout = 1; //try normal Authorize for 1s, then enter offline mode

        echoSocket.setConnected(false); //connection loss

        SECTION("set true") {
            *config = true;

            beginTransaction("mIdTag");
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::Charging);

            endTransaction();
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::Available);
        }

        SECTION("set false") {
            *config = false;

            beginTransaction("mIdTag");
            REQUIRE(connector->inferenceStatus() == OcppEvseState::Preparing);

            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::Available);
        }

        endTransaction();
        echoSocket.setConnected(true);
    }

    SECTION("LocalPreAuthorize") {
        auto config = declareConfiguration<bool>("LocalPreAuthorize", true);
        auto authorizationTimeout = ArduinoOcpp::declareConfiguration<int>("AO_AuthorizationTimeout", 20);
        *authorizationTimeout = 300; //try normal Authorize for 5 minutes

        auto localAuthorizeOffline = declareConfiguration<bool>("LocalAuthorizeOffline", true, CONFIGURATION_FN, true, true, true, false);
        *localAuthorizeOffline = true;
        auto localAuthListEnabled = declareConfiguration<bool>("LocalAuthListEnabled", true, CONFIGURATION_FN, true, true, true, false);
        *localAuthListEnabled = true;

        //define local auth list with entry local-idtag
        std::string localListMsg = "[2,\"testmsg-01\",\"SendLocalList\",{\"listVersion\":1,\"localAuthorizationList\":[{\"idTag\":\"local-idtag\",\"idTagInfo\":{\"status\":\"Accepted\"}}],\"updateType\":\"Full\"}]";
        echoSocket.sendTXT(localListMsg);
        loop();

        echoSocket.setConnected(false); //connection loss

        SECTION("set true - accepted idtag") {
            *config = true;

            beginTransaction("local-idtag");
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::Charging);
        }

        SECTION("set false") {
            *config = false;

            beginTransaction("local-idtag");
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::Preparing);

            echoSocket.setConnected(true);
            mtime += 20000; //Authorize will be retried after a few seconds
            loop();

            REQUIRE(connector->inferenceStatus() == OcppEvseState::Charging);
        }

        endTransaction();
        echoSocket.setConnected(true);
    }

    OCPP_deinitialize();
}