#include "config.h"
#include "led.h"
#include "communication.h"
#include "control_fun.h"
#include "encoder.h"
#include "peripheral/pwm.h"
#include "gpio.h"
#include "peripheral/usb.h"


#include "actuator.h"

const Config config;
static USB1 usb_;
#include "config_g474_boost.cpp"
static Actuator actuator_ = {config_items.fast_loop, config_items.main_loop};

Config::Config() :
    fast_loop(config_items.fast_loop),
    main_loop(config_items.main_loop),
    actuator(actuator_)
    {}

void actuator_run() {
    actuator_.run();
}

void usb_interrupt() {
    usb_.interrupt();
}