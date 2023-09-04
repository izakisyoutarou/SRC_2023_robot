#pragma once
#include <cmath>

/*******************************************
 * Coord ver1.0 2020/11/04
 * This is coordinate class.
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Tomoki Nagatani
 *
 * [Change history]
 * ver1.1 2022/02/24 Use cmath
 * ver1.0 2020/11/04 Separate from util.hpp
 ******************************************/

namespace rp_lib{

template <typename lType, typename aType = lType>
class Coord {
protected:
	lType cartesianXData;
	lType cartesianYData;
	lType cartesianZData;
	aType angleXData;
	aType angleYData;
	aType angleZData;

public:
	Coord() {
		cartesianXData = 0;
		cartesianYData = 0;
		cartesianZData = 0;
		angleXData = 0;
		angleYData = 0;
		angleZData = 0;
	};
	Coord(Coord const &coord) {
		cartesianXData = coord.cartesianXData;
		cartesianYData = coord.cartesianYData;
		cartesianZData = coord.cartesianZData;
		angleXData = coord.angleXData;
		angleYData = coord.angleYData;
		angleZData = coord.angleZData;
	}
	Coord(lType cx, lType cy, lType cz) {
		cartesianXData = cx;
		cartesianYData = cy;
		cartesianZData = cz;
		angleXData = 0;
		angleYData = 0;
		angleZData = 0;
	}
	Coord(lType cx, lType cy, lType cz, aType ex, aType ey, aType ez) {
		cartesianXData = cx;
		cartesianYData = cy;
		cartesianZData = cz;
		angleXData = ex;
		angleYData = ey;
		angleZData = ez;
	}

	void cartesian(lType cx, lType cy, lType cz) {
		cartesianXData = cx;
		cartesianYData = cy;
		cartesianZData = cz;
	}
	lType cartesianX() {
		return cartesianXData;
	}
	lType cartesianY() {
		return cartesianYData;
	}
	lType cartesianZ() {
		return cartesianZData;
	}
	void cartesianX(lType x) {
		cartesianXData = x;
	}
	void cartesianY(lType y) {
		cartesianYData = y;
	}
	void cartesianZ(lType z) {
		cartesianZData = z;
	}
	void cartesianXAdd(lType x) {
		cartesianXData = x;
	}
	void cartesianYAdd(lType y) {
		cartesianYData = y;
	}
	void cartesianZAdd(lType z) {
		cartesianZData = z;
	}

	void polar(lType radius, aType theta, aType phi) {
		lType xy = 0;
		if (theta == 0) {
			cartesianXData = 0;
			cartesianYData = 0;
			cartesianZData = radius;
			return;
		}
		cartesianZData = radius*std::cos(theta);
		xy = radius*std::sin(theta);
		cartesianXData = xy*std::cos(phi);
		cartesianYData = xy*std::sin(phi);
	}
	lType polarRadius() {
		lType radius;
		//    float theta;
		//    float phi;
		radius = std::sqrt(std::pow(cartesianXData, 2) + std::pow(cartesianYData, 2) + std::pow(cartesianZData, 2));
		//    phi=std::atan2(cartesianYData,cartesianXData);
		//    if(radius==0) theta=0;
		//    else theta=std::acos(cartesianZData/radius);
		return radius;
	}
	aType polarTheta() {
		lType radius;
		aType theta;
		//float phi;
		radius = std::sqrt(std::pow(cartesianXData, 2) + std::pow(cartesianYData, 2) + std::pow(cartesianZData, 2));
		//phi=std::atan2(cartesianYData,cartesianXData);
		if (radius == 0) theta = 0;
		else theta = std::acos(cartesianZData / radius);
		return theta;
	}
	aType polarPhi() {
		//	float radius;
		//	float theta;
		aType phi;
		//    float radius;
		//float theta;
		//radius=std::sqrt(std::pow(cartesianXData,2)+std::pow(cartesianYData,2)+std::pow(cartesianZData,2));
		phi = std::atan2(cartesianYData, cartesianXData);
		//if(radius==0) theta=0;
		//else theta=std::acos(cartesianZData/radius);
		return phi;
	}
	void polarRadius(lType radiusData) {
		lType radius;
		aType theta;
		aType phi;
		radius = std::sqrt(std::pow(cartesianXData, 2) + std::pow(cartesianYData, 2) + std::pow(cartesianZData, 2));
		phi = std::atan2(cartesianYData, cartesianXData);
		if (radius == 0) theta = 0;
		else theta = std::acos(cartesianZData / radius);
		radius = radiusData;
		Coord::polar(radius, theta, phi);
	}
	void polarTheta(aType thetaData) {
		lType radius;
		aType theta;
		aType phi;
		radius = std::sqrt(std::pow(cartesianXData, 2) + std::pow(cartesianYData, 2) + std::pow(cartesianZData, 2));
		phi = std::atan2(cartesianYData, cartesianXData);
		if (radius == 0) theta = 0;
		else theta = std::acos(cartesianZData / radius);
		theta = thetaData;
		Coord::polar(radius, theta, phi);
	}
	void polarPhi(aType phiData) {
		lType radius;
		aType theta;
		aType phi;
		radius = std::sqrt(std::pow(cartesianXData, 2) + std::pow(cartesianYData, 2) + std::pow(cartesianZData, 2));
		phi = std::atan2(cartesianYData, cartesianXData);
		if (radius == 0) theta = 0;
		else theta = std::acos(cartesianZData / radius);
		phi = phiData;
		Coord::polar(radius, theta, phi);
	}
	void polarRadiusAdd(lType radius) {
		polarRadius(polarRadius() + radius);
	}
	void polarThetaAdd(aType theta) {
		polarTheta(polarTheta() + theta);
	}
	void polarPhiAdd(aType phiArg) {
		polarPhi(polarPhi() + phiArg);
	}

