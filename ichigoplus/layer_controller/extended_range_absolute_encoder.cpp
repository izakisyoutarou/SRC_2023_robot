#include "extended_range_absolute_encoder.hpp" //ver1.0
#include "ichigoplus/lib_src/utilplus.hpp"

using rp_lib::f_pi;

namespace encoder {

ExtendedRangeAbsoluteEncoder::ExtendedRangeAbsoluteEncoder(Encoder &enc)
:enc(enc) {
	enc.rev(false);
}

int ExtendedRangeAbsoluteEncoder::setup() {
	return enc.setup();
}

void ExtendedRangeAbsoluteEncoder::cycle() {
	enc.cycle();
	const float gap = enc.radian() - old_enc_radian;
	old_enc_radian = enc.radian();

	if(gap > f_pi) rotation_count--;
	else if(gap < -f_pi) rotation_count++;
	
	radian_ = (is_reverse ? -1 : 1) * (enc.radian() + rotation_count * 2.f * f_pi);
}

}