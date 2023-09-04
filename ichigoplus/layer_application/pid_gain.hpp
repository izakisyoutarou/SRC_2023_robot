#pragma once
/*******************************************
 * pid_gain ver2.0
 * This program r/w pid gain
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Merge into ichigoplus.
 * ver1.0 2021/08/13 The first version.
 ******************************************/

#include "ichigoplus/layer_controller/pid.hpp"
#include "ichigoplus/layer_application/console.hpp"
#include <map>
#include <string>
#include <cstring>

namespace console {

class PidGain: public ConsoleCommand {
	using Pid = siso_controller::Pid;
public:
	PidGain(const char* command_name = "pid") { this->commandName(command_name); }
	void add(const char* pid_name, Pid *pid) { this->pidMap.insert(std::make_pair(pid_name, pid)); }
	void add(const char* pid_name, Pid &pid) { add(pid_name, &pid); }

	void commandName(const char* command_name) { std::strcpy(this->command_name, command_name); }
	int commandCompare(char *command) override { return !std::strcmp(command, command_name); }
	int commandRequest(int argc,char **argv) override;

private:
	std::map<std::string, Pid*> pidMap;
	
	char command_name[11];
};

}