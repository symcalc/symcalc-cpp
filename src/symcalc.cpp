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

#include "symcalc/symcalc.hpp"
#include <cstdlib>

namespace symcalc{
	
bool SYMCALC_AUTO_SIMPLIFY = true;
	
EquationBase* to_equation(SYMCALC_VALUE_TYPE num){
	return new EquationValue(num);
}



EquationBase::EquationBase(std::string el_type){
	this->type = el_type;
}

EquationBase::EquationBase(const EquationBase& lvalue){
	type = lvalue.type;
}

EquationBase::~EquationBase(){
	// std::cout << "destructor" << std::endl;
}


EquationBase* EquationBase::__simplify__() const {return copy(this);};


Variable::Variable(SYMCALC_VAR_NAME_TYPE name) : EquationBase("var"), name(name) {}

Variable::Variable(const Variable& lvalue) : EquationBase(lvalue){
	name = lvalue.name;
}

Variable::~Variable(){
	
}


std::vector<SYMCALC_VAR_NAME_TYPE> Variable::list_variables() const{
	return std::vector<SYMCALC_VAR_NAME_TYPE>({this->name});
}

std::string Variable::txt() const{
	return name;
}

SYMCALC_VALUE_TYPE Variable::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return var_hash[this->name];
}



EquationBase* Variable::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	EquationValue* out = new EquationValue(1.0);
	if(var != this->name){
		out->value = 0;
	}
	return out;
}


EquationBase* Variable::__simplify__() const{
	return copy(this);
}

EquationBase* Variable::__copy_equation_base__() const{
	const Variable* casted = dynamic_cast<const Variable*>(this);
	return new Variable(*casted);
}
void Variable::__delete_equation_base__(){
	Variable* casted = dynamic_cast<Variable*>(this);
	delete casted;
}


EquationValue::EquationValue(SYMCALC_VALUE_TYPE value) : EquationBase("val"), value(value) {
	this->ready_txt = std::to_string(value);
	this->ready_txt.erase(this->ready_txt.find_last_not_of('0') + 1, std::string::npos);
	this->ready_txt.erase(this->ready_txt.find_last_not_of('.') + 1, std::string::npos);
}

EquationValue::EquationValue(const EquationValue& lvalue) : EquationBase(lvalue), value(lvalue.value), ready_txt(lvalue.ready_txt){
}

EquationValue::~EquationValue(){
	
}

std::vector<SYMCALC_VAR_NAME_TYPE> EquationValue::list_variables() const{
	return std::vector<SYMCALC_VAR_NAME_TYPE>();
}

std::string EquationValue::txt() const{
	return this->ready_txt;
}

SYMCALC_VALUE_TYPE EquationValue::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return value;
}

EquationBase* EquationValue::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	return (new EquationValue(0));
}


EquationBase* EquationValue::__simplify__() const{
	return copy(this);
}


EquationBase* EquationValue::__copy_equation_base__() const{
	const EquationValue* casted = dynamic_cast<const EquationValue*>(this);
	return new EquationValue(*casted);
}
void EquationValue::__delete_equation_base__(){
	EquationValue* casted = dynamic_cast<EquationValue*>(this);
	delete casted;
}




Constant::Constant(SYMCALC_VAR_NAME_TYPE name, SYMCALC_VALUE_TYPE value) : EquationValue(value), name(name) {
	this->type = "const";
}

Constant::Constant(const Constant& lvalue) : EquationValue(lvalue){
	this->name = lvalue.name;
}

std::string Constant::txt() const{
	return this->name;
}

EquationBase* Constant::__copy_equation_base__() const{
	const Constant* casted = dynamic_cast<const Constant*>(this);
	return new Constant(*casted);
}

void Constant::__delete_equation_base__(){
	Constant* casted = dynamic_cast<Constant*>(this);
	delete casted;
}





