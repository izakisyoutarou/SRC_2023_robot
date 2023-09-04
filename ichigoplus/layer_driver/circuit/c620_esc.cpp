#include "c620_esc.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

using namespace rp_lib;
namespace robomaster{

C620ESC::C620ESC(const int number, const int period_ms, const bool count, const bool rpm, const bool torque)
: encoder_(*this), motor_(*this) {
    config_.number = number;
    config_.send_period_ms = period_ms;
    config_.count_resolution = count_resolution;
    config_.send_count = count;
    config_.send_rpm = rpm;
    config_.send_torque = torque;
}

/*エンコーダ*/
int C620ESC::Encoder::setup(){
    return 0;
}
void C620ESC::Encoder::cycle(){
	radian_ = (is_reverse ? -1 : 1) * ((float)c620ESC.count / c620ESC.count_resolution * dtor(360.f));    //カウント値からradに変換
    radian_vel_ = (is_reverse ? -1 : 1) * (float)c620ESC.rpm /60.f * dtor(360.f);   //rpmからrad/sに変換
    torque_ = (float)c620ESC.torque;
}

/*モータ*/
int C620ESC::Motor::setup(){
    return 0;
}
void C620ESC::Motor::cycle(){
    c620ESC.current_ = (is_reverse ? -1 : 1) * c620ESC.current_resolution * duty_;
}
}
