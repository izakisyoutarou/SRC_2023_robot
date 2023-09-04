#pragma once
#include "coord.hpp"
#include <stdint.h>
#include <complex>
#include <initializer_list>
/*******************************************
 * utilplus ver2.1 2020/11/25
 * This is a program made for mathematically calculation.
 * This is the first kitrp's microcomputer program that uses classes!
 * [Dependency]
 * util 1.1+
 *
 * [Note]
 *
 * [Author]
 * Tomoki Nagatani
 *
 * [Change history]
 * ver2.2 2022/02/24 Use cmath. add pi().
 * ver2.1 2020/11/25 add combination.
 * ver2.0 2020/11/04 Separate Coord, Merge util, Rename utilplus.hpp, Refactoring by Yuta Uehara
 * ver1.1 2016/ 2/ 15 Add PhysicalObject,Change,UpEdge,DownEdge,quadraticEquationSolver,cubicEquationSolver. RingBuffer fatal bug fix.RingBuffer add class support.
 * ver1.0 2015/ 2/ 6 Add revolution. Add operator overload for Coord. Add sign.Dependency update to util 1.1+.
 * ver0.2 2015/ 1/16 Addition of add functions.
 * ver0.1 2015/ 1/11 The first version.Constuctor ,read and write members available.
 ******************************************/

