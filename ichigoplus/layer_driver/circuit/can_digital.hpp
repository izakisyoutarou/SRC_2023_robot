#pragma once
/*******************************************
 * can_digital ver1.0 2021/5/5
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

#include "ichigoplus/layer_driver/base/can.hpp"
#include "can_id.hpp"
#include "ichigoplus/layer_driver/base/digital.hpp"
#include <vector>

class CanDigital : public CanHandler {
private:
	class PinState {
	public:
		enum class SetupMode {
			null,
			digital_in,
			digital_in_pullup,
			digital_in_pulldown,
			digital_out
		} setupMode = SetupMode::null;

		enum class SetupTask {
			null,
			digital_in,
			digital_in_pullup,
			digital_in_pulldown,
			digital_out
		} setupTask = SetupTask::null;

		enum class OperationTask {
			null,
			change_sending_mode,
			write
		} operationTask = OperationTask::null;

		int slave_sending_period = 0;
		bool io_value = 0;
	};

	int canId(int id) override { return canId(static_cast<can::CanId>(id)); }
	int canId(can::CanId id);
	int canRead(int id, int dlc, unsigned char data[8]) override { return canRead(static_cast<can::CanId>(id), static_cast<can::CanDLC>(dlc), static_cast<can::CanData*>(data)); }
	int canRead(can::CanId id, can::CanDLC dlc, can::CanDLC data[8]);

	const int bus_check_timeout = 100;
	const can::CanId setup_pin_mode_id;			// send from master to slave
	const can::CanId change_sending_mode_id;		// send from master to slave
	const can::CanId input_port_value_id;			// send from slave to master
	const can::CanId output_port_value_id;		// send from master to slave
	const std::vector<can::CanId> receivingId;

	PinState pinState[8];

	bool is_setuped = false;
	bool is_id_setted = false;
	bool is_bus_checked = false;

public:
	class PinInterface final : public Digital {
	public:
		PinInterface(PinState &pinState): pinState(pinState) {};

		int setupDigitalIn() override;
		int setupDigitalInPullUp() override;
		int setupDigitalInPullDown() override;
		int setupDigitalOut() override;

		void startDigitalRead(int period);				// period: Period in which IO node sends data. unit: ms
		void stopDigitalRead();
		int digitalRead() override;
		void digitalWrite(int value) override;
		void digitalHigh() override;
		void digitalLow() override;
		void digitalToggle() override;

	private:
		// disable
		int _digitalWrite() override { return 0; }
		void _digitalWrite(int value) override {}
		int _digitalRead() override { return 0; }
		int _setupDigitalIn() override { return 1; }
		int _setupDigitalOut() override { return 1; }
		int _setupDigitalInPullUp() override { return 1; }
		int _setupDigitalInPullDown() override { return 1; }

		PinState &pinState;
	};


	CanDigital(Can &can, can::CanId instance_id , can::CanId node_id);
	int setup();
	void cycle();

	PinInterface pin0;
	PinInterface pin1;
	PinInterface pin2;
	PinInterface pin3;
	PinInterface pin4;
	PinInterface pin5;
	PinInterface pin6;
	PinInterface pin7;
};

namespace can_digital {
	namespace setup {
		namespace check_communicate {
			enum data : can::CanData {
				request,
				response
			};
			enum : can::CanDLC {
				dlc = 1
			};
		}

		namespace setup_port {
			enum data : can::CanData {
				in,
				in_pullup,
				in_pulldown,
				out
			};
			enum : can::CanDLC {
				dlc = 2
			};
		}
	}

	namespace input_port {
		enum : can::CanDLC {
			dlc = 1
		};
	}

	namespace output_port {
		enum : can::CanDLC {
			dlc = 2
		};
	}

	namespace change_sending_mode {
		enum : can::CanDLC {
			dlc = 5
		};
	}
}
