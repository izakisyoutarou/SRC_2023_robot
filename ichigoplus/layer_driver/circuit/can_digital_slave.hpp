#pragma once
/*******************************************
 * can_digital_slave ver1.0 2021/5/5
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.0 2021/5/5 The first version
 ******************************************/

#include "ichigoplus/layer_application/timer.hpp"
#include "can_digital.hpp"

class CanDigitalSlave : public CanHandler {
public:
	CanDigitalSlave(Can &can, can::CanId instance_id, can::CanId node_id, Digital *d0, Digital *d1, Digital *d2, Digital *d3, Digital *d4, Digital *d5, Digital *d6, Digital *d7);
	int setup();
	void cycle();

private:
	enum class SetupMode {
		null,
		in,
		in_pullup,
		in_pulldown,
		out
	};

	int canId(int id) override { return canId(static_cast<can::CanId>(id)); }
	int canId(can::CanId id);
	int canRead(int id, int dlc, unsigned char data[8]) override { return canRead(static_cast<can::CanId>(id), static_cast<can::CanDLC>(dlc), static_cast<can::CanData*>(data)); }
	int canRead(can::CanId id, can::CanDLC dlc, can::CanData data[8]);

	const can::CanId setup_pin_mode_id;			// send from master to slave
	const can::CanId change_sending_mode_id;		// send from master to slave
	const can::CanId input_port_value_id;			// send from slave to master
	const can::CanId output_port_value_id;		// send from master to slave
	const std::vector<can::CanId> receivingId;

	Digital *digital[8];
	SetupMode setupMode[8] = {SetupMode::null, SetupMode::null, SetupMode::null, SetupMode::null, SetupMode::null, SetupMode::null, SetupMode::null, SetupMode::null};

	Timer sendingCycle;
	bool is_sending = false;
	int sending_period[8] = {0};

	bool is_setuped = false;
	bool is_bus_checked = false;
};