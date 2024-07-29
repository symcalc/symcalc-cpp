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

//
// functions.cpp:
// Defines outside functions that help create expressions, e.g. exp, ln and others
//

namespace symcalc{

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

} // End of symcalc namespace
