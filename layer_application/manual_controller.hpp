#pragma once

#include "../ichigoplus/layer_driver/circuit/sbdbt.hpp"
#include "../ichigoplus/lib_src/utilplus.hpp"
#include "../ichigoplus/layer_driver/circuit/sbdbt.hpp"
#include "../ichigoplus/layer_driver/circuit/valve.hpp"
#include "../ichigoplus/layer_driver/circuit/bno055.hpp"
#include "layer_controller/three_omni.hpp"
#include "layer_controller/padora_arm.hpp"
#include "layer_controller/Elevating_arm.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

class ManualController{
    using ThreeOmni = three_omni::ThreeOmni;
public:
    ManualController(Sbdbt &controller, ThreeOmni &three_omni, Padora_arm &padora_arm, Elevating_arm &elevating_arm, Valve &hand_valve, Valve &stock_valve, Valve &padora_valve)
        :controller(controller),three_omni(three_omni),padora_arm(padora_arm),elevating_arm(elevating_arm),hand_valve(hand_valve),stock_valve(stock_valve),padora_valve(padora_valve){};

    int setup();
    void chassis_mode();
    void arm_mode();
    void cycle();
    void calbration();

private:
    Sbdbt &controller;
    ThreeOmni &three_omni;
    Padora_arm &padora_arm;
    Elevating_arm &elevating_arm;
    Valve &hand_valve;
    Valve &stock_valve;
    Valve &padora_valve;
    enum class cMode {//足回り系のenum class
        setup,
        reset,
        nomal,
        slow
    } cmode = cMode::setup;
    enum class aMode {//ハンド系のenum class
        setup,
        reset,
        automatic,
        manual,
    } amode = aMode::setup;
    int elevating_arm_count = 0;
    bool is_calibrated = false;


    enum class Up_Down{
        up,
        down,
        stop,
    }up_down = Up_Down::stop;



    //ボタンを押している間は0を出力し続け、離すタイミングで1を出力する
    //使う場合はボタンの判定をしているif文の条件式の中に書き込む必要がある
    rp_lib::UpEdge upStart;
    rp_lib::UpEdge upPs;
    rp_lib::UpEdge upSelect;
    rp_lib::UpEdge upCircle;
    rp_lib::UpEdge upSquare;
    rp_lib::UpEdge upCross;
    rp_lib::UpEdge upRectangle;
    rp_lib::UpEdge valveTriangle;
    rp_lib::UpEdge upLeft;
    rp_lib::UpEdge upRight;
    rp_lib::UpEdge upR1;
    rp_lib::UpEdge upL1;
    rp_lib::UpEdge upL2;
    rp_lib::UpEdge padoraup;
    rp_lib::UpEdge padoradown;

    float lx = 0.f;
    float ly = 0.f;
    float rx = 0.f;
    float ry = 0.f;
    int count = 0;
};
