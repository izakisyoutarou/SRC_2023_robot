#pragma once
#define EXECUTE_FUNCTION_ENABLE true
//#define
/*******************************************
 * ExecutueFunction ver1.6 2020/08/26
 * This program make debug a console to easy
 * [Dependency]
 * Console 1.2 and over
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.7 2021/07/16 add commandName() to set the command name. delete static assert
 * ver1.6 2020/08/06 fix a bug that firstExecuteFlag
 * ver1.5 2020/01/07 bug fixes about EXECUTE_FUNCTION_ENABLE
 * ver1.4 2019/12/04 add mergeRequest for initialize console
 * ver1.3 2019/12/03 Remove extra functions. add macro for enable
 * ver1.2 2019/12/02 delete cycle and setup, add static_assert
 * ver1.1 2019/10/20 add function:merge,getFuncMap,getKeyDuplicateFlag,getConsoleNotDefineFlag, add variables, split file
 * ver1.0 2019/10/20 The first version
 *
 ******************************************/
#include <string>
#include <string.h>
#include <map>
#include <functional>
#include <vector>
#include "ichigoplus/layer_application/console.hpp"
using fMap = std::map<std::string, std::function<void()>>;

#if EXECUTE_FUNCTION_ENABLE
class ExecuteFunction: public ConsoleCommand{
public:
	ExecuteFunction(const char* command = "func");
	void addFunc(const char* funcName, std::function<void()> func);
	void merge(ExecuteFunction& exeFunc);
	void commandName(const char* command);

	virtual void mergeRequest(){}

private:
	int commandCompare(char* command){return !strcmp(this->command, command);}
	int commandRequest(int argc, char** argv);
	int commandCycle(){funcCursor();return (int)sustain;};
	void consoleReadChar(char data){sustain=false;}

	char command[11];
	bool sustain=false;
	fMap funcMap;
	std::function<void()> funcCursor;
	bool keyDuplicateFlag=false;
	bool consoleNotDefineFlag=false;
	bool firstExecuteFlag=true;

	std::vector<int> vec;
};
#else
class ExecuteFunction: public ConsoleCommand{
public:

	ExecuteFunction(const char *command = "func"){}
	void addFunc(const char* funcName, std::function<void()> func){}
	void merge(ExecuteFunction &exeFunc){}

	virtual void mergeRequest(){};

	int commandCompare(char *command){return false;}
	int commandRequest(int argc, char **argv){return false;}
	int commandCycle(){return false;}
	void consoleReadChar(char data){}

private:
	char command[11];
	fMap funcMap;
	bool keyDuplicateFlag=false;
	bool consoleNotDefineFlag=false;
	bool firstExecuteFlag=false;
};
#endif //USE_EXECUTE_FUNCTION