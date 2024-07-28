#include "symcalc/symcalc.hpp"

using namespace symcalc;

int main(){
	
	Equation x ("x");
	Equation y ("y");
	Equation z ("z");
	
	
	Equation fx = x.pow(2) + y.pow(2);
	Equation gx = z.pow(2) + x.pow(3);
	
	auto matrix = jacobian({fx, gx});

	for(std::vector<Equation> vec : matrix){
		std::cout << "[ ";
		for(Equation eq : vec){
			std::cout << eq << "   ";
		}
		std::cout << " ]" << std::endl;
	}

//	std::vector<Equation> gradients {gradients_map};
//
//	std::cout << gradients[0] << std::endl;

	return 0;
}