	void angle(aType x, aType y, aType z) {
		angleXData = x;
		angleYData = y;
		angleZData = z;
	}
	aType angleX() {
		return angleXData;
	}
	aType angleY() {
		return angleYData;
	}
	aType angleZ() {
		return angleZData;
	}
	void angleX(aType x) {
		angleXData = x;
	}
	void angleY(aType y) {
		angleYData = y;
	}
	void angleZ(aType z) {
		angleZData = z;
	}
	void angleXAdd(aType x) {
		angleXData += x;
	}
	void angleYAdd(aType y) {
		angleYData += y;
	}
	void angleZAdd(aType z) {
		angleZData += z;
	}

	lType x() {
		return cartesianX();
	}
	lType y() {
		return cartesianY();
	}
	lType z() {
		return cartesianZ();
	}
	lType radius() {
		return polarRadius();
	}
	aType theta() {
		return polarTheta();
	}
	aType phi() {
		return polarPhi();
	}

	aType ax() {
		return angleX();
	}
	aType ay() {
		return angleY();
	}
	aType az() {
		return angleZ();
	}

	aType roll() {
		return angleX();
	}
	aType pitch() {
		return angleX();
	}
	aType yaw() {
		return angleX();
	}

	void x(lType value) {
		cartesianX(value);
	}
	void y(lType value) {
		cartesianY(value);
	}
	void z(lType value) {
		cartesianZ(value);
	}
	void radius(lType value) {
		polarRadius(value);
	}
	void theta(aType value) {
		polarTheta(value);
	}
	void phi(aType value) {
		polarPhi(value);
	}

	void ax(aType value) {
		angleX(value);
	}
	void ay(aType value) {
		angleY(value);
	}
	void az(aType value) {
		angleZ(value);
	}

	void revolutionXY(aType angle) {
		lType cartesianXSub = cartesianXData;
		lType cartesianYSub = cartesianYData;
		angleZData = angleZData + angle;
		//	int a = std::cos(angle);
		//	std::cout << std::endl << " angle : " << a << std::endl;
		cartesianXData = cartesianXSub * std::cos(angle) + (-1 * cartesianYSub * std::sin(angle));
		cartesianYData = cartesianXSub * std::sin(angle) + cartesianYSub * std::cos(angle);
	}
	void revolutionYZ(aType angle) {
		lType cartesianYSub = cartesianYData;
		lType cartesianZSub = cartesianZData;
		angleXData = angleXData + angle;
		cartesianYData = cartesianYSub * std::cos(angle) - cartesianZSub * std::sin(angle);
		cartesianZData = cartesianYSub * std::sin(angle) + cartesianZSub * std::cos(angle);
	}
	void revolutionZX(aType angle) {
		lType cartesianXSub = cartesianXData;
		lType cartesianZSub = cartesianZData;
		angleYData = angleYData + angle;
		cartesianZData = cartesianZSub * std::cos(angle) - cartesianXSub * std::sin(angle);
		cartesianXData = cartesianZSub * std::sin(angle) + cartesianXSub * std::cos(angle);
	}