Sum::Sum(std::vector<EquationBase*> inp_elements) : EquationBase("sum"){
	std::vector<EquationBase*> extracted_elements;
	
	for(EquationBase* &el : inp_elements){	
		
		if(el->type == "sum"){ // if the element is a sum - extract its elements into the current sum object
			Sum* sum_element = dynamic_cast<Sum*>(el); // dynamic cast of EquationBase* to Sum* to get the .elements attribute
			for(EquationBase* sum_el_part : sum_element->elements){
				extracted_elements.push_back(sum_el_part);
			};
		}else{
			extracted_elements.push_back(el);
		}
	}
	
	this->elements = extracted_elements;
	
	ready_txt = "(" + this->elements[0]->txt() + ")";
	
	for(size_t i = 1; i < this->elements.size(); i++){
		ready_txt += " + (" + this->elements[i]->txt() + ")";
	}
}





Sum::Sum(const Sum& lvalue) : EquationBase(lvalue), elements(), ready_txt(lvalue.ready_txt){
	for(const EquationBase* lvalue_el : lvalue.elements){
		elements.push_back(copy(lvalue_el));
	}
	
}

Sum::~Sum(){
	for(EquationBase* el : elements){
		delete_equation_base(el);
	}
}



std::vector<SYMCALC_VAR_NAME_TYPE> Sum::list_variables() const{
	std::vector<SYMCALC_VAR_NAME_TYPE> vars;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> temp_vars;
	
	for(EquationBase* el : elements){
		temp_vars = el->list_variables();
		
		for(SYMCALC_VAR_NAME_TYPE var : temp_vars){
			if(!include(vars, var)){
				vars.push_back(var);
			}
		}
		
	}
	
	return vars;
}


std::string Sum::txt() const{
	return ready_txt;
}

SYMCALC_VALUE_TYPE Sum::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	SYMCALC_VALUE_TYPE result {0};
	
	for(EquationBase* el : this->elements){
		result += el->eval(var_hash);
	}
	
	return result;
}

EquationBase* Sum::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	
	std::vector<EquationBase*> derivs;
	derivs.reserve(elements.size());
	
	for(EquationBase* el : elements){
		derivs.push_back(el->__derivative__(var));
	}
	
	return new Sum(derivs);
}


EquationBase* Sum::__simplify__() const{
	std::vector<EquationBase*> els;

	
	for(EquationBase* element : elements){
		EquationBase* simplified = element->__simplify__();
		if(simplified->type == "val"){
			EquationValue* casted = dynamic_cast<EquationValue*>(simplified);
			if(casted->value != 0){
				els.push_back(simplified);
			}
		}else{
			els.push_back(simplified);
		}
	}
	
	if(els.size() == 1){
		return els[0];
	}
	
	return new Sum(els);
}

EquationBase* Sum::__copy_equation_base__() const{
	const Sum* casted = dynamic_cast<const Sum*>(this);
	return new Sum(*casted);
}
void Sum::__delete_equation_base__(){
	Sum* casted = dynamic_cast<Sum*>(this);
	delete casted;
}



Negate::Negate(EquationBase* eq) : EquationBase("neg"), eq(eq) {
	this->ready_txt = "-(" + eq->txt() + ")";
}

Negate::Negate(const Negate& lvalue) : EquationBase(lvalue), eq(nullptr), ready_txt(lvalue.ready_txt){
	const EquationBase* lvalue_eq = lvalue.eq;
	eq = copy(lvalue_eq);
}

Negate::~Negate(){
	delete_equation_base(eq);
}

std::vector<SYMCALC_VAR_NAME_TYPE> Negate::list_variables() const{
	return eq->list_variables();
}


std::string Negate::txt() const{
	return this->ready_txt;
}

SYMCALC_VALUE_TYPE Negate::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return -eq->eval(var_hash);
}

EquationBase* Negate::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	return new Negate(eq->__derivative__(var));
}


EquationBase* Negate::__simplify__() const{
	EquationBase* simplified = eq->__simplify__();
	if(eq->type == "neg"){
		Negate* casted = dynamic_cast<Negate*>(simplified);
		return casted->eq;
	}else{
		return new Negate(simplified);
	}
}

