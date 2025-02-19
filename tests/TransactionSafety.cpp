#include <MicroOcpp.h>
#include <MicroOcpp/Core/Connection.h>
#include <MicroOcpp/Core/Context.h>
#include <MicroOcpp/Model/Model.h>
#include <MicroOcpp/Core/Configuration.h>
#include <MicroOcpp/Core/SimpleRequestFactory.h>
#include <MicroOcpp/Operations/BootNotification.h>
#include <MicroOcpp/Operations/StatusNotification.h>
#include <MicroOcpp/Debug.h>
#include "./catch2/catch.hpp"
#include "./helpers/testHelper.h"

using namespace MicroOcpp;


TEST_CASE( "Transaction safety" ) {

    //initialize Context with dummy socket
    LoopbackConnection loopback;
    mocpp_initialize(loopback);

    mocpp_set_timer(custom_timer_cb);

    auto connectionTimeOut = declareConfiguration<int>("ConnectionTimeOut", 30, CONFIGURATION_FN);
        *connectionTimeOut = 30;

    SECTION("Basic transaction") {
        MOCPP_DBG_DEBUG("Basic transaction");
        loop();
        startTransaction("mIdTag");
        loop();
        REQUIRE(ocppPermitsCharge());
        stopTransaction();
        loop();
        REQUIRE(!ocppPermitsCharge());

        mocpp_deinitialize();
    }

    SECTION("Managed transaction") {
        MOCPP_DBG_DEBUG("Managed transaction");
        loop();
        setConnectorPluggedInput([] () {return true;});
        beginTransaction("mIdTag");
        loop();
        REQUIRE(ocppPermitsCharge());
        endTransaction();
        loop();
        REQUIRE(!ocppPermitsCharge());
        
        mocpp_deinitialize();
    }

    SECTION("Reset during transaction 01 - interrupt initiation") {
        MOCPP_DBG_DEBUG("Reset during transaction 01 - interrupt initiation");
        setConnectorPluggedInput([] () {return false;});
        loop();
        beginTransaction("mIdTag");
        loop();
        mocpp_deinitialize(); //reset and jump to next section
    }

    SECTION("Reset during transaction 02 - interrupt initiation second time") {
        MOCPP_DBG_DEBUG("Reset during transaction 02 - interrupt initiation second time");
        setConnectorPluggedInput([] () {return false;});
        loop();
        REQUIRE(!ocppPermitsCharge());
        mocpp_deinitialize();
    }

    SECTION("Reset during transaction 03 - interrupt running tx") {
        MOCPP_DBG_DEBUG("Reset during transaction 03 - interrupt running tx");
        setConnectorPluggedInput([] () {return true;});
        loop();
        REQUIRE(ocppPermitsCharge());
        mocpp_deinitialize();
    }

    SECTION("Reset during transaction 04 - interrupt stopping tx") {
        MOCPP_DBG_DEBUG("Reset during transaction 04 - interrupt stopping tx");
        setConnectorPluggedInput([] () {return true;});
        loop();
        REQUIRE(ocppPermitsCharge());
        endTransaction();
        mocpp_deinitialize();
    }

    SECTION("Reset during transaction 06 - check tx finished") {
        MOCPP_DBG_DEBUG("Reset during transaction 06 - check tx finished");
        setConnectorPluggedInput([] () {return true;});
        loop();
        REQUIRE(!ocppPermitsCharge());
        mocpp_deinitialize();
    }

}
