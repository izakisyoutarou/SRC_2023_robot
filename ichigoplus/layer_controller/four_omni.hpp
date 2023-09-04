#pragma once
/*******************************************
 * FourOmni ver2.0
 * This program control four wheel omni controller
 * 
 * [Dependency]
 * torippy.hpp
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/22 Refactoring. Merge into ichigoplus.
 * ver1.0 2020/03/11 The first version
 ******************************************/

#include "ichigoplus/lib_src/utilplus.hpp"
#include "omni.hpp"
#include "ichigoplus/layer_controller/self_localization.hpp"
#include "ichigoplus/layer_controller/chassis_driver.hpp"

namespace four_omni {

using SelfLocalization = self_localization::SelfLocalization;
using ChassisDriver = chassis_driver::ChassisDriver;

class FourOmni : public ChassisDriver {
public:
    FourOmni(Omni &omni0, Omni &omni1, Omni &omni2, Omni &omni3, const SelfLocalization &selfLocalization, float move_vel_limit, float ang_vel_limit):
        omni0(omni0),omni1(omni1),omni2(omni2),omni3(omni3),selfLocalization(selfLocalization),move_vel_limit(move_vel_limit),ang_vel_limit(ang_vel_limit),
        motor_vel_limit(rp_lib::absMax({omni0.limit.vel, omni1.limit.vel, omni2.limit.vel, omni3.limit.vel})) {}
    FourOmni(Omni &omni0, Omni &omni1, Omni &omni2, Omni &omni3, SelfLocalization &selfLocalization):FourOmni(omni0, omni1, omni2, omni3, selfLocalization, 1.f, 2.f / 3.f * rp_lib::f_pi) {};
    int setup() override;
    void cycle() override;
    void stop() override { move(TwoVector(0.f,0.f), 0.f); omni0.stop(); omni1.stop(); omni2.stop(); omni3.stop(); }
    void restart() override { omni0.restart(); omni1.restart(); omni2.restart(); omni3.restart(); }
    void move(TwoVector vel, float ang_vel) override;

private:
    Omni &omni0;
    Omni &omni1;
    Omni &omni2;
    Omni &omni3;
    const SelfLocalization &selfLocalization;

    const float move_vel_limit;
    const float ang_vel_limit;
    const float motor_vel_limit;

    float move_direction=0.f;
    float move_vel=0.f;
    float ang_vel=0.f;
};

}