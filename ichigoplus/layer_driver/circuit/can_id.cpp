#include "can_id.hpp"

namespace can {
namespace can_id {

	static constexpr CanId pow(CanId x, CanId y) {		// x^y
		if(y > 0) return pow(x,y-1)*x;
		else return static_cast<CanId>(1);
	}

    namespace odrive36{
        CanId calcId(CanId node_id){
            if(node_id > max_node_id){
                return base_id | increment_id * max_node_id;
            }
            return base_id | increment_id * node_id;
        }
    }

    namespace robomaster_io{
        CanId calcSendId(CanId node_id){
            if(node_id > max_node_id){
                return send_base_id | increment_id * max_node_id;
            }
            return send_base_id | increment_id * node_id;
        }
        CanId calcReceptionId(CanId node_id){
            if(node_id > max_node_id){
                return reception_base_id | increment_id * max_node_id;
            }
            return reception_base_id | increment_id * node_id;
        }
    }

    namespace amt_absolute_encoder{
        CanId calcId(CanId node_id){
            if(node_id > max_node_id){
                return base_id | max_node_id;
            }
            return base_id | node_id;
        }
    }

	namespace digital {
		static constexpr CanId max_instance_id = pow(2, instance_id::instance_bits) - 1;
		static constexpr CanId max_node_id = pow(2, instance_id::node_bits) - 1;
		static constexpr CanId max_data_id = pow(2, instance_id::data_id_bits) - 1;

		CanId calcId(CanId instance_id, CanId node_id, CanId data_id) {
			if(instance_id <= max_instance_id && node_id <= max_node_id && data_id <= max_data_id) {
				const CanId data_id_ = data_id << (instance_id::instance_bits + instance_id::node_bits);
				const CanId node_id_ = node_id << instance_id::instance_bits;
				return base_id | data_id_ | node_id_ | instance_id;
			}
			else return error_id::invalid_id;
		}
	}

}
}
