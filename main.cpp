//application
#include "ichigoplus/layer_application/cycle_checker.hpp"
#include "ichigoplus/layer_application/cycle_once_checker.hpp"
#include "ichigoplus/layer_application/timer.hpp"
#include "ichigoplus/layer_application/console.hpp"
#include "ichigoplus/layer_application/execute_function.hpp"
#include "layer_application/manual_controller.hpp"

//controller
#include "ichigoplus/layer_controller/brush_motor_pos_vel_controller.hpp"
#include "ichigoplus/layer_controller/trapezoidal_velocity_planner.hpp"
#include "ichigoplus/layer_controller/photo_interrputer.hpp"
#include "ichigoplus/layer_controller/filtered_encoder.hpp"
#include "ichigoplus/layer_controller/position_pid.hpp"
#include "layer_controller/three_omni.hpp"
#include "layer_controller/Elevating_arm.hpp"
#include "layer_controller/padora_arm.hpp"

//circuit
#include "ichigoplus/layer_driver/circuit/lap_motor_driver.hpp"
#include "ichigoplus/layer_driver/circuit/sbdbt.hpp"
#include "ichigoplus/layer_driver/circuit/aqm0802.hpp"
#include "ichigoplus/layer_driver/circuit/rotary_sw.hpp"
#include "ichigoplus/layer_driver/circuit/digital_valve.hpp"
#include "ichigoplus/layer_driver/circuit/bno055.hpp"

//lib_src
#include "ichigoplus/lib_src/utilplus.hpp"

//device
#include "layer_driver/device/pin.hpp"

#include "config.hpp"

using TraVelPlanner = velocity_planner::trapezoidal_velocity_planner::TrapezoidalVelocityPlanner;
using TraVelPlannerLimit = velocity_planner::trapezoidal_velocity_planner::Limit_t;
using PhotoInt = photo_interrputer::PhotoInterrputerDigital;
using encoder::Enc0;
using encoder::Enc1;
using encoder::Enc2;
using encoder::Enc3;
using namespace cfg;
using namespace rp_lib;