EquationBase* Negate::__copy_equation_base__() const{
	const Negate* casted = dynamic_cast<const Negate*>(this);
	return new Negate(*casted);
}
void Negate::__delete_equation_base__(){
	Negate* casted = dynamic_cast<Negate*>(this);
	delete casted;
}



Mult::Mult(std::vector<EquationBase*> inp_elements) : EquationBase("mult"){
	std::vector<EquationBase*> extracted_elements;
	
	for(EquationBase* &el : inp_elements){	
		
		if(el->type == "mult"){ // if the element is a sum - extract its elements into the current sum object
			Mult* mult_element = dynamic_cast<Mult*>(el); // dynamic cast of EquationBase* to Sum* to get the .elements attribute
			for(EquationBase* mult_el_part : mult_element->elements){
				extracted_elements.push_back(mult_el_part);
			};
		}else{
			extracted_elements.push_back(el);
		}
	}
	
	this->elements = extracted_elements;
	
	ready_txt = "(" + this->elements[0]->txt() + ")";
	
	for(size_t i = 1; i < this->elements.size(); i++){
		ready_txt += " * (" + this->elements[i]->txt() + ")";
	}
}

Mult::Mult(const Mult& lvalue) : EquationBase(lvalue), elements(), ready_txt(lvalue.ready_txt){
	for(const EquationBase* lvalue_el : lvalue.elements){
		elements.push_back(copy(lvalue_el));
	}
}

Mult::~Mult(){
	for(EquationBase* el : elements){
		delete_equation_base(el);
	}
}

std::vector<SYMCALC_VAR_NAME_TYPE> Mult::list_variables() const{
	std::vector<SYMCALC_VAR_NAME_TYPE> vars;
	std::vector<SYMCALC_VAR_NAME_TYPE> temp_vars;
	
	
	for(const EquationBase* el : elements){
		temp_vars = el->list_variables();
		for(SYMCALC_VAR_NAME_TYPE var : temp_vars){
			if(!include(vars, var)){
				vars.push_back(var);
			}
		}
	}
	
	return vars;
}


std::string Mult::txt() const{
	return ready_txt;
}

SYMCALC_VALUE_TYPE Mult::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	SYMCALC_VALUE_TYPE result (1.0);
	
	for(EquationBase* el : this->elements){
		result *= el->eval(var_hash);
	}
	
	return result;
}

EquationBase* Mult::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	
	std::vector<EquationBase*> mults_to_sum;
	mults_to_sum.reserve(elements.size());
	
	
	std::vector<EquationBase*> els_to_mult;
	els_to_mult.reserve(elements.size());
	
	for(size_t el_i = 0; el_i < elements.size(); el_i++){
		
		els_to_mult.push_back(elements[el_i]->__derivative__(var));
		
		for(size_t el_noderiv_i = 0; el_noderiv_i < elements.size(); el_noderiv_i++){
			if(el_noderiv_i == el_i){
				continue;
			}
			els_to_mult.push_back(copy(elements[el_noderiv_i]));
		}
		
		mults_to_sum.push_back(new Mult(els_to_mult));
		els_to_mult.clear();
	}
	
	return new Sum(mults_to_sum);
}



EquationBase* Mult::__simplify__() const{
	
	std::vector<EquationBase*> els;
	
	if(elements.size() == 1){
		return elements[0]->__simplify__();
	}
	
	SYMCALC_VALUE_TYPE coeff = 1.0; // Multiply all numerical values to a single coefficient

	for(EquationBase* el : elements){
		EquationBase* simplified = el->__simplify__();
		if(simplified->type == "val"){
			EquationValue* casted = dynamic_cast<EquationValue*>(simplified);
			if(casted->value == 0){ // If zero, stop loop and output zero, since anything * 0 is 0
				for(EquationBase* el : els){
					delete_equation_base(el);
				}
				return new EquationValue(0);
			}else{
				coeff *= casted->value;				
			}
		}else{
			els.push_back(simplified);
		}
	}
	
	if(coeff != 1){
		els.insert(els.begin(), new EquationValue(coeff));
	}
	
	if(els.size() == 1){
		return els[0];
	}
	
	return new Mult(els);
}


