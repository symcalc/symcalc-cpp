#include "symcalc/symcalc.hpp"

using namespace symcalc;

// Explanation:
// In SymCalc, you have the ability to create Constants
// They behave just like equation values (or numerical values), but look like variables when printed
//
// SymCalc has some pre-defined constants, like Pi or E
//
// Let's see how we can use them

int main(){
	// Declare a variable
	Equation x("x");
	
	// Declare a function pi * x
	Equation fx = x * Constants::Pi;
	
	std::cout << "f(x) = " << fx << std::endl;
	
	// Find it's derivative
	Equation dfdx = fx.derivative();
	
	std::cout << "f'(x) = " << dfdx << std::endl;
	
	// Evaluate the derivative, should work without any variables provided since df/dx = pi and is not dependent on x
	double value = dfdx.eval();
	
	std::cout << value << std::endl;
	
	return 0;
}