#include "padora_arm.hpp"
#include "config.hpp"
using namespace cfg;

int Padora_arm::setup(){
    int i = 0;
    i += mc.setup() != 0;
    i += base_photo.setup() !=0;
    return i;
}

void Padora_arm::calibrate(){
    //printf("calpad\n");
    calibration_flag = true;
    progress = 0;
}

void Padora_arm::stop(){
    mc.duty(arm::stop_duty);
}

void Padora_arm::manual(){
    mode = Mode::manual;
	duty_target = arm::nomal_duty;
}
//アームの位置が一番高いところ
void Padora_arm::setting_move_top(){
    mode = Mode::move_pos;
    pos_target = arm::top_pos;
}
//アームの位置が中間
void Padora_arm::setting_move_middle(){
    mode = Mode::move_pos;
    pos_target = arm::middle_pos;
}
void Padora_arm::setting_move_control(){
	mode = Mode::move_pos;
	pos_target = arm::control_pos;
}

//アームの位置が一番下
void Padora_arm::setting_move_bottom(){
    mode = Mode::move_pos;
    pos_target = arm::bottom_pos;
}

void Padora_arm::move(){
    if(mode == Mode::setup){
        stop();
    }else if(mode == Mode::manual){
        mc.duty(duty_target);
        if(base_photo.read()==0){
            stop();
        }
    }else if(mode == Mode::move_pos){
        mc.pos(pos_target);
    }
}

void Padora_arm::cycle(){

    if(calibration_flag==true) {
    	
       if(progress == 0){
           base_photo.startReading(arm::photoint::sampling_period);
           timer(arm::photoint::start_sampling_delay);
           printf("%d", progress);
           progress=1;
       }
       else if(progress == 1 && base_photo.read()==1 && timer()){
            mc.vel(arm::nomal_vel);
            printf("%d", progress);
            progress=2;
        }
        else if(progress == 2 && base_photo.read() == 0){
            mc.initPos(0.f);
            //mc.stop();
            mc.pos(0.f);
            // base_photo.stopReading();
            printf("%d", progress);
            progress = 1;
            calibration_flag=false;
            move_flag = true;
        }


    }
    mc.cycle();

}

bool Padora_arm::calibrate_flag(){
    return move_flag;
}
