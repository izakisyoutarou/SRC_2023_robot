#include "within_range_absolute_encoder.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

using rp_lib::area;
using rp_lib::f_pi;

namespace encoder{

float WithinRangeAbsoluteEncoder::radian(){
    return encoder.radian();
}
float WithinRangeAbsoluteEncoder::targetPos(){
    return area(encoder.radian(), positive_position-(2.f*f_pi), positive_position) * rotate_ratio;
}
void WithinRangeAbsoluteEncoder::rotateRatio(float encoder, float target){
    rotate_ratio = target / encoder;
}
}
