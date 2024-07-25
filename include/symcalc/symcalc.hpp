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

#ifndef SYMCALC_H
#define SYMCALC_H


#define SYMCALC_VALUE_TYPE double

#include <string>
#define SYMCALC_VAR_NAME_TYPE std::string

#include <map>
#define SYMCALC_VAR_HASH_TYPE std::map<SYMCALC_VAR_NAME_TYPE, SYMCALC_VALUE_TYPE>


#include <iostream>
#include <vector>
#include <cmath>


namespace symcalc{

extern bool SYMCALC_AUTO_SIMPLIFY;


template<typename T>
bool include(std::vector<T> vec, T element){
	for(T el : vec){
		if(el == element){
			return true;
		}
	}
	return false;
}


class EquationBase{
public:
	std::string type;
	
	
	EquationBase(std::string el_type);
	EquationBase(const EquationBase& lvalue);
	~EquationBase();
	
	virtual std::string txt() const {return "";};
	virtual SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const {return 0.0;};
	virtual EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const {return nullptr;};
	
	virtual std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const{return std::vector<SYMCALC_VAR_NAME_TYPE>();};
	
	virtual EquationBase* __simplify__() const;
	
	virtual EquationBase* __copy_equation_base__() const = 0;
	virtual void __delete_equation_base__() = 0;
};



class Variable : public EquationBase{
public:
	
	SYMCALC_VAR_NAME_TYPE name;
	
	Variable(SYMCALC_VAR_NAME_TYPE name);
	Variable(const Variable& lvalue);
	
	~Variable();
	
	std::string txt() const override;
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;
	
	EquationBase* __simplify__() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};



class EquationValue : public EquationBase{
public:
	SYMCALC_VALUE_TYPE value;
	std::string ready_txt;
	
	EquationValue(SYMCALC_VALUE_TYPE value);
	EquationValue(const EquationValue& lvalue);
	
	~EquationValue();
	
	std::string txt() const override;
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;
	
	EquationBase* __simplify__() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};


class Constant : public EquationValue{
public:
	SYMCALC_VAR_NAME_TYPE name;

	Constant(SYMCALC_VAR_NAME_TYPE name, SYMCALC_VALUE_TYPE value);
	Constant(const Constant& lvalue);
	
	std::string txt() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};


EquationBase* to_equation(SYMCALC_VALUE_TYPE num);


class Sum : public EquationBase{
public:
	std::vector<EquationBase*> elements;
	std::string ready_txt;
	
	Sum(std::vector<EquationBase*> elements);
	Sum(const Sum& lvalue);

	~Sum();

	std::string txt() const override;
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;
	
	EquationBase* __simplify__() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};


class Negate : public EquationBase{
public:
	EquationBase* eq;
	std::string ready_txt;
	
	Negate(EquationBase* eq);
	Negate(const Negate& lvalue);

	~Negate();

	std::string txt() const override;
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;
	
	EquationBase* __simplify__() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};



class Mult : public EquationBase{
public:
	std::vector<EquationBase*> elements;
	std::string ready_txt;
	
	Mult(std::vector<EquationBase*> elements);
	Mult(const Mult& lvalue);

	~Mult();

	std::string txt() const override;
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;
	
	EquationBase* __simplify__() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};



class Div : public EquationBase{
public:
	EquationBase* dividend;
	EquationBase* divisor;
	std::string ready_txt;
	
	Div(EquationBase* dividend, EquationBase* divisor);
	Div(const Div& lvalue);

	~Div();

	std::string txt() const override;
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;

	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;
	
	EquationBase* __simplify__() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};




class Power : public EquationBase{
public:
	
	EquationBase* base;
	EquationBase* power;
	std::string ready_txt;
	
	Power(EquationBase* base, EquationBase* power);
	Power(const Power& lvalue);

	~Power();

	std::string txt() const override;
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;
	
	EquationBase* __simplify__() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};


class Log : public EquationBase{
public:
	EquationBase* base;
	EquationBase* eq;
	std::string ready_txt;
	
	Log(EquationBase* eq, EquationBase* base);
	Log(const Log& lvalue);
	
	~Log();
	
	std::string txt() const override;
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;
	
