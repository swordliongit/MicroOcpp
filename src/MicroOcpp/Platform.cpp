// matth-x/MicroOcpp
// Copyright Matthias Akstaller 2019 - 2023
// MIT License

#include <MicroOcpp/Platform.h>

#ifdef MOCPP_CUSTOM_CONSOLE

namespace MicroOcpp {
void (*mocpp_console_out_impl)(const char *msg) = nullptr;
}

void MicroOcpp::mocpp_console_out(const char *msg) {
    if (mocpp_console_out_impl) {
        mocpp_console_out_impl(msg);
    }
}

void mo_set_console_out(void (*console_out)(const char *msg)) {
    MicroOcpp::mocpp_console_out_impl = console_out;
    console_out("[OCPP] console initialized\n");
}

#endif

#ifdef MOCPP_CUSTOM_TIMER
unsigned long (*mocpp_tick_ms_impl)() = nullptr;

void mocpp_set_timer(unsigned long (*get_ms)()) {
    mocpp_tick_ms_impl = get_ms;
}

unsigned long mocpp_tick_ms_custom() {
    if (mocpp_tick_ms_impl) {
        return mocpp_tick_ms_impl();
    } else {
        return 0;
    }
}
#endif

#if MOCPP_PLATFORM == MOCPP_PLATFORM_UNIX
#include <chrono>

namespace MicroOcpp {

std::chrono::steady_clock::time_point clock_reference;
bool clock_initialized = false;

}

unsigned long mocpp_tick_ms_unix() {
    if (!MicroOcpp::clock_initialized) {
        MicroOcpp::clock_reference = std::chrono::steady_clock::now();
        MicroOcpp::clock_initialized = true;
    }
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - MicroOcpp::clock_reference);
    return (unsigned long) ms.count();
}
#endif

#if MOCPP_PLATFORM != MOCPP_PLATFORM_ARDUINO
void dtostrf(float value, int min_width, int num_digits_after_decimal, char *target){
    char fmt[20];
    sprintf(fmt, "%%%d.%df", min_width, num_digits_after_decimal);
    sprintf(target, fmt, value);
}
#endif
