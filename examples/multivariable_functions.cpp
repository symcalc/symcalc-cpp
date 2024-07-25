#include "symcalc/symcalc.hpp"

using namespace symcalc;

// Explanation:
// In this program, we will define two variables and a function that is dependent on both of them
// We will evaluate this function
// And also differentiate with respect to a varialbe

int main(){	
	// Declare some variables
	Equation x ("x");
	Equation y ("y");
	
	// Declare a function
	Equation fxy = sin(x - y) + abs(x * y.pow(2));
	
	// Print function
	std::cout << "f(x, y) = " << fxy << std::endl << std::endl;
	
	// Differentiate with respect to x
	Equation dfdx = fxy.derivative(x);
	
	// Differentiate with respect to y
	Equation dfdy = fxy.derivative(y);
	
	// Print the derivatives
	std::cout << "df/dx = " << dfdx << std::endl << std::endl;
	
	std::cout << "df/dy = " << dfdy << std::endl << std::endl;
	
	// Evaluate one of them
	double value = fxy.eval({{x, 5}, {y, -5}});
	
	std::cout << "f(5, -5) = " << value << std::endl;
	
	return 0;
}