EquationBase* Mult::__copy_equation_base__() const{
	const Mult* casted = dynamic_cast<const Mult*>(this);
	return new Mult(*casted);
}
void Mult::__delete_equation_base__(){
	Mult* casted = dynamic_cast<Mult*>(this);
	delete casted;
}





Div::Div(EquationBase* dividend, EquationBase* divisor) : EquationBase("div"), dividend(dividend), divisor(divisor){
	this->ready_txt = "(" + dividend->txt() + ") / (" + divisor->txt() + ")"; 
}

Div::Div(const Div& lvalue) : EquationBase(lvalue), ready_txt(lvalue.ready_txt){
	const EquationBase* lvalue_dividend = lvalue.dividend;
	dividend = copy(lvalue_dividend);
	
	const EquationBase* lvalue_divisor = lvalue.divisor;
	divisor = copy(lvalue_divisor);
}

Div::~Div(){
	delete_equation_base(dividend);
	delete_equation_base(divisor);
}

std::vector<SYMCALC_VAR_NAME_TYPE> Div::list_variables() const{
	std::vector<SYMCALC_VAR_NAME_TYPE> vars = dividend->list_variables();
	std::vector<SYMCALC_VAR_NAME_TYPE> temp_vars = divisor->list_variables();
	for(std::string el : temp_vars){
		if(!include(vars, el)){
			vars.push_back(el);
		}
	}
	return vars;
}

std::string Div::txt() const{
	return this->ready_txt;
}

SYMCALC_VALUE_TYPE Div::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return dividend->eval(var_hash) / divisor->eval(var_hash);
}

EquationBase* Div::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	
	// Derivative of f(x) / g(x)
	// = (f'(x) * g(x) - f(x) * g'(x)) / g(x)^2
	
	EquationBase* left_part = new Mult({dividend->__derivative__(var), copy(divisor)}); // f'(x) * g(x)
	EquationBase* right_part = new Mult({copy(dividend), divisor->__derivative__(var)}); // f(x) * g'(x)
	
	EquationBase* top = new Sum({left_part, new Negate(right_part)}); // f'(x) * g(x) - f(x) * g'(x)
	
	EquationBase* bottom = new Power(copy(divisor), to_equation(2)); // g(x) ^ 2
	
	return new Div(top, bottom); // (f'(x) * g(x) - f(x) * g'(x)) / g(x)^2
}



EquationBase* Div::__simplify__() const{
	
	EquationBase* dividend_s = dividend->__simplify__();
	EquationBase* divisor_s = divisor->__simplify__();
	
	return new Div(dividend_s, divisor_s);
}


EquationBase* Div::__copy_equation_base__() const{
	const Div* casted = dynamic_cast<const Div*>(this);
	return new Div(*casted);
}
void Div::__delete_equation_base__(){
	Div* casted = dynamic_cast<Div*>(this);
	delete casted;
}





Power::Power(EquationBase* base, EquationBase* power) : EquationBase("pow"), base(base), power(power){
	this->ready_txt = "(" + base->txt() + ") ^ (" + power->txt() + ")";
}

Power::Power(const Power& lvalue) : EquationBase(lvalue), ready_txt(lvalue.ready_txt){
	const EquationBase* lvalue_base = lvalue.base;
	this->base = copy(lvalue_base);
	
	const EquationBase* lvalue_power = lvalue.power;
	this->power = copy(lvalue_power);
}

Power::~Power(){
	delete_equation_base(base);
	delete_equation_base(power);
}

std::vector<SYMCALC_VAR_NAME_TYPE> Power::list_variables() const{
	std::vector<SYMCALC_VAR_NAME_TYPE> vars = base->list_variables();
	std::vector<SYMCALC_VAR_NAME_TYPE> temp_vars = power->list_variables();
	for(std::string el : temp_vars){
		if(!include(vars, el)){
			vars.push_back(el);
		}
	}
	return vars;
}



std::string Power::txt() const{
	return this->ready_txt;
}

