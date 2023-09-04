#include "can_digital.hpp" //ver1.0
#include "ichigoplus/layer_application/timer.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

using rp_lib::int_to_uchar4;

using can::can_id::digital::calcId;
using namespace can::can_id::digital;
using namespace can;

CanDigital::CanDigital(Can &can, CanId instance_id, CanId node_id)
	:setup_pin_mode_id(calcId(instance_id, node_id, data_type_id::setup_pin_mode)),
	change_sending_mode_id(calcId(instance_id, node_id, data_type_id::change_sending_mode)),
	input_port_value_id(calcId(instance_id, node_id, data_type_id::input_port_value)),
	output_port_value_id(calcId(instance_id, node_id, data_type_id::output_port_value)),
	receivingId({setup_pin_mode_id, input_port_value_id}),
	pin0(pinState[0]), pin1(pinState[1]), pin2(pinState[2]), pin3(pinState[3]), pin4(pinState[4]), pin5(pinState[5]), pin6(pinState[6]), pin7(pinState[7]) {

	can.addHandler(this);
}

int CanDigital::setup() {
	if(is_setuped) return 0;
	else if(canSetup()) return 1;

	if(!is_id_setted) {
		for(const auto &id : receivingId) {
			canSetId(id);
		}
	}

	CanData data[8] = {0};
	data[0] = can_digital::setup::check_communicate::data::request;
	canWrite(setup_pin_mode_id, can_digital::setup::check_communicate::dlc, data);

	Timer timer;
	timer(bus_check_timeout);
	while(!is_bus_checked && !timer());

	if(timer()) {
		return 1;
	}
	else {
		is_setuped = true;
		return 0;
	}
}

void CanDigital::cycle() {
	if(!is_bus_checked) return;

	// define lambda expression to setup.
	auto setupFunc = [&](PinState::SetupTask setupTask, CanData task_data, PinState::SetupMode mode) {
		CanData target_instance_id = 0x00;
		for(int i = 0; i < 8; i++) {
			if(pinState[i].setupTask == setupTask) {
				target_instance_id |= 0x01 << i;
				pinState[i].setupMode = mode;
				pinState[i].setupTask = PinState::SetupTask::null;
			}
		}
		if(target_instance_id != 0x00) {
			CanData data[8] = {0};
			data[0] = target_instance_id;
			data[1] = task_data;
			canWrite(setup_pin_mode_id, can_digital::setup::setup_port::dlc, data);
		}
	};
	setupFunc(PinState::SetupTask::digital_in,          can_digital::setup::setup_port::data::in,          PinState::SetupMode::digital_in          );
	setupFunc(PinState::SetupTask::digital_in_pullup,   can_digital::setup::setup_port::data::in_pullup,   PinState::SetupMode::digital_in_pullup   );
	setupFunc(PinState::SetupTask::digital_in_pulldown, can_digital::setup::setup_port::data::in_pulldown, PinState::SetupMode::digital_in_pulldown );
	setupFunc(PinState::SetupTask::digital_out,	        can_digital::setup::setup_port::data::out,         PinState::SetupMode::digital_out         );
	
	bool write_io_port_request = false;
	for(int i = 0; i < 8; i++) {
		// check task to change sending mode
		if(pinState[i].operationTask == PinState::OperationTask::change_sending_mode) {
			CanData data[8] = {0};
			data[0] = static_cast<CanData>(i);
			int_to_uchar4(&data[1], pinState[i].slave_sending_period);
			canWrite(change_sending_mode_id, can_digital::change_sending_mode::dlc, data);
			pinState[i].operationTask = PinState::OperationTask::null;
		}
		// check task to write io port 
		else if(pinState[i].operationTask == PinState::OperationTask::write) {
			write_io_port_request = true;
			pinState[i].operationTask = PinState::OperationTask::null;
		}
	}
	if(write_io_port_request) {
		CanData data[8] = {0};
		for(int i = 0; i < 8; i++) {
			data[0] |= pinState[i].io_value << i;
		}
		canWrite(output_port_value_id, can_digital::output_port::dlc, data);
	}
}

int CanDigital::canId(CanId id) {
	for(const auto &id_ : receivingId) {
		if(id == id_) return 1;
	}
	return 0;
}

