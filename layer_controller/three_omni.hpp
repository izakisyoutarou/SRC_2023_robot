#pragma once

#include "ichigoplus/layer_controller/omni.hpp"
#include "ichigoplus/layer_controller/chassis_driver.hpp"
#include "ichigoplus/layer_driver/circuit/bno055.hpp"

namespace three_omni {

//using Bno055 = bno055::Bno055;

//usingを使用することでそのクラス内のコードを省略することができる
//下の例でChassisDriverと入力するとchassis_driver::ChassisDriverが入力されたことになる
using ChassisDriver = chassis_driver::ChassisDriver;
using Omni = four_omni::Omni;
using OmniLimit = four_omni::OmniLimit;


class ThreeOmni : public ChassisDriver {    //始めのpublicはクラスの継承をしている、既存のChassisDriverクラスにthreeomniの機能を加える
public:
    //初期化子リスト クラス名 オブジェクト化
    ThreeOmni(Omni &omni0, Omni &omni1, Omni &omni2, Bno055 &gyro, float move_vel_limit, float ang_vel_limit):
        omni0(omni0),omni1(omni1),omni2(omni2),gyro(gyro),move_vel_limit(move_vel_limit),ang_vel_limit(ang_vel_limit),
        motor_vel_limit(rp_lib::absMax({omni0.limit.vel, omni1.limit.vel, omni2.limit.vel})) {}
    ThreeOmni(Omni &omni0, Omni &omni1, Omni &omni2, Bno055 &gyro):ThreeOmni(omni0, omni1, omni2, gyro, 1.f, 2.f / 3.f * rp_lib::f_pi) {};
    
    //関数のオーバーライド、ChassisDriverクラスの中にあるvirtual関数をtherromniクラスで使用する（正確には上書）
    int setup() override;
    void cycle() override;
    void stop() override { move(TwoVector(0.f,0.f), 0.f); omni0.stop(); omni1.stop(); omni2.stop(); }
    void restart() override { omni0.restart(); omni1.restart(); omni2.restart(); }
    void move(TwoVector vel, float ang_vel) override;
    void omni_stop();
    //2023よりChassisDriverにこの２つの関数が追加された。
    void calibrate() override{};
    void reset() override{};

private:
    //オブジェクト化
    Omni &omni0;
    Omni &omni1;
    Omni &omni2;
    Bno055 &gyro;

    //宣言時に入力されたデータを変更する必要がない場合はconstを利用する
    //プログラムの書き換えを防ぐことができる
    const float move_vel_limit;
    const float ang_vel_limit;
    const float motor_vel_limit;

    float move_direction=0.f;
    float move_vel=0.f;
    float ang_vel=0.f;
};

}
