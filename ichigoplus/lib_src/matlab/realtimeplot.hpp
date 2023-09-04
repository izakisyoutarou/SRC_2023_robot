#pragma once
/*******************************************
 * realtimeplot ver1.0 2021/2/23
 * This program send binary for matlab realtimeplot.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.0 2021/2/23 The first version
 ******************************************/

#include "ichigoplus/layer_driver/base/serial.hpp"
#include "ichigoplus/layer_application/timer.hpp"
#include <vector>
#include <initializer_list>
#include <deque>
#include <string>
#include <type_traits>

namespace rp_lib {
namespace matlab {
namespace realtime_plot {

template <typename T>
struct VariableType {
	static_assert(std::is_same<T, int>::value || std::is_same<T, float>::value ,"invalid template parameter");
};

template <>
struct VariableType<int> {
	const char *name = "int";
};

template <>
struct VariableType<float> {
	const char *name = "float";
};

class AbstructVariable {
public:
	virtual void send(Serial &serial) const = 0;
	virtual const char * type() const = 0;
};

template <typename T>
class Variable : public AbstructVariable {
public:
	Variable(T var) : var(var) {}
	Variable() {}
	void send(Serial &serial) const override {
		for(int i = 0; i < (int)sizeof(T); i++) {
			const char *c = (char*)(&var);
			serial.writeChar(*(c+i));
		}
	}
	const char * type() const override { return type_.name; }
	void operator=(const T var) { this->var = var; }
	operator T() const { return var; }
	operator T&() { return var; }
private:
	T var = 0;
	const VariableType<T> type_;
};

class RealTimePlot : SerialHandler {
public:
	RealTimePlot(Serial &serial, std::initializer_list<AbstructVariable*> vars): serial(serial), vars(vars) {}
	RealTimePlot(Serial &serial, std::vector<AbstructVariable*> vars): serial(serial), vars(vars) {}
	int setup(int baudrate = 115200);
	void cycle();

private:
	void serialReadChar(char c) override;
	bool isContain(const std::string &str);

	enum class Status {
		setup,
		running
	} status = Status::setup;
	int task = 0;

	Serial &serial;
	std::vector<AbstructVariable*> vars;
	std::deque<char> buffer;

	struct {
		const std::string format = "format\n";
		const std::string ok = "ok\n";
		const std::string stop = "stop\n";
	} request;

	Timer timeoutTimer;
	const int timeout = 1000;
};

}
}
}