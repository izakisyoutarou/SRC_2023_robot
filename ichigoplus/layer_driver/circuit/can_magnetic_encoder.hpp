#pragma once
/*******************************************************************************************************************************************
 * can_magnetic_encoder ver3.0 2022/2/3
 * Class for using magnetic_encoder.
 * count is number of elements, default count is 4095.
 *
 * [Note]
 *
 * [Change history]
 * ver3.0  2022/  2/  3 Support New Encoder class And improve by Yuta Uehara
 * ver2.31 2021/  7/ 20 Fix return value in setup() by Keigo Shinomiya
 * ver2.3  2020/ 10/ 31 Fix include by Yuta Uehara
 * ver2.2  2020/ 10/ 30 Fix include by Yuta Uehara
 * ver2.1  2019/ 12/ 11 Fix Bug by Shoma Nakaaki
 * ver2.0  2019/ 12/ 10 Changed to send data at specified intervals by Shoma Nakaaki
 * ver1.2  2019/ 11/  5 Fix Bug by Sato Yoshifumi
 * ver1.11 2019/ 11/  5 Add minor modification, make lighter process of cycle by Shinomiya Keigo
 * ver1.1  2019/ 11/  2 Add functions, cycle and mirror. Mirror is count down system from 4095 to 0   by Hayashi Istuki and Shinomiya Keigo
 * ver1.0  2019/ 10/ 26 First version. Add functions for magnetic_encoder   by Sato Yoshihumi, Uehara Yuta and Shinomiya Keigo
 *******************************************************************************************************************************************/
#include "ichigoplus/layer_driver/base/can.hpp"
#include "ichigoplus/layer_driver/base/encoder.hpp"

#define CAN_MAGNETIC_ENCODER_ID 0x480

namespace encoder {
class CanMagneticEncoder : public CanHandler, public Encoder {
public:
    CanMagneticEncoder(Can &can, int number, int cpr_ = 4095);

    int setup() override;
    int setup(uint16_t send_time);
    void cycle() override;
    void rev(bool rev) override { is_reverse = rev; }
    float radian() const override { return radian_; }
    void cpr(int cpr) { cpr_ = cpr; }
    int count() const { return count_; }

    float degree() const { return degree_; }

	int canRead(int id, int number, unsigned char data[8]);
	int canId(int id);

private:
	int id;
	int bit = 0;
	float degree_ = 0.f;
	float radian_ = 0.f;
	bool is_reverse = false;
    int cpr_;
    int count_ = 0;
    bool is_receivable = false;
    bool is_can_setuped = false;
};
}