namespace rp_lib{

// math defines
static constexpr double d_pi = 3.1415926535897932384626433832795;
static constexpr float f_pi = (float)d_pi;

// Pi
template <class T>
constexpr T pi() { return d_pi; }
template <>
inline constexpr float pi() { return f_pi; }


using Coordf = rp_lib::Coord<float>;
using Coordd = rp_lib::Coord<double>;

template <typename lType, typename aType = lType>
class PhysicalObject {
public:
	Coord<lType, aType> position;
	Coord<lType, aType> velocity;
	Coord<lType, aType> acceleration;
	Coord<lType, aType> jerk;
	void gainTime(float seconds) { velocity = velocity + acceleration*seconds; position = position + velocity*seconds; };
};

template<typename T=char,int bufSize=256> class RingBuffer{
private:
	T data[bufSize+2];
	int readPointer;
	int writePointer;

public:
	RingBuffer(){
		readPointer=0;
		writePointer=0;
	};
	int write(T value){
		int next=(writePointer + 1) % (bufSize+1);
		if(readPointer==next)return 1;
		data[writePointer]=value;
		writePointer = next;
		return 0;
	}
	T& read(){
		if(readPointer==writePointer) return data[readPointer];
		int oldReadPointer=readPointer;
		readPointer = (readPointer + 1) % (bufSize+1);
		return data[oldReadPointer];
	}
	T& peek(){
		if(readPointer==writePointer) return data[readPointer];
		return data[readPointer];
	}
	T& peek(int pointer){
		if(pointer>=length()||pointer<0) return data[readPointer];
		int tReadPointer = (readPointer + pointer) % (bufSize+1);
		return data[tReadPointer];
	}
	void clear(){ readPointer = 0; writePointer = 0; };
	void remove(){
		if(isEmpty())return;
		if(writePointer>0)writePointer--;
		else writePointer+=bufSize;
	};
	int isEmpty(){return  (readPointer==writePointer)?(1):(0);}
	int isFull(){return (readPointer==((writePointer + 1) % (bufSize+1)))?(1):(0);}
	int size(){return bufSize;};
	int length(){
		if(writePointer>=readPointer) return writePointer-readPointer;
		else                          return bufSize-(readPointer-writePointer-1);
	}
};

template<typename T,int bufSize> class Recorder{
public:
	void write(T value,int64_t time){
		//Record force the first data.
		if(records.isEmpty()){
			Record record(value,time);
			records.write(record);
			return;
		}

		//Ignore same values.
		if(value==records.peek(records.length()-1).value){
			return;
		}

		//Make space to record a new data when the buffer is full.
		if(records.isFull()){
			records.read();
		}

		//Record
		Record record(value,time);
		records.write(record);
	}

	T& read(int64_t time){
		for(int i=records.length()-1;i>=0;i--){
			if(records.peek(i).time<=time){
				return records.peek(i).value;
			}
		}
		return records.peek(records.length()-1).value;
	}

	T smoothRead(int64_t time){
		for(int i=records.length()-1;i>=0;i--){
			if(records.peek(i).time<=time){
				if(records.length()-1<=i){
					//std::cout << "smoothRead end records" << std::endl;
					return records.peek(records.length()-1).value;
				}

				return (records.peek(i).value*(records.peek(i+1).time-time)
						+records.peek(i+1).value*(time-records.peek(i).time))
						/(records.peek(i+1).time-records.peek(i).time);
			}
		}
		//std::cout << "smoothRead invailed time" << std::endl;
		return records.peek(0).value;
	}
private:
	class Record{
	public:
		Record(){
		}
		Record(T value,int64_t time){
			this->value=value;
			this->time=time;
		}
		T value;
		int64_t time;
	};
	RingBuffer<Record,bufSize> records;
};

template <class T>
class Change {
public:
	bool operator()(T value) {
		if(is_first) {
			is_first = false;
			old = value;
			return false;
		}
		else if(value == old) {
			old = value;
			return false;
		}
		else {
			old = value;
			return true;
		}
	}
private:
	T old;
	bool is_first = true;
};

class UpEdge{
public:
	bool operator()(bool value){
		if(!oldValue && value){
			oldValue=value;
			return true;
		}else{
			oldValue=value;
			return false;
		}
	};
private:
	bool oldValue = true;
};

class DownEdge{
public:
	bool operator()(bool value){
		if(oldValue && !value){
			oldValue=value;
			return true;
		}else{
			oldValue=value;
			return false;
		}
	};
private:
	bool oldValue = false;
};

class Toggle{
public:
	int returnValue;
	UpEdge sw;
	Toggle(){returnValue=0;sw(true);}
	int operator()(bool value){
		if(sw(value)){
			returnValue=!returnValue;
		}
		return returnValue;
	};
	void operator=(bool value){
		returnValue=value;
	}
};

template <class T>
constexpr T dtor(T x) { return pi<T>()*x/180; }

template <class T>
constexpr T rtod(T x) { return x*180/pi<T>(); }

template <class T>
constexpr int sign(T val) {
	if(val>=0) return 1;
	else return -1;
}
inline constexpr int fsign(float val) { return sign(val); }

template <class T>
constexpr T constrain(T x, T min, T max) {
	if(x<min) return min;
	else if(max<x) return max;
	else return x;
}
inline constexpr int intlimit(int mae,int val,int ato) { return constrain(val, mae, ato); }
inline constexpr float floatlimit(float mae,float val,float ato) { return constrain(val, mae, ato); }

template <class T>
constexpr T area(T value,T shita,T ue) {
	int i=0,limit=100;
	if(shita>=ue) return 0;
	const T period = ue - shita;
	for(i=0;value<shita&&i<limit;i++) {value+=period;}
	for(i=0;ue<value&&i<limit;i++) {value-=period;}
	return value;
}

template <class T>
constexpr T max(T a,T b) {
	if(a>b) return a;
	else return b;
}
template <class T>
constexpr T max(std::initializer_list<T> list) {
	T max_value = *list.begin();
	for(const auto &value : list) {
		max_value = max(max_value, value);
	}
	return max_value;
}

template <class T>
constexpr T min(T a,T b) {
	if(a<b) return a;
	else return b;
}
template <class T>
constexpr T min(std::initializer_list<T> list) {
	T min_value = *list.begin();
	for(const auto &value : list) {
		min_value = min(min_value, value);
	}
	return min_value;
}

template <class T>
constexpr T absMax(T a,T b) {
	if(std::abs(a)>std::abs(b)) return a;
	else return b;
}
template <class T>
constexpr T absMax(std::initializer_list<T> list) {
	T max_value = *list.begin();
	for(const auto &value : list) {
		max_value = absMax(max_value, value);
	}
	return max_value;
}

template <class T>
constexpr T absMin(T a, T b) {
	if(std::abs(a)<std::abs(b)) return a;
	else return b;
}
template <class T>
constexpr T absMin(std::initializer_list<T> list) {
	T min_value = *list.begin();
	for(const auto &value : list) {
		min_value = absMin(min_value, value);
	}
	return min_value;
}

union Int8t_char{
    std::int8_t int_value;
    unsigned char char_value;
};
inline std::int8_t char_to_int8t(unsigned char *value){
    Int8t_char tmp;
    tmp.char_value = *value;
    return tmp.int_value;
}
inline void int8t_to_char(unsigned char *value, std::int8_t int_value){
    Int8t_char tmp;
    tmp.int_value = int_value;
    *value = tmp.char_value;
}

union Int_char4 {
	int int_value;
	unsigned char char4_value[4];
};
inline int uchar4_to_int(unsigned char *value) {
	Int_char4 tmp;
    tmp.char4_value[0]=value[0];
    tmp.char4_value[1]=value[1];
    tmp.char4_value[2]=value[2];
    tmp.char4_value[3]=value[3];
    return tmp.int_value;
}
inline void int_to_uchar4(unsigned char *value,int int_value){
	Int_char4 tmp;
	tmp.int_value=int_value;
	value[0]=tmp.char4_value[0];
	value[1]=tmp.char4_value[1];
	value[2]=tmp.char4_value[2];
	value[3]=tmp.char4_value[3];
}

union UShort_char2{
	unsigned short short_value;
	unsigned char char2_value[2];
};
inline unsigned short uchar2_to_ushort(unsigned char *value) {
	UShort_char2 tmp;
	tmp.char2_value[0]=value[0];
	tmp.char2_value[1]=value[1];
	return tmp.short_value;
}
inline void ushort_to_uchar2(unsigned char *value,unsigned short short_value) {
	UShort_char2 tmp;
	tmp.short_value=short_value;
	value[0]=tmp.char2_value[0];
	value[1]=tmp.char2_value[1];
}

union Float_char4 {
	float float_value;
	unsigned char char4_value[4];
};
inline float uchar4_to_float(unsigned char *value) {
	Float_char4 tmp;
	tmp.char4_value[0]=value[0];
	tmp.char4_value[1]=value[1];
	tmp.char4_value[2]=value[2];
	tmp.char4_value[3]=value[3];
	return tmp.float_value;
}
inline void float_to_uchar4(unsigned char *value,float float_value) {
	Float_char4 tmp;
	tmp.float_value=float_value;
	value[0]=tmp.char4_value[0];
	value[1]=tmp.char4_value[1];
	value[2]=tmp.char4_value[2];
	value[3]=tmp.char4_value[3];
}

union Short_char2{
    short short_value;
    unsigned char char2_value[2];
};
inline short uchar2_to_short(unsigned char *value){
    Short_char2 tmp;
    tmp.char2_value[0]=value[0];
    tmp.char2_value[1]=value[1];
    return tmp.short_value;
}
inline void short_to_uchar2(unsigned char * value, short short_value){
    Short_char2 tmp;
    tmp.short_value = short_value;
    value[0]=tmp.char2_value[0];
    value[1]=tmp.char2_value[1];
}


//a*x^2+b*x^1+c=0
inline float quadraticEquationSolverPlus(float a,float b,float c){
	if(b*b-4.0f*a*c<0.0f)return 0.0f;
	return (-b+std::sqrt(b*b-4.0f*a*c))/(2.0f*a);
}
inline float quadraticEquationSolverMinus(float a,float b,float c){
	if(b*b-4.0f*a*c<0.0f)return 0.0f;
	return (-b-std::sqrt(b*b-4.0f*a*c))/(2.0f*a);
}
int quadraticEquationSolver(float &x1,float &x1i,float &x2,float &x2i,float a,float b,float c);
int quadraticEquationSolver(std::complex<float> &x1,std::complex<float> &x2,float a,float b,float c);

//a*x^3+b*x^2+c*x^1+d=0
int cubicEquationSolver(std::complex<float> &x1,std::complex<float> &x2,std::complex<float> &x3 ,float a,float b,float c,float d);
int cubicEquationSolver(float &x1r,float &x1i,float &x2r,float &x2i,float &x3r,float &x3i ,float a,float b,float c,float d);

int combination(int n, int r);

}