int CanDigital::canRead(CanId id, CanDLC dlc, CanData data[8]) {
	if(id == setup_pin_mode_id && dlc == can_digital::setup::check_communicate::dlc) {
		if(data[0] == can_digital::setup::check_communicate::data::response) {
			is_bus_checked = true;
		}
	}
	else if(id == input_port_value_id && dlc == can_digital::input_port::dlc) {
		for(int i = 0; i < 8; i++) {
			using Mode = PinState::SetupMode;
			using namespace can_digital::output_port;
			auto &state = pinState[i];
			const auto &setupMode = state.setupMode;
			if((setupMode == Mode::digital_in || setupMode == Mode::digital_in_pullup || setupMode == Mode::digital_in_pulldown) && state.slave_sending_period != 0) {
				state.io_value = (data[0] & (0x01 << i)) != 0;
			}
		}
	}
	return 0;
}

int CanDigital::PinInterface::setupDigitalIn() { 
	if(pinState.setupMode != PinState::SetupMode::digital_in) {
		pinState.operationTask = PinState::OperationTask::null;
		pinState.setupTask = PinState::SetupTask::digital_in;
		pinState.io_value = 0;
	}
	return 0;
}

int CanDigital::PinInterface::setupDigitalInPullUp() { 
	if(pinState.setupMode != PinState::SetupMode::digital_in_pullup) {
		pinState.operationTask = PinState::OperationTask::null;
		pinState.setupTask = PinState::SetupTask::digital_in_pullup;
		pinState.io_value = 0;
	}
	return 0;
}

int CanDigital::PinInterface::setupDigitalInPullDown() {
	if(pinState.setupMode != PinState::SetupMode::digital_in_pulldown) {
		pinState.operationTask = PinState::OperationTask::null;
		pinState.setupTask = PinState::SetupTask::digital_in_pulldown;
		pinState.io_value = 0;
	}
	return 0;
}

int CanDigital::PinInterface::setupDigitalOut() {
	if(pinState.setupMode != PinState::SetupMode::digital_out) {
		pinState.operationTask = PinState::OperationTask::null;
		pinState.setupTask = PinState::SetupTask::digital_out;
		pinState.io_value = 0;
		pinState.slave_sending_period = 0;
	}
	return 0;
}

void CanDigital::PinInterface::startDigitalRead(int period) {
	using Mode = PinState::SetupMode;
	using Task = PinState::SetupTask;
	if((pinState.setupMode == Mode::digital_in || pinState.setupMode == Mode::digital_in_pullup || pinState.setupMode == Mode::digital_in_pulldown
		|| pinState.setupTask == Task::digital_in || pinState.setupTask == Task::digital_in_pullup || pinState.setupTask == Task::digital_in_pulldown) 
		&& pinState.slave_sending_period != period && period > 0) {

		pinState.operationTask = PinState::OperationTask::change_sending_mode;
		pinState.slave_sending_period = period;
	}
}

void CanDigital::PinInterface::stopDigitalRead() {
	using Mode = PinState::SetupMode;
	using Task = PinState::SetupTask;
	if((pinState.setupMode == Mode::digital_in || pinState.setupMode == Mode::digital_in_pullup || pinState.setupMode == Mode::digital_in_pulldown
		|| pinState.setupTask == Task::digital_in || pinState.setupTask == Task::digital_in_pullup || pinState.setupTask == Task::digital_in_pulldown)
		&& pinState.slave_sending_period > 0) {

		pinState.operationTask = PinState::OperationTask::change_sending_mode;
		pinState.slave_sending_period = 0;
	}
}

int CanDigital::PinInterface::digitalRead() {
	using Mode = PinState::SetupMode;
	using Task = PinState::SetupTask;
	if(pinState.setupMode == Mode::digital_in || pinState.setupMode == Mode::digital_in_pullup || pinState.setupMode == Mode::digital_in_pulldown
		|| pinState.setupTask == Task::digital_in || pinState.setupTask == Task::digital_in_pullup || pinState.setupTask == Task::digital_in_pulldown) {

		return pinState.io_value;
	}
	else {
		return 0;
	}
}

void CanDigital::PinInterface::digitalWrite(int value) {
	if((pinState.setupMode == PinState::SetupMode::digital_out || pinState.setupTask == PinState::SetupTask::digital_out)
		&& pinState.io_value != value) {
		pinState.operationTask = PinState::OperationTask::write;
		pinState.io_value = value;
	}
}

void CanDigital::PinInterface::digitalHigh() {
	digitalWrite(true);
}

void CanDigital::PinInterface::digitalLow() {
	digitalWrite(false);
}

void CanDigital::PinInterface::digitalToggle() {
	digitalWrite(!pinState.io_value);
}
