#include "symcalc/symcalc.hpp"

using namespace symcalc;

// Explanation:
// One of SymCalc's classes are logarithms
// Let's see how to work with them!

int main(){
	// Declare a variable
	Equation x("x");
	
	// Declare a function log_2(x)
	Equation fx = log(x, 2);
	
	std::cout << "f(x) = " << fx << std::endl;
	
	// Find it's derivative
	Equation dfdx = fx.derivative();
	
	std::cout << "f'(x) = " << dfdx << std::endl;
	
	// Evaluate the derivative
	double value = dfdx.eval({{x, 2}});
	
	std::cout << value << std::endl;
	
	return 0;
}