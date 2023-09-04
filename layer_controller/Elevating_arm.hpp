#pragma once

#include "ichigoplus/layer_driver/circuit/motor_driver.hpp"
#include "ichigoplus/layer_controller/photo_interrputer.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
using namespace rp_lib;

class Elevating_arm {
using PhotoInt = photo_interrputer::PhotoInterrputer;
public:
	Elevating_arm(MotorDriver &md, PhotoInt &top_photo, PhotoInt &bottom_photo)
        :md(md),top_photo(top_photo),bottom_photo(bottom_photo){};

	int setup();
    void calibrate();
    void stop();
    void moveup();
    void movedown();
	void cycle();
    void vertical_manual();


private:
	MotorDriver &md;
    PhotoInt &top_photo;
    PhotoInt &bottom_photo;
    Timer timer;
    int progress = 0;
    
    enum class ver_automatic{
        setup,
        automatic,
        
    }Ver_automatic = ver_automatic::setup;
     
    bool calibration_flag=false;
    
    
};