SYMCALC_VALUE_TYPE Power::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return std::pow(base->eval(var_hash), power->eval(var_hash));
}

EquationBase* Power::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	if(power->type == "val"){
		EquationValue* power_eq = dynamic_cast<EquationValue*>(power);
		return new Mult({copy(power), new Power(copy(base), to_equation(power_eq->value - 1)), base->__derivative__(var)});
	}else{
		// derivative of f(x) ^ g(x) = f(x)^g(x) * (g'(x) * ln(f(x)) + f'(x) * g(x) / f(x))
		EquationBase* mult_l_part = new Power(copy(base), copy(power));
		
		EquationBase* sum_l_part = new Mult({power->__derivative__(var), new Ln(copy(base))});
		
		EquationBase* sum_r_part_mult = new Mult({base->__derivative__(var), copy(power)});
		EquationBase* sum_r_part = new Div(sum_r_part_mult, copy(base));
		
		EquationBase* mult_r_part = new Sum({sum_l_part, sum_r_part});
		
		return new Mult({mult_l_part, mult_r_part});
	}
}



EquationBase* Power::__simplify__() const{

	EquationBase* base_s = base->__simplify__();
	EquationBase* power_s = power->__simplify__();

	if(base_s->type == "val"){
		EquationValue* casted = dynamic_cast<EquationValue*>(base_s);
		if(casted->value == 0){
			return new EquationValue(0);
		}else if(casted->value == 1){
			return new EquationValue(1);
		}
	}
	
	if(power_s->type=="val"){
		EquationValue* casted = dynamic_cast<EquationValue*>(power_s);
		if(casted->value == 0){
			return new EquationValue(1);
		}else if(casted->value == 1){
			return base_s;
		}
	}
	
	return new Power(base_s, power_s);
	
}

EquationBase* Power::__copy_equation_base__() const{
	const Power* casted = dynamic_cast<const Power*>(this);
	return new Power(*casted);
}
void Power::__delete_equation_base__(){
	Power* casted = dynamic_cast<Power*>(this);
	delete casted;
}




Log::Log(EquationBase* eq, EquationBase* base) : EquationBase("log"), eq(eq), base(base){
	this->ready_txt = "log_(" + base->txt() + ")(" + eq->txt() + ")";
}

Log::Log(const Log& lvalue) : EquationBase(lvalue), ready_txt(lvalue.ready_txt){
	const EquationBase* lvalue_eq = lvalue.eq;
	const EquationBase* lvalue_base = lvalue.base;
	this->eq = copy(lvalue_eq);
	this->base = copy(lvalue_base);
}

Log::~Log(){
	delete_equation_base(eq);
	delete_equation_base(base);
}

std::vector<SYMCALC_VAR_NAME_TYPE> Log::list_variables() const{
	std::vector<SYMCALC_VAR_NAME_TYPE> variables = eq->list_variables();
	for(SYMCALC_VAR_NAME_TYPE var : base->list_variables()){
		if(!include(variables, var)){
			variables.push_back(var);
		}
	}
	return variables;
}


std::string Log::txt() const{
	return this->ready_txt;
}

SYMCALC_VALUE_TYPE Log::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return std::log(eq->eval(var_hash)) / std::log(base->eval(var_hash));
}

EquationBase* Log::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	EquationBase* div = new Div(eq->__derivative__(var), copy(eq));
	EquationBase* natural_log = new Ln(copy(this->base));
	return new Mult({div, natural_log});
}


EquationBase* Log::__simplify__() const{
	EquationBase* simplified_eq = eq->__simplify__();
	EquationBase* simplified_base = base->__simplify__();
	return new Log(simplified_eq, simplified_base);
}

EquationBase* Log::__copy_equation_base__() const{
	const Log* casted = dynamic_cast<const Log*>(this);
	return new Log(*casted);
}
void Log::__delete_equation_base__(){
	Log* casted = dynamic_cast<Log*>(this);
	delete casted;
}







Ln::Ln(EquationBase* eq) : EquationBase("ln"), eq(eq){
	this->ready_txt = "ln(" + eq->txt() + ")";
}

