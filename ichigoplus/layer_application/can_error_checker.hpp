#pragma once
/*******************************************
 * can_error_checker ver1.0 2021/09/20
 * This program check Can Error.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Refactoring. Merge into ichigoplus
 * ver1.0 2021/09/20 The first version
 ******************************************/

#include <array>
#include "layer_driver/device/cmsis_boot/stm32f4xx.h"
#include "ichigoplus/layer_driver/base/serial.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

namespace can_error_checker {

enum class ErrorStatus { error_active, error_warning, error_passive, busoff };
const char* str(ErrorStatus status);

template<class CanX>
class CanErrorChecker {
public:
	CanErrorChecker(Serial &serial);
	int setup();
	void cycle();
	void dispErrorCode(bool enable) { is_disp_error_code= enable; }
	void dispErrorStatus(bool enable) { is_disp_error_status = enable; }
	ErrorStatus getErrorStatus() { return errorStatus; }
	static void interruptSCE();
private:
	Serial &serial;
	static bool is_disp_error_code;
	static bool is_disp_error_status;
	static bool is_occursed_error;
	static std::array<uint8_t, 256> errorCode;
	static std::array<int, 8> errorCount;
	static int write_index;
	static ErrorStatus errorStatus;
	static rp_lib::Change<ErrorStatus> changeErrorStatus;
	static CAN_TypeDef * const can_type;
};
template<class CanX> bool CanErrorChecker<CanX>::is_disp_error_code = false;
template<class CanX> bool CanErrorChecker<CanX>::is_disp_error_status = false;
template<class CanX> bool CanErrorChecker<CanX>::is_occursed_error = false;
template<class CanX> std::array<uint8_t, 256> CanErrorChecker<CanX>::errorCode = {0};
template<class CanX> std::array<int, 8> CanErrorChecker<CanX>::errorCount = {0};
template<class CanX> int CanErrorChecker<CanX>::write_index = 0;
template<class CanX> ErrorStatus CanErrorChecker<CanX>::errorStatus = ErrorStatus::error_active;
template<class CanX> rp_lib::Change<ErrorStatus> CanErrorChecker<CanX>::changeErrorStatus;

class Can0;
class Can1;

}