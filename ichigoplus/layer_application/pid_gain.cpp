#include "pid_gain.hpp"

namespace console {

int PidGain::commandRequest(int argc,char **argv) {
	if(argc > 1) {
		if(!strcmp(argv[1], "help")) consolePrintf((char*)"usage: <command> <pid> [<arg>]\n== command ==\n  show: show pid\n  show <arg>: show gain  \n  kp: set Kp\n  ki: set Ki\n  kd: set Kd\n");
		else if(!strcmp(argv[1], "show")) {
			if(argc == 2) {
				consolePrintf((char*)"pid list\n");
				for(const auto &pidPair : pidMap) {
					consolePrintf((char*)"  %s\n", pidPair.first.c_str());
				}
			}
			else if(argc == 3) {
				if(pidMap.find(argv[2]) != pidMap.end()) {
					auto pid = pidMap.at(argv[2]);
					consolePrintf((char*)"Kp:%f  Ki:%f  Kd:%f\n", pid->kp(), pid->ki(), pid->kd());
				}
				else {
					consolePrintf((char*)"%s: pid \"%s\" is not found\n", command_name, argv[2]);		
				}
			}
			else consolePrintf((char*)"%s: command error\n", command_name);
		}
		else if(!strcmp(argv[1], "kp")) {
			if(argc > 2) {
				if(pidMap.find(argv[2]) != pidMap.end()) {
					if(argc == 3) {
						consolePrintf((char*)"%s commnad error. Please enter a kp value.\n", command_name);
					}
					else if(argc == 4) {
						auto pid = pidMap.at(argv[2]);
						pid->kp(strtof(argv[3], NULL));
					}
					else {
						consolePrintf((char*)"%s: command error\n", command_name);
					}
				}
				else {
					consolePrintf((char*)"%s: pid \"%s\" is not found\n", command_name, argv[2]);		
				}
			}
			else consolePrintf((char*)"%s: command error\n", command_name);
		}
		else if(!strcmp(argv[1], "ki")) {
			if(argc > 2) {
				if(pidMap.find(argv[2]) != pidMap.end()) {
					if(argc == 3) {
						consolePrintf((char*)"%s commnad error. Please enter a ki value.\n", command_name);
					}
					else if(argc == 4) {
						auto pid = pidMap.at(argv[2]);
						pid->ki(strtof(argv[3], NULL));
					}
					else {
						consolePrintf((char*)"%s: command error\n", command_name);
					}
				}
				else {
					consolePrintf((char*)"%s: pid \"%s\" is not found\n", command_name, argv[2]);		
				}
			}
			else consolePrintf((char*)"%s: command error\n", command_name);
		}
		else if(!strcmp(argv[1], "kd")) {
			if(argc > 2) {
				if(pidMap.find(argv[2]) != pidMap.end()) {
					if(argc == 3) {
						consolePrintf((char*)"%s commnad error. Please enter a kd value.\n", command_name);
					}
					else if(argc == 4) {
						auto pid = pidMap.at(argv[2]);
						pid->kd(strtof(argv[3], NULL));
					}
					else {
						consolePrintf((char*)"%s: command error\n", command_name);
					}
				}
				else {
					consolePrintf((char*)"%s: pid \"%s\" is not found\n", command_name, argv[2]);		
				}
			}
			else consolePrintf((char*)"%s: command error\n", command_name);
		}
		else {
			consolePrintf((char*)"%s: command \"%s\" is not found\n", command_name, argv[1]);
		}
	}
	else {
		consolePrintf((char*)"%s: missing operand. Try %s help\n", command_name, command_name);
	}
	return 0;
}

}