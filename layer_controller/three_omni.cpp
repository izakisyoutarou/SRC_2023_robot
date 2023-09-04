#include "layer_controller/three_omni.hpp"
#include "ichigoplus/layer_driver/circuit/bno055.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
#include <math.h>
//utilを使うときに書く↓（unitl=math.h）
using namespace rp_lib;

namespace three_omni {

    int ThreeOmni::setup() {
        int i = 0;
        //auto…左辺の情報を確認して適切な型に自動的に調整
        //ラムダ式の中身…setup関数の処理を行う、成功すれば0、失敗したら1が帰ってくる、0でなかった時1加算する
        //その値をみてエラーが出てないか確認
        //setup関数は上手くいかないとiに1を返すようにプログラムされている
        auto func = [&](int n){i += n != 0;};
        func(omni0.setup());
        func(omni1.setup());
        func(omni2.setup());
        func(gyro.setup());
        return i;
    }

    void ThreeOmni::cycle() {
        //移動方向、角度、並進速度、回転速度の計算
        float angle = gyro.angleZ();//+f_pi;
        omni0.vel(move_direction, angle, move_vel, ang_vel);
        omni1.vel(move_direction, angle, move_vel, ang_vel);
        omni2.vel(move_direction, angle, move_vel, ang_vel);

        //上記の3つの関数の中で最も値が大きい物を算出する
        float motor_vel_max = max({omni0.vel(), omni1.vel(), omni2.vel()});

        //velの大きさがmotor_vel_limitを超える場合に回転を優先しつつ、移動方向が変わらない様にする
        if(motor_vel_max > motor_vel_limit){
            float abs_ang_vel = std::abs(omni0.ang_vel);
            if(abs_ang_vel > motor_vel_limit) {
                omni0.vel(0.f, omni0.ang_vel * motor_vel_limit / abs_ang_vel);
                omni1.vel(omni0);
                omni2.vel(omni0);
            }
            else{
                float gain = (motor_vel_limit - abs_ang_vel) / (motor_vel_max - abs_ang_vel);
                omni0.move_vel *= gain;
                omni1.move_vel *= gain;
                omni2.move_vel *= gain;
            }
        }

        //omniクラスの実行
        omni0.cycle();
        omni1.cycle();
        omni2.cycle();
        gyro.cycle();
    }

    void ThreeOmni::move(TwoVector vel, float ang_vel) {

        //printf("move\n");
        //コントローラから受け取った情報をmove_direction、move_vel、ang_velに代入してcycleで出力する
        this->move_direction = vel.angle();
        this->move_vel = constrain(vel.length(), -move_vel_limit, move_vel_limit);
        this->ang_vel = constrain(ang_vel, -ang_vel_limit, ang_vel_limit);

    
    }



}
