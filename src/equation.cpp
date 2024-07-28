// Copyright 2024 Kyrylo Shyshko
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


// Equation.cpp:
// Definitions for the class Equation and friend operator overloaded functions


#include "symcalc/symcalc.hpp"

namespace symcalc{


// Constructors and deconstructors

// Empty constructor
Equation::Equation() : eq(nullptr) {}
// Constructor for any operation with check to auto simplify or not
Equation::Equation(EquationBase* make_eq) {
	if(SYMCALC_AUTO_SIMPLIFY){
		eq = make_eq->__simplify__();
		delete_equation_base(make_eq);
	}else{
		eq = make_eq;
	}
}

// Constructor for wrapped EquationValue
Equation::Equation(SYMCALC_VALUE_TYPE num){
	this->eq = new EquationValue(num);
}

// Constructor for wrapped Variable
Equation::Equation(SYMCALC_VAR_NAME_TYPE var_name) : eq(nullptr){
	eq = new Variable(var_name);
}

// Constructor for wrapped Constant
Equation::Equation(SYMCALC_VAR_NAME_TYPE const_name, SYMCALC_VALUE_TYPE value){
	this->eq = new Constant(const_name, value);
}

// Copy constructor
Equation::Equation(const Equation& lvalue) : eq(nullptr){
	const EquationBase* lvalue_eq = lvalue.eq;	
	eq = copy(lvalue_eq);
}

// Deconstructor
Equation::~Equation(){
	delete_equation_base(eq);
}














//
// Friend operators
//

// Print operator for std::cout << Equation
std::ostream& operator<<(std::ostream &stream, const Equation equation){
	stream << equation.eq->txt();
	return stream;
}

// Sum operator for Equation + Equation
Equation operator+(const Equation eq1, const Equation eq2){
	return Equation(new Sum({copy(eq1.eq), copy(eq2.eq)}));
}

// Subtraction operator for Equation - Equation
Equation operator-(const Equation eq1, const Equation eq2){
	return Equation(new Sum({copy(eq1.eq), new Negate(copy(eq2.eq))}));
}

// Multiplication operator for Equation * Equation
Equation operator*(const Equation eq1, const Equation eq2){
	return Equation(new Mult({copy(eq1.eq), copy(eq2.eq)}));
}

// Division operator for Equation / Equation
Equation operator/(const Equation eq1, const Equation eq2){
	return Equation(new Div(copy(eq1.eq), copy(eq2.eq)));
}

// Negate operator for -Equation
Equation operator-(const Equation eq1){
	return Equation(new Negate(copy(eq1.eq)));
}

// Comparison operator for std::map
bool operator<(const Equation holder1, const Equation holder2){
	return true;
}



// Other operators for creating expressions

// Power operator for Equation.pow(Equation)
Equation Equation::pow(Equation power) const{
	EquationBase* copy1 = copy(this->eq);
	EquationBase* copy2 = copy(power.eq);
	return Equation(new Power(copy1, copy2));
}


//
// Abstract operations functions
// Here are functions that relate to SymCalc functionality, like eval and derivative
//


// Derivative functions

Equation Equation::derivative(Equation variable, size_t order) const{
	Variable* var = dynamic_cast<Variable*>(variable.eq);
	if(!var){
		throw std::runtime_error("Provided variable is not of Variable type");
	}
	EquationBase* deriv = eq;
	for(size_t i = 0; i < order; i++){
		deriv = deriv->__derivative__(var->name);
	}	
	
	return Equation(deriv);
}


Equation Equation::derivative(size_t order) const{
	std::vector<Equation> vars = this->list_variables();
	if(vars.size() == 1){
		return this->derivative(vars[0], order);
	}else{
		throw std::runtime_error("Equation.derivative expects a variable to differentiate with respect to, since list_variables() gives more than one variable.");
	}
}




// Evaluation functions

SYMCALC_VALUE_TYPE Equation::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return eq->eval(var_hash);
}


SYMCALC_VALUE_TYPE Equation::eval(std::map<Equation, SYMCALC_VALUE_TYPE> var_hash) const{
	SYMCALC_VAR_HASH_TYPE new_var_hash;
	for(std::pair<Equation, SYMCALC_VALUE_TYPE> mypair: var_hash){
		Variable* var = dynamic_cast<Variable*>(mypair.first.eq);
		if(!var){
			throw std::runtime_error("Provided variable is not of Variable type");
		}
		new_var_hash[var->name] = mypair.second;
	}
	return eq->eval(new_var_hash);
}

SYMCALC_VALUE_TYPE Equation::eval() const{
	return this->eval(SYMCALC_VAR_HASH_TYPE());
}

SYMCALC_VALUE_TYPE Equation::operator()(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return eval(var_hash);
}
SYMCALC_VALUE_TYPE Equation::operator()(std::map<Equation, SYMCALC_VALUE_TYPE> var_hash) const{
	return eval(var_hash);
}
SYMCALC_VALUE_TYPE Equation::operator()() const{
	return this->eval(SYMCALC_VAR_HASH_TYPE());
}


// Simplification

Equation Equation::simplify() const{
	return Equation(eq->__simplify__());
}




// Listing variables

std::vector<Equation> Equation::list_variables() const{
	std::vector<SYMCALC_VAR_NAME_TYPE> vars = eq->list_variables();
	
	std::vector<Equation> converted;
	
	for(SYMCALC_VAR_NAME_TYPE var : vars){
		converted.push_back(Equation(var));
	}
	
	return converted;	
}

std::vector<std::string> Equation::list_variables_str() const{
	return eq->list_variables();
}



//
// Gradient vector functions
// 
// Returns a vector partial derivatives in the function
//

// Return the gradient vector for variables specified as the argument in that order
std::vector<Equation> Equation::gradient(std::vector<Equation> order) const{
	std::vector<Equation> final_out {};
	for(Equation var : order){
		final_out.push_back(this->derivative(var));
	}
	return final_out;
}

// Returns the gradient vector for all variables in some order provided by list_variables()
std::vector<Equation> Equation::gradient() const{
	return this->gradient(this->list_variables());
}




//
// Outside functions
//
// Functions that implement SymCalc functionality, but are outside of Equation
//


// Get list of variables for the vector of Equation
std::vector<std::string> list_variables_str(std::vector<Equation> equations){
	std::vector<std::string> variables {};
	for(Equation& eq : equations){
		std::vector<std::string> eq_variables = eq.list_variables_str();
		for(std::string& var : eq_variables){
			if(!include(variables, var)){
				variables.push_back(var);
			}
		}
	}
	return variables;
}


std::vector<Equation> list_variables(std::vector<Equation> equations){
	std::vector<std::string> variables = list_variables_str(equations);
	std::vector<Equation> final_vars {};
	final_vars.reserve(variables.size());
	for(std::string var : variables){
		final_vars.push_back(Equation(var));
	}
	return final_vars;
}

// Jacobian matrix out of a list of equations
std::vector<std::vector<Equation>> jacobian(std::vector<Equation> eqs){
	std::vector<std::string> var_list = list_variables_str(eqs);
	std::vector<std::vector<Equation>> jacobian_matrix {};
	
	for(Equation eq : eqs){
		std::vector<Equation> gradients {};
		for(std::string var : var_list){
			gradients.push_back(eq.derivative(var));
		}
		jacobian_matrix.push_back(gradients);
	}
	return jacobian_matrix;
}






} // End symcalc namespace