Ln::Ln(const Ln& lvalue) : EquationBase(lvalue), ready_txt(lvalue.ready_txt){
	const EquationBase* lvalue_eq = lvalue.eq;
	eq = copy(lvalue_eq);
}

Ln::~Ln(){
	delete_equation_base(eq);
}

std::vector<SYMCALC_VAR_NAME_TYPE> Ln::list_variables() const{
	return eq->list_variables();
}


std::string Ln::txt() const{
	return this->ready_txt;
}

SYMCALC_VALUE_TYPE Ln::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return std::log(eq->eval(var_hash));
}

EquationBase* Ln::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	return new Div(eq->__derivative__(var), copy(eq));
}


EquationBase* Ln::__simplify__() const{
	EquationBase* simplified = eq->__simplify__();
	if(simplified->type == "exp"){
		Exp* casted = dynamic_cast<Exp*>(simplified);
		EquationBase* return_value = copy(casted->eq);
		delete_equation_base(casted);
		return return_value;
	}else if(simplified->type == "const"){
		Constant* casted = dynamic_cast<Constant*>(simplified);
		if(casted->name == "e"){
			delete_equation_base(simplified);
			return new EquationValue(1);
		}
	}
	return new Ln(simplified);
}

EquationBase* Ln::__copy_equation_base__() const{
	const Ln* casted = dynamic_cast<const Ln*>(this);
	return new Ln(*casted);
}
void Ln::__delete_equation_base__(){
	Ln* casted = dynamic_cast<Ln*>(this);
	delete casted;
}









Exp::Exp(EquationBase* eq) : EquationBase("exp"), eq(eq) {

}

Exp::Exp(const Exp& lvalue) : EquationBase("exp"){
	eq = copy(lvalue.eq);
}



Exp::~Exp(){
	delete_equation_base(eq);
}


std::string Exp::txt() const{
	return std::string("exp(") + eq->txt() + ")";
}


SYMCALC_VALUE_TYPE Exp::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return std::exp(eq->eval(var_hash));
}

EquationBase* Exp::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	return new Mult({copy(this), eq->__derivative__(var)});
}


std::vector<SYMCALC_VAR_NAME_TYPE> Exp::list_variables() const{
	return eq->list_variables();
}


EquationBase* Exp::__simplify__() const{
	EquationBase* simplified = eq->__simplify__();
	if(simplified->type == "ln"){
		Ln* casted = dynamic_cast<Ln*>(simplified);
		return casted->eq;
	}else{
		return new Exp(simplified);
	}
}

EquationBase* Exp::__copy_equation_base__() const{
	const Exp* casted = dynamic_cast<const Exp*>(this);
	return new Exp(*casted);
}
void Exp::__delete_equation_base__(){
	Exp* casted = dynamic_cast<Exp*>(this);
	delete casted;
}







Abs::Abs(EquationBase* insides) : EquationBase("abs"), insides(insides) {} // Normal constructor
Abs::Abs(const Abs& lvalue) : EquationBase(lvalue){
	// Use the copy function to copy the insides of the lvalue Abs object
	this->insides = copy(lvalue.insides);
}

// Deconstructor
Abs::~Abs(){
	// Delete the insides on deconstruct
	delete_equation_base(insides);
}


// Dynamic cast copy function
EquationBase* Abs::__copy_equation_base__() const{
	const Abs* casted = dynamic_cast<const Abs*>(this);
	return new Abs(*casted); // Create a copy and return it
}


// Dynamic cast delete function
void Abs::__delete_equation_base__(){
	Abs* casted = dynamic_cast<Abs*>(this);
	delete casted;
}

// Text represantation
std::string Abs::txt() const{
	return "|" + insides->txt() + "|";
}

// Eval function
SYMCALC_VALUE_TYPE Abs::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	SYMCALC_VALUE_TYPE insides_eval = insides->eval(var_hash);
	if(insides_eval < 0){
		return -insides_eval;
	}else{
		return insides_eval;
	}
}

// List variables function
std::vector<SYMCALC_VAR_NAME_TYPE> Abs::list_variables() const{
	return insides->list_variables();
}

