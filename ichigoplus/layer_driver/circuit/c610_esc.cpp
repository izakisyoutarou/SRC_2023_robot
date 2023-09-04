#include "c610_esc.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

using namespace rp_lib;
namespace robomaster{

C610ESC::C610ESC(const int number, const int period_ms, const bool count, const bool rpm, const bool torque)
: encoder_(*this), motor_(*this) {
    config_.number = number;
    config_.send_period_ms = period_ms;
    config_.count_resolution = count_resolution;
    config_.send_count = count;
    config_.send_rpm = rpm;
    config_.send_torque = torque;
}

/*エンコーダ*/
int C610ESC::Encoder::setup(){
    return 0;
}
void C610ESC::Encoder::cycle(){
	radian_ = (is_reverse ? -1 : 1) * ((float)c610ESC.count / c610ESC.count_resolution * dtor(360.f));    //カウント値からradに変換
    radian_vel_ = (is_reverse ? -1 : 1) * (float)c610ESC.rpm /60.f * dtor(360.f);   //rpmからrad/sに変換
    torque_ = (float)c610ESC.torque;
}

/*モータ*/
int C610ESC::Motor::setup(){
    return 0;
}
void C610ESC::Motor::cycle(){
    c610ESC.current_ = (is_reverse ? -1 : 1) * c610ESC.current_resolution * duty_;
}
}
