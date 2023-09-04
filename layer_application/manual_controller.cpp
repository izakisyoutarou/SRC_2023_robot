#include "manual_controller.hpp"

#include "config.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
#include "stm32f4xx.h"
using namespace rp_lib;
using namespace cfg;

int ManualController::setup(){
    int i = 0;
    i +=controller.setup() !=0;
    i +=three_omni.setup() !=0;
    i +=padora_arm.setup() !=0;
    i +=elevating_arm.setup() !=0;
    i +=hand_valve.setup() !=0;
    i +=stock_valve.setup() !=0;
    i +=padora_valve.setup() !=0;

    return i;
}

void ManualController::chassis_mode(){
    if(controller.getDigitalR1()){
        cmode = cMode::slow;
    }else{
        cmode = cMode::nomal;
    }
}

void ManualController::arm_mode(){
    if(controller.getDigitalL1()){
        amode = aMode::manual;
    }else{
        amode = aMode::automatic;
    }
}

void ManualController::cycle(){



    if(controller.getDigitalSelect()){
        printf("reset\n");
        NVIC_SystemReset();
        cmode = cMode::reset;
        //足回りクラスのリセット
        three_omni.restart();

    }

    if(controller.getDigitalStart()){
        //アームクラスのキャリブレーション
        padora_arm.calibrate();
    }

    //threeOmni
    //スティックでオムニを動かす
    //左側のスティックの制御、スティックの倒したときのx,yの値をもとにモーター出力を計算し、move_velに格納される
    //エラーチェックのためprintfを書いている（気にしなくてよい）
    //計算されたモーター出力の値(move_vel)と右側のスティックcontroller.ry()をthree_omniクラスに渡している。
    //右スティックcontroller.ry()は回転運動(左右)にしか動かさないため複雑な計算は必要ない
    //基本的方法としてはR1ボタンを押しながらスティックを動かす
    if(cmode == cMode::nomal){
        //printf("nomal\n");
        TwoVectorf move_vel(controller.getValueLx(), controller.getValueLy());
        three_omni.move(move_vel, controller.getValueRy());
    }    
        //モーター出力をslowで低速にする
    else if(cmode == cMode::slow){
        //printf("slow");
        TwoVectorf move_vel(controller.getValueLx()/omni::deceleration, controller.getValueLy()/omni::deceleration);
        three_omni.move(move_vel, controller.getValueRy());
    }
        

    //アームを動かす制御
    //automaticは最上、最低などのあらかじめ設定された位置に自動的に移動する
    //ボタンを押すごとにcountが増えていき、それぞれのカウントに応じてpos制御をする
     if(amode == aMode::automatic && padora_arm.calibrate_flag()){

         if(padoraup(controller.getDigitalDown())&&count<3){
             count++;
             printf("flag");
         }
         else if(padoradown(controller.getDigitalUp())&&count>0){
             count--;
         }
         if(count==0){
             padora_arm.setting_move_top();
         }else if(count==1){
        	 padora_arm.setting_move_control();
         }else if(count==2){
             padora_arm.setting_move_middle();
         }else if(count==3){
             padora_arm.setting_move_bottom();
         }
         padora_arm.move();
     }


    

    if(controller.getDigitalRight()){
        elevating_arm.moveup();
    }else if(controller.getDigitalLeft()){
        elevating_arm.movedown();
    }else{
        elevating_arm.stop();
    }


    //〇ボタンを押すとhandのエアシリンダーが動く
    if(upCircle(controller.getDigitalCircle())){
        hand_valve.toggle();
    }
    //×ボタンを押すとhandのエアシリンダーが動く
    if(upCross(controller.getDigitalCross())){
        padora_valve.toggle();
    }
    //□ボタンを押すとhandのエアシリンダーが動く
    if(upRectangle(controller.getDigitalRectangle())){
        stock_valve.toggle();
    }
    controller.cycle();
    elevating_arm.cycle();
    padora_arm.cycle();
    three_omni.cycle();
    chassis_mode();
    arm_mode();
}

void ManualController::calbration(){
   padora_arm.calibrate();
   elevating_arm.calibrate();
}

