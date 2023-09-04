#pragma once

#include <float.h>
#include "ichigoplus/lib_src/utilplus.hpp"

namespace cfg{
    using namespace rp_lib;

    //configでは値を変更されたくない変数を宣言し、一括管理する
    //例えばモーターのlimitや最大出力値、gainの値は変更されたくないため、constexprを使って変更できないように設定する
    //その変数をmain.cppで宣言し各関数の引数に入れることでロボットの暴走を防ぐことができる
    //ここではomni(足回り系),arm(ハンド系),stock(押し出し機構)に分けて管理している
    namespace omni {
       
        constexpr float move_vel_limit = 100.f;
        constexpr float rotate_vel_limit = rp_lib::dtor(150.f);

        constexpr float radius = 60.f/2.f;                   	//駆動輪半径[mm]
		constexpr float attached_radius = 220.f;             	//駆動輪取り付け半径[mm]
		constexpr float pos_limit = FLT_MAX;
        constexpr float calVelLimit(){
            constexpr float vel1 = move_vel_limit / (radius / 1000.f);
            constexpr float vel2 = (attached_radius / 1000.f) * rotate_vel_limit / (radius / 1000.f);
            return vel1 + vel2;
        }
        constexpr float vel_limit = calVelLimit();
        constexpr float acc_limit = 0.8f * vel_limit;
        constexpr float dec_limit = 0.8f * vel_limit;
        constexpr float gear_ratio = 1.f/1.f;
        constexpr float pos_gain = 1.f;
        constexpr float vel_gain = 0.f;
        constexpr float acc_gain = 0.f;
        constexpr float duty_limit_min = -0.95f;
        constexpr float duty_limit_max = 0.95f;
        constexpr float omni0_angle = 5.f*f_pi/3.f;
		constexpr float omni1_angle = f_pi;
		constexpr float omni2_angle = f_pi/3.f;
        constexpr float deceleration = 1.5f; //調整必要
    }

    namespace arm {
        namespace photoint{
            constexpr float sampling_period = 2.f;
            constexpr float start_sampling_delay = 10.f;
        }
        constexpr float pos_limit_max = 1.75f; //[mm] //未設定
        constexpr float pos_limit_min = 0.f; //[mm] // 未設定
        constexpr float vel_limit = 3.f;
        constexpr float acc_limit = 3.f;
        constexpr float dec_limit = 3.f;
        constexpr float pos_gain = 2.f;
        constexpr float vel_gain = 0.f;
        constexpr float acc_gain = 0.f;
        constexpr float top_pos = 0.3f; //[mm]  //未設定
        constexpr float middle_pos = -0.8f; //[mm]  //未設定
        constexpr float control_pos = -0.3f;
        constexpr float bottom_pos = -1.0f; //[mm]  //未設定
        constexpr float slow_vel = 0.01f; //[m/s]  //調整必要
        constexpr float nomal_vel = 0.8f; //[m/s]  //調整必要
        constexpr float stop_duty = 0.f;
        constexpr float arm_enc = 50.f;
        constexpr float vertical_vel = 0.05f;
        constexpr float nomal_duty = 0.3f;
    }

    namespace stock {
        namespace photoint{
            constexpr float sampling_period = 2.f;
            constexpr float start_sampling_delay = 10.f;
        }
        constexpr float slow_duty = 0.3f;
        constexpr float nomal_duty = 0.5f;
        constexpr float stop_duty = 0.f;
    }
}
