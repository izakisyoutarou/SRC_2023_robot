#include "can_error_checker.hpp"

extern "C" {
#include "layer_driver/device/cmsis_lib/include/stm32f4xx_can.h"
#include "layer_driver/device/stm32f4_config/name_converter.h"
}

extern "C" void NVIC_Configuration(int);

namespace can_error_checker {

const char* str(ErrorStatus status) {
	switch(status) {
	case ErrorStatus::error_active:
		return "ErrorActive";
	case ErrorStatus::error_warning:
		return "ErrorWarning";
	case ErrorStatus::error_passive:
		return "ErrorPassive";
	case ErrorStatus::busoff:
		return "BusOff";
	default:
		return "undefiend ErrorStatus";
	}
}

const char* str(CAN_TypeDef * const type) {
	if(type == CAN1) {
		return "Can0";
	}
	else if(type == CAN2) {
		return "Can1";
	}
	else {
		return "undefined can type";
	}
}

void configureNVIC(CAN_TypeDef * const type) {
	if(type == CAN1) {
		NVIC_Configuration(CAN1_SCE_IRQn);
	}
	else if(type == CAN2) {
		NVIC_Configuration(CAN2_SCE_IRQn);
	}
}

template<class CanX> CanErrorChecker<CanX>::CanErrorChecker(Serial &serial): serial(serial){}

template<class CanX> int CanErrorChecker<CanX>::setup() {
	configureNVIC(can_type);
	CAN_ITConfig(can_type, CAN_IT_ERR, ENABLE);
	CAN_ITConfig(can_type, CAN_IT_LEC, ENABLE);
	return 0;
}

template<class CanX> void CanErrorChecker<CanX>::cycle() {
	constexpr std::array<const char*, 8> errorStr = {"NoError", "Stuff Error", "From Error", "ACK Error", "Bit Recessive Error", "Bit Dominant Error", "CRC Error", "Software Set Error"};
	
	is_occursed_error |= write_index != 0;
	for(int i = 0; i < write_index; i++) {
		errorCount[errorCode[i]]++;
	}
	write_index = 0;
	
	if(is_occursed_error && is_disp_error_code) {
		serial.printf("%s: Error\n", str(can_type));
		for(int i = 0; i < (int)errorCount.size(); i++) {
			if(errorCount[i] != 0) {
				serial.printf("  %s: %d\n", errorStr[i], errorCount[i]);
			}
		}
		serial.printf("  ReceiveErrorCount:%u TransmitErrorCount:%u\n", CAN_GetReceiveErrorCounter(can_type), CAN_GetLSBTransmitErrorCounter(can_type));
		is_occursed_error = false;
		errorCount.fill(0);
	}

	if((uint8_t)can_type->ESR & (uint8_t)CAN_ESR_EWGF) {
		errorStatus = ErrorStatus::error_warning;
	}
	else if((uint8_t)can_type->ESR & (uint8_t)CAN_ESR_EPVF) {
		errorStatus = ErrorStatus::error_passive;
	}
	else if((uint8_t)can_type->ESR & (uint8_t)CAN_ESR_BOFF) {
		errorStatus = ErrorStatus::busoff;
	}
	else {
		errorStatus = ErrorStatus::error_active;
	}
	
	if(changeErrorStatus(errorStatus) && is_disp_error_status) {
		serial.printf("%s: Transition to %s\n", str(can_type), str(errorStatus));
	}
}

template<class CanX> void CanErrorChecker<CanX>::interruptSCE() {
	if(CAN_GetFlagStatus(can_type, CAN_FLAG_LEC) != RESET) {
		if(write_index < (int)errorCode.size()) {
			errorCode[write_index++] = CAN_GetLastErrorCode(can_type) >> 4;
		}
		CAN_ClearITPendingBit(can_type, CAN_IT_LEC);
	}
}

template class CanErrorChecker<Can0>;
template<> CAN_TypeDef * const CanErrorChecker<Can0>::can_type = CAN1;
extern "C" void CAN1_SCE_IRQHandler(void) { CanErrorChecker<Can0>::interruptSCE(); }

template class CanErrorChecker<Can1>;
template<> CAN_TypeDef * const CanErrorChecker<Can1>::can_type = CAN2;
extern "C" void CAN2_SCE_IRQHandler(void) {	CanErrorChecker<Can1>::interruptSCE(); }

}
