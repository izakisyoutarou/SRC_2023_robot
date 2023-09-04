#include "pascal_triangle.hpp" //ver1.0

#include <stdexcept>

namespace rp_lib {

void PascalTriangle::calc(int exponent){
	value.clear();
	for(int l=0; l<=exponent; l++){
		value.push_back(std::vector<int>({1}));
		for(int n=1; n<l; n++){
			value[l].push_back(value[l-1][n-1] + value[l-1][n]);
		}
		if(l>0) value[l].push_back(1);
	}
}

int PascalTriangle::at(int n, int r){
	try{
		return value.at(n).at(r);
	}
	catch(std::out_of_range &ex){
		printf("*************** out of range ***************\n");
		return -1;
	}
}

}