#pragma once
#include "robomaster_esc.hpp"
#include "ichigoplus/layer_driver/base/encoder.hpp"
#include "ichigoplus/layer_driver/circuit/motor_driver.hpp"

namespace robomaster {
class GM6020ESC : public RobomasterESC{
public:
	GM6020ESC(const int number, const int period_ms=2, const bool count=true, const bool rpm=false, const bool torque=false);

    encoder::Encoder &encoder(){return (encoder::Encoder&)encoder_; }
    MotorDriver &motor(){return (MotorDriver&)motor_;}

private:
    //エンコーダ
    class Encoder : public encoder::Encoder{
    public:
        Encoder(GM6020ESC &gm6020ESC) : gm6020ESC(gm6020ESC){};
        int setup() override;
    	void cycle() override;
    	void rev(bool rev) override { is_reverse = rev; };
    	float radian() const override { return radian_; };

        float radianVel() const {return radian_vel_;}
        int torque() const{return torque_;}

    private:
        GM6020ESC &gm6020ESC;
    	bool is_reverse = false;

    	float radian_ = 0.f;
        float radian_vel_ = 0.f;
        float torque_ = 0;
    };

    //モータ
    class Motor : public MotorDriver{
    public:
        Motor(GM6020ESC &gm6020ESC) : gm6020ESC(gm6020ESC){};
        int setup()override;
	    void cycle()override;
	    void outRev(bool rev)override{is_reverse=rev;}
	    void outRev()override{is_reverse=!is_reverse;}
	    void duty(float value)override{duty_=value;}
	    float duty()override{return duty_;}
	    void free()override{}
    private:
        GM6020ESC &gm6020ESC;

        bool is_reverse = false;
	    float duty_ = 0.f;
    };

    Encoder encoder_;
    Motor motor_;
    const int count_resolution = 8191;
	const int current_resolution = 30000;

};
}
