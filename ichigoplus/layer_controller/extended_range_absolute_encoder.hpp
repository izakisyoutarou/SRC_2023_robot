#pragma once
/*******************************************
 * extended_range_absolute_encoder ver1.0 2022/02/04
 * This program extend range of absolute encoder
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.0 2022/02/04 The first version
 ******************************************/

#include "ichigoplus/layer_driver/base/encoder.hpp"

namespace encoder {
class ExtendedRangeAbsoluteEncoder : public Encoder {
public:
	ExtendedRangeAbsoluteEncoder(Encoder &enc);
	int setup() override;
	void cycle() override;
	void rev(bool rev) override { is_reverse = rev; }
	float radian() const override { return radian_; }

private:
	Encoder &enc;

	bool is_reverse = false;
	float radian_ = 0.f;
	float old_enc_radian = 0.f;
	int rotation_count = 0;
};
}