int main(){
	// cycle period [ms]
	constexpr int ctrl_period = 2;
	constexpr int disp_period = 100;

	//LED
	LED0 led0;
	led0.setupDigitalOut();
	led0.digitalToggle();
	LED1 led1;
	led1.setupDigitalOut();
	LED2 led2;
	led2.setupDigitalOut();
	LED3 led3;
	led3.setupDigitalOut();
	LED5 led5;
	led5.setupDigitalOut();

	//md
	Md0l md0l;
	Md0r md0r;
	Md1l md1l;
	Md1r md1r;
	Md2l md2l;
	Md2r md2r;
	Md3l md3l;
	Md3r md3r;
	Md4l md4l;
	Md4r md4r;
	Md5l md5l;
	Md5r md5r;

	//足回り、アーム、stockのモータープログラム
	//右回り、左回り、dutyの周波数をomni_mdに引数として持たせている
	LAPMotorDriver omni_md0(md0l, md0r, 100);
	LAPMotorDriver omni_md1(md1l, md1r, 100);
	LAPMotorDriver omni_md2(md2l, md2r, 100);
	LAPMotorDriver padora_arm_md(md3l, md3r, 100);
	LAPMotorDriver Elevating_arm_md(md4l, md4r, 100);


	//Valve
	Valve0 valve0; valve0.setupDigitalOut();
	Valve1 valve1; valve1.setupDigitalOut();
	Valve2 valve2; valve2.setupDigitalOut();

	//digitalValve
	DigitalValve hand_valve(valve0);
	// hand_valve.setup();
	hand_valve.turnOver(true);
	DigitalValve stock_valve(valve1);
	// stock_valve.setup();
	stock_valve.turnOver(true);
	DigitalValve padora_valve(valve2);
	// padora_valve.setup();
	padora_valve.turnOver(true);

	// Serial
	Serial0 forCons;
	Serial1 serialGyro;
	Serial2 forPscon;

	// Console
	//teratermに値を表示するために設定
	//serial 0番を指定
	Console cons(forCons);
	//周波数
	cons.setup(115200);
	//teratermに表示させるためのwindowsの設定
	cons.setNewLine(Console::NEWLINE_CRLF);

	// ExecuteFunction
	ExecuteFunction exeFunc("func");
	cons.addCommand(exeFunc);

	// Gyro
	Bno055 gyro(serialGyro);
	gyro.setup();
	gyro.rev(true);

	// Sbdbt
	Sbdbt psCon(forPscon);
	// psCon.setup();

	//Photo
	//フォトイントラプタクラスの宣言、全部で3つ
	Photo0 photo0; 
	photo0.setupDigitalOut();
	Photo1 photo1; 
	photo1.setupDigitalOut();
	Photo2 photo2; 
	photo2.setupDigitalOut();

	//PhotoInt
	//それぞれのフォトイントラプタにphotoの値を格納
	PhotoInt Elevating_top_photo(photo0, PhotoInt::SetupType::in);
	PhotoInt Elevating_bottom_photo(photo1, PhotoInt::SetupType::in);
	PhotoInt padora_base_photo(photo2, PhotoInt::SetupType::in);
	
	Elevating_top_photo.startReading(2);
	Elevating_bottom_photo.startReading(2);

	// Encoder
	Enc0 enc0;
	enc0.cpr(4000);
	Enc1 enc1;	
	enc1.cpr(4000);
	Enc2 enc2;
	enc2.cpr(4000);
	Enc3 enc3;
	enc3.cpr(4000);

	//Filtered encoder
	//それぞれのエンコーダーの情報をomni_encに格納している
	encoder::FilteredEncoder omni_enc0(enc3);
	// omni_enc0.setup();
	omni_enc0.rev(false);
	encoder::FilteredEncoder omni_enc1(enc2);
	// omni_enc1.setup();
	omni_enc1.rev(false);
	encoder::FilteredEncoder omni_enc2(enc0);
	// omni_enc2.setup();
	omni_enc2.rev(false);
	encoder::FilteredEncoder padora_enc(enc1);
	// padora_enc.setup();
	padora_enc.rev(false);

	//TravelPlanner
	////omni_tvpl、arm_tvplにconfigクラスで宣言したそれぞれのlimitの値を入れる
	const TraVelPlannerLimit omni_tvpl(cfg::omni::pos_limit,cfg::omni::vel_limit,cfg::omni::acc_limit,cfg::omni::dec_limit);
	const TraVelPlannerLimit arm_tvpl(cfg::arm::pos_limit_max,cfg::arm::vel_limit,cfg::arm::acc_limit,cfg::arm::dec_limit);

	//tvpにomnni_tvplとarm_tvplに格納
	TraVelPlanner tvp0(omni_tvpl);
	TraVelPlanner tvp1(omni_tvpl);
	TraVelPlanner tvp2(omni_tvpl);
	TraVelPlanner tvp3(arm_tvpl);

	//siso
	//pidにconfigクラスで宣言したgainの値を入れる
	siso_controller::PositionPid pid0; pid0.gain(cfg::omni::pos_gain, cfg::omni::vel_gain, cfg::omni::acc_gain);
    siso_controller::PositionPid pid1; pid1.gain(cfg::omni::pos_gain, cfg::omni::vel_gain, cfg::omni::acc_gain);
    siso_controller::PositionPid pid2; pid2.gain(cfg::omni::pos_gain, cfg::omni::vel_gain, cfg::omni::acc_gain);
    siso_controller::PositionPid pid3; pid3.gain(cfg::arm::pos_gain, cfg::arm::vel_gain, cfg::arm::acc_gain);

	//BrushMotorposvelcontroller
	//omni_mc0にomni_md,omni_enc,tvp,pidの値を全て格納
	BrushMotorPosVelController omni_mc0(omni_md0, omni_enc0, tvp0, pid0);
	omni_mc0.outRev(true);
	BrushMotorPosVelController omni_mc1(omni_md1, omni_enc1, tvp1, pid1);
	omni_mc1.outRev(true);
	BrushMotorPosVelController omni_mc2(omni_md2, omni_enc2, tvp2, pid2);
	omni_mc2.outRev(true);
	BrushMotorPosVelController arm_mc(padora_arm_md, padora_enc, tvp3, pid3);
	arm_mc.outRev(true);
	arm_mc.commandName("amc");
	cons.addCommand(arm_mc);

	//three_omni
	auto initMc = [&](BrushMotorPosVelController &mc) {
		using namespace cfg::omni;
		mc.rotateRatio(1.f, gear_ratio);
		mc.limit(pos_limit, vel_limit, acc_limit, dec_limit);
		mc.limitDuty(duty_limit_min, duty_limit_max);
	};

	//tera termでpidやpos,mdの値をみる
	//gainの値やモーターの出力をその場で変更・確認することができる
	//ExecuteFunctionを書くのがめんどくさいのでこのような使い方をすると全てのセンサーやモーターの値を知ることができる
	//今回はcommandNameを打つことでそれぞれのオムニの情報を知ることができる
	initMc(omni_mc0);
	omni_mc0.commandName("omni_mc0");
	cons.addCommand(omni_mc0);
	initMc(omni_mc1);
	omni_mc1.commandName("omni_mc1");
	cons.addCommand(omni_mc1);
	initMc(omni_mc2);
	omni_mc2.commandName("omni_mc2");
	cons.addCommand(omni_mc2);

	const three_omni::OmniLimit omniLimit(omni_tvpl.pos, omni_tvpl.vel, omni_tvpl.acc, omni_tvpl.dec);
	three_omni::Omni omni0(omni_mc0, cfg::omni::radius,cfg::omni::attached_radius, cfg::omni::omni0_angle,omniLimit);
	three_omni::Omni omni1(omni_mc1, cfg::omni::radius,cfg::omni::attached_radius, cfg::omni::omni1_angle,omniLimit);
	three_omni::Omni omni2(omni_mc2, cfg::omni::radius,cfg::omni::attached_radius, cfg::omni::omni2_angle,omniLimit);

	three_omni::ThreeOmni three_omni(omni0, omni1, omni2, gyro,cfg::omni::move_vel_limit,cfg::omni::rotate_vel_limit);

	//Padora_arm
	Padora_arm padora_arm(arm_mc,padora_base_photo);
	// padora_arm.setup();

	//Elevating_arm
	Elevating_arm elevating_arm(Elevating_arm_md,Elevating_top_photo,Elevating_bottom_photo);
	//elevating_arm.setup();

	//manualController
	//manualControllerのそれぞれの部分の情報を格納したオブジェクトを引数として持たせる
	//それをcycleで回すことで実行できる
	ManualController manualcontroller(psCon,three_omni,padora_arm,elevating_arm,hand_valve,stock_valve,padora_valve);
	manualcontroller.setup();
	// printf("error:%d\n",elevating_arm.setup());
	// printf("error:%d\n",padora_arm.setup());
	// printf("error:%d\n",three_omni.setup());

	// Cycle Timer
	Timer ctrlCycle;
	ctrlCycle(ctrl_period, true);
	Timer dispCycle;
	dispCycle(disp_period, true);

	// CycleChecker
	CycleChecker cycleChecker(ctrl_period);

	// CycleCounter
	cycle_once_checker::CycleCounter cycleCounter;

	// ExecuteFunction(add func)
	exeFunc.addFunc("reset", [&]{ NVIC_SystemReset(); });
	exeFunc.addFunc("cal",[&]{manualcontroller.calbration();});
	exeFunc.addFunc("calpad",[&]{padora_arm.cycle();});
	exeFunc.addFunc("calelevate",[&]{elevating_arm.calibrate();});
	exeFunc.addFunc("gyro",[&]{ printf("angleZ:%f\n", rtod(gyro.angleZ())); });
	exeFunc.addFunc("photostart",[&]{Elevating_top_photo.startReading(arm::photoint::sampling_period);
										Elevating_bottom_photo.startReading(arm::photoint::sampling_period);
										padora_base_photo.startReading(arm::photoint::sampling_period);});
	exeFunc.addFunc("photo", [&] { printf("top: %d  bottom: %d padora:%d\n", Elevating_top_photo.read(),
									Elevating_bottom_photo.read(),padora_base_photo.read()); });
	exeFunc.addFunc("enc",[&]{printf("enc0:%3f enc1:%3f enc2:%3f arm_enc:%3f\n",omni_enc0.radian(),omni_enc1.radian(),omni_enc2.radian(), padora_enc.radian());});
	exeFunc.addFunc("hand",[&]{hand_valve.toggle();});
	exeFunc.addFunc("stock",[&]{stock_valve.toggle();});
	exeFunc.addFunc("padora",[&]{padora_valve.toggle();});
	exeFunc.addFunc("md0",[&]{omni_md0.duty(0.3);});
	exeFunc.addFunc("md1",[&]{omni_md1.duty(0.3);});
	exeFunc.addFunc("md2",[&]{omni_md2.duty(0.3);});
	exeFunc.addFunc("padoarm",[&]{arm_mc.duty(0.3);});
	exeFunc.addFunc("eduty",[&]{printf("duty: %f \n",Elevating_arm_md.duty());});
	exeFunc.addFunc("elevate",[&]{Elevating_arm_md.duty(0.3);});//+が下ーが上
	exeFunc.addFunc("up",[&]{elevating_arm.moveup();});
	exeFunc.addFunc("dn",[&]{elevating_arm.movedown();});
	exeFunc.addFunc("pos",[&]{printf("pos:%3f\n",arm_mc.pos());});
	exeFunc.addFunc("restart",[&]{three_omni.restart();});
	exeFunc.addFunc("conL",[&]{printf("x: %f y: %f\n",psCon.getValueLx(), psCon.getValueLy());});
	exeFunc.addFunc("conR",[&]{printf("x: %f y: %f\n",psCon.getValueRx(), psCon.getValueRy());});
	exeFunc.addFunc("sankaku",[&]{printf("%d",psCon.getDigitalTriangle());});
	exeFunc.addFunc("a",[&]{printf("%f,%f\n",omni_mc2.currentTargetVel(),omni_mc2.vel());});
	exeFunc.addFunc("s",[&]{printf("%f,%f,%f\n",omni_mc0.duty(),omni_mc1.duty(),omni_mc2.duty());});
	exeFunc.addFunc("stick",[&]{printf("omni0:%f,omni1:%f,omni2:%f\n",omni_mc0.vel(),omni_mc1.vel(),omni_mc2.vel());});

	// main loop
	while(1){
		if(ctrlCycle()) {
			cycleChecker.cycle();
			cycleCounter.cycle();


			// padora_enc.cycle();
			// padora_arm_md.cycle();

//			 Elevating_arm_md.cycle();
//			arm_mc.cycle();
//			padora_arm.cycle();
//			omni_mc0.cycle();
//			omni_mc1.cycle();
//			omni_mc2.cycle();

//			  padora_arm.cycle();
			//   elevating_arm.cycle();
			// three_omni.cycle();

			manualcontroller.cycle();


			if(psCon.connection()) {
				led2.digitalToggle();
			}
			if(psCon.l3()) {
				led0.digitalHigh();
			}

		}

		if(dispCycle()) {
			if(cycleChecker()){
				forCons.printf("cycle was delayed : %lld[ms]\n",cycleChecker.getMaxDelay());
				cycleChecker.reset();
			}
			cons.cycle();
			led5.digitalToggle();
		}

	}

}
