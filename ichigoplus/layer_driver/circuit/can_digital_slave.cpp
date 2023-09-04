#include "can_digital_slave.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
#include <limits.h>

using namespace can::can_id::digital;
using can::CanId;
using can::CanDLC;
using can::CanData;
using rp_lib::uchar4_to_int;
using rp_lib::min;

CanDigitalSlave::CanDigitalSlave(Can &can, CanId instance_id, CanId node_id, Digital *d0, Digital *d1, Digital *d2, Digital *d3, Digital *d4, Digital *d5, Digital *d6, Digital *d7)
	:setup_pin_mode_id(calcId(instance_id, node_id, data_type_id::setup_pin_mode)),
	change_sending_mode_id(calcId(instance_id, node_id, data_type_id::change_sending_mode)),
	input_port_value_id(calcId(instance_id, node_id, data_type_id::input_port_value)),
	output_port_value_id(calcId(instance_id, node_id, data_type_id::output_port_value)),
	receivingId({setup_pin_mode_id, change_sending_mode_id, output_port_value_id}),
	digital{d0, d1, d2, d3, d4, d5, d6, d7} {

	can.addHandler(this);
}

int CanDigitalSlave::setup() {
	if(is_setuped) return 0;
	else if(canSetup()) return 1;

	for(const auto &id : receivingId) {
		canSetId(id);
	}

	is_setuped = true;
	return 0;
}

void CanDigitalSlave::cycle() {
	if(is_sending && sendingCycle()) {
		CanData data[8] = {0};
		for(int i = 0; i < 8; i++) {
			data[0] |= digital[i]->digitalRead() << i;
		}
		canWrite(input_port_value_id, can_digital::input_port::dlc, data);
	}
}

int CanDigitalSlave::canId(CanId id) {
	for(const auto &id_ : receivingId) {
		if(id == id_) return 1;
	}
	return 0;
}

int CanDigitalSlave::canRead(CanId id, CanDLC dlc, CanData data[8]) {
	if(id == setup_pin_mode_id) {
		if(dlc == can_digital::setup::check_communicate::dlc) {
			CanData data[8] = {0};
			data[0] = can_digital::setup::check_communicate::response;
			canWrite(setup_pin_mode_id, can_digital::setup::check_communicate::dlc, data);
			for(int i = 0; i < 8; i++) {
				sending_period[i] = 0;
			}
		}
		else if(dlc == can_digital::setup::setup_port::dlc) {
			using data_ = can_digital::setup::setup_port::data;
			for(int i = 0; i < 8; i++) {
				if(data[0] & (0x01 << i)) {
					if(data[1] == data_::in) {
						digital[i]->setupDigitalIn();
						setupMode[i] = SetupMode::in;
					}
					else if(data[1] == data_::in_pullup) {
						digital[i]->setupDigitalInPullUp();
						setupMode[i] = SetupMode::in_pullup;
					}
					else if(data[1] == data_::in_pulldown) {
						digital[i]->setupDigitalInPullDown();
						setupMode[i] = SetupMode::in_pulldown;
					}
					else if(data[1] == data_::out) {
						digital[i]->setupDigitalOut();
						setupMode[i] = SetupMode::out;
					}
				}
			}
		}
	}
	else if(id == change_sending_mode_id && dlc == can_digital::change_sending_mode::dlc) {
		CanData i = data[0];
		if(i >= 0 && i < 8) {
			if(setupMode[i] == SetupMode::in || setupMode[i] == SetupMode::in_pullup || setupMode[i] == SetupMode::in_pulldown) {
				sending_period[data[0]] = uchar4_to_int(&data[1]);
			}
		}
	}
	else if(id == output_port_value_id && dlc == can_digital::output_port::dlc) {
		for(int i = 0; i < 8; i++) {
			if(setupMode[i] == SetupMode::out) {
				digital[i]->digitalWrite((data[0] & (0x01 << i)) != 0x00);
			}
		}
	}

	if(id == setup_pin_mode_id || id == change_sending_mode_id) {
		int min_period = INT_MAX;
		for(int i = 0; i < 8; i++) {
			if(sending_period[i] > 0 && (setupMode[i] == SetupMode::in || setupMode[i] == SetupMode::in_pullup || setupMode[i] == SetupMode::in_pulldown)) {
				min_period = min(min_period, sending_period[i]);
			}
		}
		if(min_period != INT_MAX) {
			is_sending = true;
			sendingCycle(min_period, true);
		}
		else {
			is_sending = false;
		}
	}

	return 0;
}