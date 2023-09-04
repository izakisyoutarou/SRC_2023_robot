#pragma once
/*******************************************
 * pascal_triangle ver1.0 2021/1/26
 * This program calculate binomial coefficients by using pascal triangle
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.0 2021/1/26 The first version
 ******************************************/

#include <vector>

namespace rp_lib {
	
class PascalTriangle {
public:
	PascalTriangle() {}
	void calc(int exponent);
	int at(int n, int r);
private:
	std::vector<std::vector<int>> value;
};

}