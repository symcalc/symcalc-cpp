#include "symcalc/symcalc.hpp"
#include <iomanip>

using namespace symcalc;

// Explanation:
// The Newton-Raphson method is an iterative root-finding algorithm developed in the 17th century
// It's goal is to find where a function intersects the y = 0 line, or is just equal to zero
// 
// Mathematically, the Newton-Raphson method is defined like this:
// x[n + 1] = x[n] - f(x[n]) / f'(x[n])
// where n is the iteration, x[n] is the estimate at the iteration and f(x) is the function we are finding the zero of
// 
// Let's try coding this with SymCalc!
//
// P.s. SymCalc's real power comes into play when we change the function
// Instead of having to re-write both the function and the derivative by hand
// SymCalc can do that for us, and we can just worry about fx
// Which is really useful!
//
// Try changing the function on your own and seeing how it goes!
//


int main(){
	// Declare a variable
	Equation x ("x");
	
	// Declare a function
	Equation fx = x.pow(3) - 5 * x.pow(2) + abs(x + 0.3);
	
	// Print the function
	std::cout << "f(x) = " << fx << std::endl;
	
	// Differentiate the function
	Equation dfdx = fx.derivative();
	
	// Come up with the first estimate
	double x_estimate = 1.0;
	
	// Set the number of iterations
	int iterations = 10;
	
	for(int i = 0; i < iterations; i++){
		x_estimate = x_estimate - fx.eval({{x, x_estimate}}) / dfdx.eval({{x, x_estimate}});
	}
	
	std::cout << "Final estimate: " << std::setprecision(15) << x_estimate << std::endl; // Output: 0.385391230515262
	
	std::cout << "f(x) value: " << std::setprecision(15) << fx.eval({{x, x_estimate}}) << std::endl; // A really small number, close to zero
	
	return 0;
}