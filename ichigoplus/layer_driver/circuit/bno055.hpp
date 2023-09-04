#pragma once
/*******************************************
 * BNO055 ver1.4 2021/02/10
 *
 * [Dependency]
 * serial
 * GyroSensor
 * mcutime
 * math
 *
 * [Note]
 *
 * [Author]
 * Shoma Nakaaki
 *
 * [Change history]
 * ver1.5 2021/03/16 Avoid stopping programs when not setup
 * ver1.4 2021/02/10 Change the value used for conversion
 * ver1.3 2021/02/05 angleZ value:0~2PI->-無限~無限
 * ver1.2 2021/01/29 remove function (read only Yaw, Calibration, etc.)
 * ver1.1 2021/ 1/28 bug fix.
 * ver1.0 2021/ 1/26 release.
 * ver0.1 2020/ 3/   The first version.Test.
 ******************************************/

//angVel limit : 2000deg/s

#include "ichigoplus/layer_driver/base/serial.hpp"
#include "ichigoplus/layer_driver/base/gyro_sensor.hpp"

class Bno055: public SerialHandler, public GyroSensor{
public:
	Bno055(Serial &serial);

    int setup() override;
    void cycle() override;

	/*シリアル受信するたびに呼び出される*/
	void serialReadChar(char value);


private:

	char read_baff[17];//もっと増えるかも

	/* 角速度読むための時間 */
	int64_t time_vel;

	/* 現在の角度 */
	float imu_rad_yaw;
	float imu_rad_roll;
	float imu_rad_pitch;

	/* 1つ前に受信した角度 */
	float imu_rad_yaw_old;
	float imu_rad_roll_old;
	float imu_rad_pitch_old;

	int roop_count;

	bool setup_done;

	int read_count;//読み込んだ回数

	char send_data[8];

    int64_t time_read_cycle;

	enum bno055_reg_t{
	    /* Page id register definition */
	    BNO055_PAGE_ID_ADDR = 0X07,

	    /* PAGE0 REGISTER DEFINITION START*/
	    BNO055_CHIP_ID_ADDR = 0x00,

	    /* Euler data registers */
	    BNO055_EULER_H_LSB_ADDR = 0X1A,

	    /* Unit selection register */
	    BNO055_UNIT_SEL_ADDR = 0X3B,
	    BNO055_DATA_SELECT_ADDR = 0X3C,

	    /* Mode registers */
	    BNO055_OPR_MODE_ADDR = 0X3D,
	    BNO055_PWR_MODE_ADDR = 0X3E,
	    BNO055_SYS_TRIGGER_ADDR = 0X3F,


	} ;

	  /** BNO055 power settings */
	enum bno055_powermode_t{
	    POWER_MODE_NORMAL = 0X00,
	} ;

	  /** Operation mode settings **/
	enum bno055_opmode_t{
	    /* CONFIG */
		OPERATION_MODE_CONFIG = 0X00,


	    /* FUSION MODE */
	    OPERATION_MODE_IMUPLUS = 0X08,

	};


	bool checkChipId();
	/*IDチェック*/

	bool resetChip();
	/*チップをリセット*/

	bool setOpeMode(bno055_opmode_t mode);
	/*指定されたモードに変更*/

	bool setPowerMode(bno055_powermode_t mode);
	/*パワーモードの変更*/

	bool setPageMode(char page);
	/*ページの変更*/

	bool setExternalClk();
	/*外部クロックの設定*/

	bool accSetupOffset(char offset_data[6]);
	/*加速度のオフセットをセンサに送る*/

	bool gyroSetupOffset(char offset_data[6]);
	/*ジャイロのオフセットをセンサに送る*/

	bool setupOffset(char acc_offset_data[6],char gyr_offset_data[6]);
	/*内部でジャイロと加速度のオフセットをセンサに送る*/

	void imuDataRead();
	/*センサの値更新*/

	void readData(bno055_reg_t send_reg,int length);
	/*受信用関数*/

	void writeData(bno055_reg_t send_reg,char length,char data[8]);
	/*書き込み、モード変更用関数*/

};