// Derivative function
// |f(x)|' = (f(x) / |f(x)|) * f'(x)
// Example:
// If f(x) = x, then
// |x|' = (x / |x|) * (x)' = (x / |x|) * 1 = x / |x|
EquationBase* Abs::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	EquationBase* insides_derivative = insides->__derivative__(var); // f'(x)
	EquationBase* insides_copy_1 = copy(insides); // f(x)
	EquationBase* insides_copy_2 = copy(insides); // f(x)
	EquationBase* abs_insides = new Abs(insides_copy_2); // |f(x)|
	EquationBase* div = new Div(insides_copy_1, abs_insides); // f(x) / |f(x)|
	EquationBase* mult = new Mult({div, insides_derivative}); // (f(x) / |f(x)|) * f'(x)
	return mult;
}


// Simplify function
EquationBase* Abs::__simplify__() const{
	EquationBase* simplified_insides = insides->__simplify__();
	if(simplified_insides->type == "abs"){
		return simplified_insides; // Absolute function twice is the same as once, ||x|| = |x| 
	}else if(simplified_insides->type == "pow"){
		// Check for a power that can be only positive, e.g. |x^2| = x^2
		const Power* casted = dynamic_cast<const Power*>(simplified_insides);
		if(casted->power->type == "val"){
			const EquationValue* val_casted = dynamic_cast<const EquationValue*>(casted->power);
			if(fmod(val_casted->value, 2) == 0){ // Check if power is divisible by 2
				return simplified_insides;
			}
		}
	}
	// Implement further checks if needed
	
	// If no simplifications work, return just the insides simplified in an abs function
	return new Abs(simplified_insides);
}








Sin::Sin(EquationBase* eq) : EquationBase("sin"), eq(eq) {};

Sin::Sin(const Sin& lvalue) : EquationBase(lvalue){
	eq = copy(lvalue.eq);
}

Sin::~Sin(){
	delete_equation_base(eq);
}


std::string Sin::txt() const{
	return "sin(" + eq->txt() + ")";
}

std::vector<SYMCALC_VAR_NAME_TYPE> Sin::list_variables() const{
	return eq->list_variables();
}


SYMCALC_VALUE_TYPE Sin::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return std::sin(eq->eval(var_hash));
}


EquationBase* Sin::__simplify__() const{
	return copy(this);
}

EquationBase* Sin::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	EquationBase* cos_func = new Cos(copy(eq));
	EquationBase* eq_deriv = eq->__derivative__(var);
	return new Mult({cos_func, eq_deriv});
}

EquationBase* Sin::__copy_equation_base__() const{
	const Sin* casted = dynamic_cast<const Sin*>(this);
	return new Sin(*casted);
}

void Sin::__delete_equation_base__(){
	Sin* casted = dynamic_cast<Sin*>(this);
	delete casted;
}









Cos::Cos(EquationBase* eq) : EquationBase("cos"), eq(eq) {};

Cos::Cos(const Cos& lvalue) : EquationBase(lvalue){
	eq = copy(lvalue.eq);
}

Cos::~Cos(){
	delete_equation_base(eq);
}


std::string Cos::txt() const{
	return "cos(" + eq->txt() + ")";
}

std::vector<SYMCALC_VAR_NAME_TYPE> Cos::list_variables() const{
	return eq->list_variables();
}


SYMCALC_VALUE_TYPE Cos::eval(SYMCALC_VAR_HASH_TYPE var_hash) const{
	return std::cos(eq->eval(var_hash));
}


EquationBase* Cos::__simplify__() const{
	return copy(this);
}

EquationBase* Cos::__derivative__(SYMCALC_VAR_NAME_TYPE var) const{
	EquationBase* minus_sin_func = new Negate(new Sin(copy(eq)));
	EquationBase* eq_deriv = eq->__derivative__(var);
	return new Mult({minus_sin_func, eq_deriv});
}

EquationBase* Cos::__copy_equation_base__() const{
	const Cos* casted = dynamic_cast<const Cos*>(this);
	return new Cos(*casted);
}

