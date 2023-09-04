#include "Elevating_arm.hpp"
#include "config.hpp"
using namespace cfg;

int Elevating_arm::setup() {
    int i = 0;
    i += md.setup() != 0;
    i += top_photo.setup() != 0;
    i += bottom_photo.setup() != 0;
    return i;
}

void Elevating_arm::calibrate() {
    calibration_flag=true;
    progress=1;
}

void Elevating_arm::stop() {
    md.duty(stock::stop_duty);
}

void Elevating_arm::moveup() {
    md.duty(-stock::nomal_duty);
    if(top_photo.read() == 0) {
        md.duty(0.f);
    }
}

void Elevating_arm::movedown() {
    md.duty(stock::nomal_duty);
    if(bottom_photo.read() == 0) {
        md.duty(0.f);
    }
    
}

void Elevating_arm::cycle() {

    md.cycle();
}

