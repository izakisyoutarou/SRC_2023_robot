#pragma once

/*******************************************
 * can id protcol.
 *
 * [Note]
 *
 *
 * [Author]
 * Yuta Uehara
 ******************************************/

#include <cstdint>

namespace can {
	using CanId = uint32_t;
	using CanDLC = uint8_t;
	using CanData = uint8_t;

	// CAN ID
	namespace can_id {

		// 0x000~0x0FF. CAN communication status.
		namespace can_status {
			enum : CanId {
				can_emergency_stop_id = 0
			};
		}

		// 0x100~0x1FF. CAN motor driver.
		namespace md {

		}

		// 0x200~0x2FF. Odrive36.
		namespace odrive36 {
            CanId calcId(CanId node_id);
            enum : CanId{
                base_id = 0x200,
                increment_id = 0x20
            };
            enum {
                max_node_id = 7
            };
		}

		// 0x300~0x3FF. Robomasetr IO.
		namespace robomaster_io {
            CanId calcSendId(CanId node_id);
            CanId calcReceptionId(CanId node_id);
            enum : CanId{
                send_base_id = 0x300,
                reception_base_id = 0x380,
                increment_id = 0x10
            };
            enum {
                max_node_id = 7
            };
		}

		// 0x400~0x47F. CAN encoder.
		namespace encoder {

		}

		// 0x480~0x48F. abs absolute encoder.
		namespace amt_absolute_encoder {
            CanId calcId(CanId node_id);
            enum : CanId{
                base_id = 0x480
            };
            enum {
                max_node_id = 14 //1基板5ポートなので3枚
            };
		}

		// 0x500~0x5F0. CAN dIgital.
		// 0bit: instance_bits. 1~4bit: node_bits. 5~6bit: data_id
		namespace digital {
			CanId calcId(CanId instance_id, CanId node_id, CanId data_id);
			enum {
				base_id = 0x500
			};
			namespace instance_id {
				enum : CanId {
					instance_bits = 1,			// Number of bits used to identify the instance of the node.
					node_bits = 4,				// Number of bits used to identify the node.
					data_id_bits = 2			// Number of bits used to identify the type of data.
				};
			}
			namespace data_type_id {
				enum : CanId {
					setup_pin_mode,				// send from master to slave
					change_sending_mode,		// send from master to slave
					input_port_value,			// send from slave to master
					output_port_value			// send from master to slave
				};
			}
			namespace error_id {
				enum : CanId {
					invalid_id = 0x5F0
				};
			}
		}

		// 0x600~0x6FF. Character Display
		namespace charactor_display {

		}
	}
}
