#include "symcalc/symcalc.hpp"

using namespace symcalc;

// Explanation:
// In this program, we will just get a basic glimpse of SymCalc's abilities
// We will create a new variable, a function dependent on it, evaluate and differentiate it!

int main(){	
	// Declare a variable
	Equation x ("x");
	
	// Print the variable
	std::cout << "Variable: " << x << std::endl;
	
	// Declare a function
	Equation fx = sin(x + 5);
	
	// Print the function
	std::cout << "f(x) = " << fx << std::endl;
	
	// Evaluate the function at x = 5
	double value = fx.eval({{x, 5}});
	std::cout << fx << " at x = 5, is equal to " << value << std::endl;
	
	// Differentiate the function
	Equation dfdx = fx.derivative();
	
	// Print the derivative
	std::cout << "Derivative of " << fx << " is equal to " << dfdx << std::endl;
	
	// Evaluate the derivative at x = 5
	double slope = dfdx.eval({{x, 5}});
	
	std::cout << "Slope of " << fx << ", at x = 5, is equal to " << slope << std::endl;
	
	return 0;
}