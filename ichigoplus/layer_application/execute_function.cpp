#include "execute_function.hpp"
#if EXECUTE_FUNCTION_ENABLE
using namespace std;

ExecuteFunction::ExecuteFunction(const char *command){
	strcpy(this->command, command);
}

int ExecuteFunction::commandRequest(int argc, char **argv){
	if(firstExecuteFlag){
		firstExecuteFlag = false;
		if(keyDuplicateFlag){
			consolePrintf((char*)"********** ExecuteFunction:Warning: Duplicate key ********** \n");			//このエラーが出る場合は関数登録時(addFunc実行時)の文字列(関数呼び出し用)が重複している
		}
		if(consoleNotDefineFlag){
			consolePrintf((char*)"********** ExecuteFunction:Warning: console is not define ********** \n");	//これが出る場合は、merge関数を実行する前にmerge元(大体main)のExecuteFunctionクラスのオブジェクトをaddCommandする
		}
	}
	if(argc > 1){
		if(!strcmp(argv[1],"help")){
			for(const auto &func : funcMap){
				consolePrintf((char*)"%s\n",func.first.data());
			}
		}
		else if(argc == 2){
			if(funcMap.find(argv[1]) != funcMap.end()) funcMap.at(argv[1])();
			else consolePrintf((char*)"command error\n");
		}
		else if(argc == 3){
			if(funcMap.find(argv[1]) != funcMap.end()){
				funcCursor = funcMap.at(argv[1]);
				sustain = true;
				return 1;
			}
			else consolePrintf((char*)"command error\n");
		}
		else{
			consolePrintf((char*)"command error\n");
		}
	}
	else{
		consolePrintf((char*)"command error\n");
	}
	return 0;
}

void ExecuteFunction::addFunc(const char* funcName, function<void()> func){
	if(funcMap.count(funcName) > 0) keyDuplicateFlag = true;
	funcMap.insert(make_pair(funcName, func));
}

void ExecuteFunction::merge(ExecuteFunction &exeFunc){
	if(getConsole() == 0){
		consoleNotDefineFlag = true;
	}
	else{
		exeFunc.setConsole(getConsole());
	}
	exeFunc.mergeRequest();
	keyDuplicateFlag |= exeFunc.keyDuplicateFlag;
	consoleNotDefineFlag |= exeFunc.consoleNotDefineFlag;
	for(const auto &func : exeFunc.funcMap){
		if(funcMap.count(func.first.data()) > 0){
			keyDuplicateFlag = true;
		}
		else{
			funcMap.insert(func);
		}
	}
}

void ExecuteFunction::commandName(const char* command) {
	strcpy(this->command, command);
}

#endif //USE_EXECUTE_FUNCTION