	EquationBase* __simplify__() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};


class Ln : public EquationBase{
public:

	EquationBase* eq;
	std::string ready_txt;
	
	Ln(EquationBase* eq);
	Ln(const Ln& lvalue);

	~Ln();

	std::string txt() const override;
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;
	
	EquationBase* __simplify__() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};




class Exp: public EquationBase{
public:

	EquationBase* eq;
	
	Exp(EquationBase* eq);
	Exp(const Exp& lvalue);

	~Exp();
	
	std::string txt() const override;
	
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;
	
	EquationBase* __simplify__() const override;
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
};



class Abs : public EquationBase{
public:
	
	EquationBase* insides;
	
	Abs(EquationBase* insides);
	Abs(const Abs& lvalue);
	
	~Abs();
	
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
	
	std::string txt() const override;
	
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;

	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	EquationBase* __simplify__() const override;
};






class Sin : public EquationBase{
public:
	
	EquationBase* eq;
	
	Sin(EquationBase* eq);
	Sin(const Sin& lvalue);
	
	~Sin();
	
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
	
	std::string txt() const override;
	
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;

	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	EquationBase* __simplify__() const override;
};




class Cos : public EquationBase{
public:
	
	EquationBase* eq;
	
	Cos(EquationBase* eq);
	Cos(const Cos& lvalue);
	
	~Cos();
	
	
	EquationBase* __copy_equation_base__() const override;
	void __delete_equation_base__() override;
	
	std::string txt() const override;
	
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const override;
	
	std::vector<SYMCALC_VAR_NAME_TYPE> list_variables() const override;

	EquationBase* __derivative__(SYMCALC_VAR_NAME_TYPE var) const override;
	
	EquationBase* __simplify__() const override;
};








EquationBase* copy(const EquationBase* start_eq);
std::vector<EquationBase*> copy(std::vector<const EquationBase*> start_eq);


void delete_equation_base(EquationBase* eq);



class Equation{
public:
	EquationBase* eq;
	
	Equation(EquationBase* equation);
	Equation(Variable var);
	Equation(SYMCALC_VALUE_TYPE value);
	Equation(SYMCALC_VAR_NAME_TYPE var_name);
	Equation(SYMCALC_VAR_NAME_TYPE const_name, SYMCALC_VALUE_TYPE value);
	Equation(const Equation& lvalue);
	
	friend std::ostream& operator<<(std::ostream &stream, const Equation equation);
	friend bool operator<(const Equation equation1, const Equation equation2);
	friend Equation operator+(const Equation eq1, const Equation eq2);
	friend Equation operator-(const Equation eq1, const Equation eq2);
	friend Equation operator*(const Equation eq1, const Equation eq2);
	friend Equation operator/(const Equation eq1, const Equation eq2);
	friend Equation operator-(const Equation eq1);
	
	
	
	Equation pow(Equation power) const;
	
	Equation derivative(Equation variable, size_t order = 1) const;
	Equation derivative(size_t order=1) const;
	
	Equation simplify() const;
	
	std::vector<Equation> list_variables() const;
	
	SYMCALC_VALUE_TYPE eval(SYMCALC_VAR_HASH_TYPE var_hash) const;
	SYMCALC_VALUE_TYPE eval(std::map<Equation, SYMCALC_VALUE_TYPE> var_hash) const;
	SYMCALC_VALUE_TYPE eval() const;
	SYMCALC_VALUE_TYPE operator()(SYMCALC_VAR_HASH_TYPE var_hash) const;
	SYMCALC_VALUE_TYPE operator()(std::map<Equation, SYMCALC_VALUE_TYPE> var_hash) const;
	SYMCALC_VALUE_TYPE operator()() const;

	std::string type() const{
		return eq->type;
	}

	~Equation();
};



Equation exp(const Equation eq);
Equation ln(const Equation eq);
Equation log(const Equation eq, const Equation base);
Equation pow(const Equation base, const Equation power);
Equation abs(const Equation eq);
Equation sin(const Equation eq);
Equation cos(const Equation eq);



// Constants

namespace Constants{
	extern Equation Pi;
	extern Equation E;
}

}


#endif