	void revolutionX(aType angle) {
		lType cartesianXSub = cartesianXData;
		lType cartesianYSub = cartesianYData;
		lType cartesianZSub = cartesianZData;
		angleXData = angleXData + angle;
		cartesianXData = cartesianXSub;
		cartesianYData = cartesianYSub * std::cos(angle) - cartesianZSub * std::sin(angle);
		cartesianZData = cartesianYSub * std::sin(angle) + cartesianZSub * std::cos(angle);
	}
	void revolutionY(aType angle) {
		lType cartesianXSub = cartesianXData;
		lType cartesianYSub = cartesianYData;
		lType cartesianZSub = cartesianZData;
		angleYData = angleYData + angle;
		cartesianXData = cartesianXSub * std::cos(angle) + cartesianZSub * std::sin(angle);
		cartesianYData = cartesianYSub;
		cartesianZData = (-1 * cartesianYSub * std::sin(angle)) + cartesianZSub * std::cos(angle);
	}
	void revolutionZ(aType angle) {
		lType cartesianXSub = cartesianXData;
		lType cartesianYSub = cartesianYData;
		lType cartesianZSub = cartesianZData;
		angleZData = angleZData + angle;
		cartesianXData = cartesianXSub * std::cos(angle) - cartesianYSub * std::sin(angle);
		cartesianYData = cartesianXSub * std::sin(angle) + cartesianYSub * std::cos(angle);
		cartesianZData = cartesianZSub;
	}

	void revolutionXYPosition(aType angle) {
		lType cartesianXSub = cartesianXData;
		lType cartesianYSub = cartesianYData;
		//angleZData = angleZData + angle;
		//	int a = std::cos(angle);
		//	std::cout << std::endl << " angle : " << a << std::endl;
		cartesianXData = cartesianXSub * std::cos(angle) + (-1 * cartesianYSub * std::sin(angle));
		cartesianYData = cartesianXSub * std::sin(angle) + cartesianYSub * std::cos(angle);
	}
	void revolutionYZPosition(aType angle) {
		lType cartesianYSub = cartesianYData;
		lType cartesianZSub = cartesianZData;
		//angleXData = angleXData + angle;
		cartesianYData = cartesianYSub * std::cos(angle) - cartesianZSub * std::sin(angle);
		cartesianZData = cartesianYSub * std::sin(angle) + cartesianZSub * std::cos(angle);
	}
	void revolutionZXPosition(aType angle) {
		lType cartesianXSub = cartesianXData;
		lType cartesianZSub = cartesianZData;
		//angleYData = angleYData + angle;
		cartesianZData = cartesianZSub * std::cos(angle) - cartesianXSub * std::sin(angle);
		cartesianXData = cartesianZSub * std::sin(angle) + cartesianXSub * std::cos(angle);
	}

	void revolutionXPosition(aType angle) {
		lType cartesianXSub = cartesianXData;
		lType cartesianYSub = cartesianYData;
		lType cartesianZSub = cartesianZData;
		//angleXData = angleXData + angle;
		cartesianXData = cartesianXSub;
		cartesianYData = cartesianYSub * std::cos(angle) - cartesianZSub * std::sin(angle);
		cartesianZData = cartesianYSub * std::sin(angle) + cartesianZSub * std::cos(angle);
	}
	void revolutionYPosition(aType angle) {
		lType cartesianXSub = cartesianXData;
		lType cartesianYSub = cartesianYData;
		lType cartesianZSub = cartesianZData;
		//angleYData = angleYData + angle;
		cartesianXData = cartesianXSub * std::cos(angle) + cartesianZSub * std::sin(angle);
		cartesianYData = cartesianYSub;
		cartesianZData = (-1 * cartesianYSub * std::sin(angle)) + cartesianZSub * std::cos(angle);
	}
	void revolutionZPosition(aType angle) {
		lType cartesianXSub = cartesianXData;
		lType cartesianYSub = cartesianYData;
		lType cartesianZSub = cartesianZData;
		//angleZData = angleZData + angle;
		cartesianXData = cartesianXSub * std::cos(angle) - cartesianYSub * std::sin(angle);
		cartesianYData = cartesianXSub * std::sin(angle) + cartesianYSub * std::cos(angle);
		cartesianZData = cartesianZSub;
	}

