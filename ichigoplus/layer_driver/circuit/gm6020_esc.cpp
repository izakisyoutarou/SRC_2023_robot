#include "gm6020_esc.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

using namespace rp_lib;
namespace robomaster{

GM6020ESC::GM6020ESC(const int number, const int period_ms, const bool count, const bool rpm, const bool torque)
: encoder_(*this), motor_(*this) {
    config_.number = number+4;  //GMをCシリーズと同じプロトコルで扱うためモータIDをシフト
    config_.send_period_ms = period_ms;
    config_.count_resolution = count_resolution;
    config_.send_count = count;
    config_.send_rpm = rpm;
    config_.send_torque = torque;
}

/*エンコーダ*/
int GM6020ESC::Encoder::setup(){
    return 0;
}
void GM6020ESC::Encoder::cycle(){
	radian_ = (is_reverse ? -1 : 1) * ((float)gm6020ESC.count / gm6020ESC.count_resolution * dtor(360.f));    //カウント値からradに変換
    radian_vel_ = (is_reverse ? -1 : 1) * (float)gm6020ESC.rpm /60.f * dtor(360.f);   //rpmからrad/sに変換
    torque_ = (float)gm6020ESC.torque;
}

/*モータ*/
int GM6020ESC::Motor::setup(){
    return 0;
}
void GM6020ESC::Motor::cycle(){
    gm6020ESC.current_ = (is_reverse ? -1 : 1) * gm6020ESC.current_resolution * duty_;
}
}
