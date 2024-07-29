# SymCalc C++

![SymCalc Logo](/symcalc_logo.png)

[Website](https://symcalc.site/ruby)
/
[License: Apache-2.0](http://www.apache.org/licenses/LICENSE-2.0)
/
[Changelog](https://symcalc.site/cpp/changelog)
/
[Ruby](https://github.com/symcalc/symcalc-ruby)
, 
[C++](https://github.com/symcalc/symcalc-cpp)

SymCalc (which stands for **Sym**bolic **Calc**ulus) is a library that introduces mathematics to code, where you can declare, evaluate, and differentiate any possible maths function with a single call.

SymCalc allows to write readable and flexible code, adding a lot of functionality along the way, like this:
```cpp
Equation fx = 5 * pow(x, 2) + sin(x);
```
Instead of hard-coded functions like this:
```cpp
#include <cmath>
double fx(double x){
	return 5 * pow(x, 2) + sin(x);
}
```

## Contents
- [Example](#example)
- [Basic usage](#basic-usage)
- [Install](#install-with-make)
- [Learning SymCalc](#learning-symcalc)
- [Authors](#authors)

## Example

```cpp
#include <symcalc/symcalc.hpp>

using namespace symcalc;

int main(){
    // SymCalc variable
    Equation x ("x");

    // SymCalc function
    Equation fx = pow(x, 2) * 5 - 4 * sin(exp(x));

    // SymCalc derivative
    Equation dfdx = fx.derivative();

    // SymCalc evaluate
    double value = dfdx.eval({{x, 5}});

    std::cout << value << std::endl;

    return 0;
}
```

## Basic usage

1. Include the SymCalc header:
```cpp
#include <symcalc/symcalc.hpp>
using namespace symcalc;
```

2. Define a variable:
```cpp
Equation x("x");
```

3. Define a function:
```cpp
Equation fx = pow(x, 2);
```

4. Evaluate:
```cpp
double value = fx.eval({{x, 4}});
// or
value = fx({{x, 4}});
```

5. Differentiate:
```cpp
Equation dfdx = fx.derivative();
```

6. Multi-variable!:
```cpp
Equation x("x");
Equation y("y");

Equation fxy = pow(x, 2) - 4 * abs(y);

Equation dfdx = fxy.derivative(x);
Equation dfdy = fxy.derivative(y);
```

7. Display:
```cpp
std::cout << fx << std::endl; // Prints the function
```

8. Compile:
```bash
g++ main.cpp -o main -std=c++11 -lsymcalc
```

9. See more on the [website](https://symcalc.site/cpp)!

## Install with make

1. Download the source code with git or wget:
```bash
git clone https://github.com/symcalc/symcalc-cpp; cd symcalc-cpp
```

or

```bash
wget https://symcalc.site/cpp/source_latest.zip; unzip source_latest.zip; cd symcalc_cpp_source
```

2. Build SymCalc with make
```bash
make
```

3. Install SymCalc
```bash
make install
```
Or to a specific location with
```bash
make install PREFIX=/path/to/install
```

## Learning SymCalc

You can learn more about SymCalc on these resources:

- [SymCalc's Website](https://symcalc.site/cpp)
- [Intro](https://symcalc.site/cpp/intro)
- [Examples](https://symcalc.site/cpp/examples)
- [Docs](https://symcalc.site/cpp/docs)


## Authors

SymCalc is currently developed and maintaned by [Kyryl Shyshko](https://kyrylshyshko.me) ([@kyryloshy](https://github.com/kyryloshy))
