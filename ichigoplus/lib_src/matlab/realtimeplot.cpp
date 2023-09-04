#include "realtimeplot.hpp"

namespace rp_lib {
namespace matlab {
namespace realtime_plot {

int RealTimePlot::setup(int baudrate) {
	int i = 0;
	i += serialHandlerSetup(&serial) != 0;
	i += serialSetup(baudrate) != 0;
	return i;
}

void RealTimePlot::cycle() {
	if(status == Status::setup) {
		if(task == 0) {
			if(isContain(request.format)) {
				buffer.clear();
				task++;
			}
			while(buffer.size() > request.format.size()) buffer.pop_front();
		}
		else if(task == 1) {
			for(const auto &var : vars) {
				serial.printf(var->type());
				serial.printf(" ");
			}
			serial.printf("\n");
			task++;
			timeoutTimer(timeout);
		}
		else if(task == 2) {
			if(isContain(request.ok)) {
				buffer.clear();
				task = 0;
				status = Status::running;
			}
			else if(timeoutTimer()) {
				buffer.clear();
				task = 0;
				status = Status::setup;
			}
			while(buffer.size() > request.ok.size()) buffer.pop_front();
		}
	}
	else if(status == Status::running) {
		if(isContain(request.stop)) {
			buffer.clear();
			task = 0;
			status = Status::setup;
		}
		while(buffer.size() > request.stop.size()) buffer.pop_front();

		for(const auto &var : vars) {
			var->send(serial);
		}
	}
}

bool RealTimePlot::isContain(const std::string &str) {
	int i;
	for(i = 0; i < (int)buffer.size() && i < (int)str.size(); i++) {
		if(buffer[i] != str[i]) break;
	}
	return i == (int)str.size();
}

void RealTimePlot::serialReadChar(char c) {
	buffer.push_back(c);
}

}
}
}