void Cos::__delete_equation_base__(){
	Cos* casted = dynamic_cast<Cos*>(this);
	delete casted;
}












Equation exp(const Equation eq){
	return Equation(new Exp(copy(eq.eq)));
}

Equation ln(const Equation eq){
	return Equation(new Ln(copy(eq.eq)));
}

Equation log(const Equation eq, const Equation base){
	return Equation(new Log(copy(eq.eq), copy(base.eq)));
}

Equation pow(const Equation base, const Equation power){
	return Equation(new Power(copy(base.eq), copy(power.eq)));
}

Equation abs(const Equation eq){
	return Equation(new Abs(copy(eq.eq)));
}

Equation sin(const Equation eq){
	return Equation(new Sin(copy(eq.eq)));
}

Equation cos(const Equation eq){
	return Equation(new Cos(copy(eq.eq)));
}


Equation::Equation(EquationBase* make_eq) {
	if(SYMCALC_AUTO_SIMPLIFY){
		eq = make_eq->__simplify__();
		delete_equation_base(make_eq);
	}else{
		eq = make_eq;
	}
}


Equation::Equation(SYMCALC_VALUE_TYPE num){
	this->eq = new EquationValue(num);
}

Equation::Equation(SYMCALC_VAR_NAME_TYPE const_name, SYMCALC_VALUE_TYPE value){
	this->eq = new Constant(const_name, value);
}

std::ostream& operator<<(std::ostream &stream, const Equation equation){
	stream << equation.eq->txt();
	return stream;
}


Equation operator+(const Equation eq1, const Equation eq2){
	return Equation(new Sum({copy(eq1.eq), copy(eq2.eq)}));
}

Equation operator-(const Equation eq1, const Equation eq2){
	return Equation(new Sum({copy(eq1.eq), new Negate(copy(eq2.eq))}));
}

Equation operator*(const Equation eq1, const Equation eq2){
	return Equation(new Mult({copy(eq1.eq), copy(eq2.eq)}));
}

Equation operator/(const Equation eq1, const Equation eq2){
	return Equation(new Div(copy(eq1.eq), copy(eq2.eq)));
}

Equation operator-(const Equation eq1){
	return Equation(new Negate(copy(eq1.eq)));
}



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
 

bool operator<(const Equation holder1, const Equation holder2){
	return true;
}





Equation::~Equation(){
	delete_equation_base(eq);
}






Equation::Equation(const Equation& lvalue) : eq(nullptr){
	
	const EquationBase* lvalue_eq = lvalue.eq;	

	
	eq = copy(lvalue_eq);
	
}


Equation::Equation(SYMCALC_VAR_NAME_TYPE var_name) : eq(nullptr){
	eq = new Variable(var_name);
}




std::vector<Equation> Equation::list_variables() const{
	std::vector<SYMCALC_VAR_NAME_TYPE> vars = eq->list_variables();
	
	std::vector<Equation> converted;
	
	for(SYMCALC_VAR_NAME_TYPE var : vars){
		converted.push_back(Equation(var));
	}
	
	return converted;
	
}






EquationBase* copy(const EquationBase* start_eq){
	return start_eq->__copy_equation_base__();
}



Equation Equation::pow(Equation power) const{
	EquationBase* copy1 = copy(this->eq);
	EquationBase* copy2 = copy(power.eq);
	return Equation(new Power(copy1, copy2));
}




Equation Equation::simplify() const{
	return Equation(eq->__simplify__());
}



std::vector<EquationBase*> copy(std::vector<const EquationBase*> eqs){
	std::vector<EquationBase*> vec;
	vec.reserve(eqs.size());
	for(const EquationBase* eq : eqs){
		vec.push_back(copy(eq));
	}
	return vec;
}




void delete_equation_base(EquationBase* eq){
	if(eq == nullptr) return;
	
	eq->__delete_equation_base__();
	eq = nullptr;
}


// Constants

namespace Constants{
	Equation Pi ("pi", M_PI);
	Equation E ("e", std::exp(1.0));
}


}