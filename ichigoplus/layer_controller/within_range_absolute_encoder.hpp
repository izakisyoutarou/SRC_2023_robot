#pragma once

#include "ichigoplus/layer_driver/base/encoder.hpp"

namespace encoder{
class WithinRangeAbsoluteEncoder{
public:
	WithinRangeAbsoluteEncoder(Encoder &encoder, float positive_position)
    :encoder(encoder), positive_position(positive_position){};
	float radian();
    float targetPos();
    void rotateRatio(float encoder, float target);

private:
    Encoder &encoder;
    const float positive_position;
    float rotate_ratio = 1.f;
};
}