	void revolutionEuler(aType a, aType b, aType c) {
		lType cartesianXSub = cartesianXData;
		lType cartesianYSub = cartesianYData;
		lType cartesianZSub = cartesianZData;
		cartesianXData = cartesianXSub * (std::cos(a) * std::cos(c) - std::sin(a) * std::cos(b) * std::sin(c)) + cartesianYSub * (std::cos(a) * std::sin(c) + std::sin(a) * std::cos(b) * std::cos(c)) + cartesianZSub * (std::sin(a) * std::sin(b));
		cartesianYData = cartesianXSub * (-1 * std::sin(a) * std::cos(c) - std::cos(a) * std::cos(b) * std::sin(c)) + cartesianYData * (-1 * std::sin(a) * std::sin(c) + std::cos(a) * std::cos(b) * std::cos(c)) + cartesianZData * (std::cos(a) * std::sin(b));
		cartesianZData = cartesianXSub * (std::sin(b) * std::sin(c)) + cartesianYSub * (-1 * std::sin(b) * std::cos(c)) + cartesianZSub * std::cos(b);

	}
	void revolutionEulerinverse(aType a, aType b, aType c) {
		lType cartesianXSub = cartesianXData;
		lType cartesianYSub = cartesianYData;
		lType cartesianZSub = cartesianZData;
		cartesianXData = cartesianXSub * (std::cos(a) * std::cos(c) - std::sin(a) * std::cos(b) * std::sin(c)) + cartesianYSub * (-1 * std::sin(a) * std::cos(c) + -1 * std::cos(a) * std::cos(b) * std::sin(c)) + cartesianZSub * (std::sin(b) * std::sin(c));
		cartesianYData = cartesianXSub * (std::cos(a) * std::sin(c) + std::sin(a) * std::cos(b) * std::cos(c)) + cartesianYSub * (-1 * std::sin(a) * std::sin(c) + std::cos(a) * std::cos(b) * std::cos(c)) + cartesianZSub * (-1 * std::sin(b) * std::cos(c));
		cartesianZData = cartesianXSub * (std::sin(a) * std::sin(b)) + cartesianYSub * (std::cos(a) * std::sin(b)) + cartesianZSub * std::cos(b);

	}

	Coord <lType, aType>operator+(Coord<lType, aType> &coord) {
		return Coord<lType, aType>(x() + coord.x(), y() + coord.y(), z() + coord.z(), ax() + coord.ax(), ay() + coord.ay(), az() + coord.az());
	}
	Coord <lType, aType>operator-(Coord<lType, aType> &coord) {
		return Coord<lType, aType>(x() - coord.x(), y() - coord.y(), z() - coord.z(), ax() - coord.ax(), ay() - coord.ay(), az() - coord.az());
	}
	Coord<lType, aType> operator*(aType value) {
		return Coord<lType, aType>(x()*value, y()*value, z()*value, ax()*value, ay()*value, az()*value);
	}
	Coord<lType, aType> operator/(aType value) {
		return Coord<lType, aType>(x() / value, y() / value, z() / value, ax() / value, ay() / value, az() / value);
	}
	void operator+=(Coord<lType, aType> &coord) {
		cartesian(x() + coord.x(), y() + coord.y(), z() + coord.z());
		angle(x() + coord.x(), y() + coord.y(), z() + coord.z());
	}
	void operator-=(Coord<lType, aType> &coord) {
		cartesian(x() - coord.x(), y() - coord.y(), z() - coord.z());
		angle(x() - coord.x(), y() - coord.y(), z() - coord.z());
	}
	void operator*=(aType value) {
		cartesian(x()*value, y()*value, z()*value);
		angle(x()*value, y()*value, z()*value);
	}
	void operator/=(aType value) {
		cartesian(x() / value, y() / value, z() / value);
		angle(x() / value, y() / value, z() / value);
	}
	bool operator==(Coord &coord) {
		return   ((x() == coord.x())
				|| (y() == coord.y())
				|| (z() == coord.z())
				|| (ax() == coord.ax())
				|| (ax() == coord.ax())
				|| (ax() == coord.ax()));
	}

};

}