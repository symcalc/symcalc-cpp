# SymCalc C++

![SymCalc Logo](/symcalc_logo.png)

SymCalc is a library that introduces mathematics to code, where you can declare, evaluate, and differentiate any possible maths function with a single call.

[Website](https://symcalc.site/cpp)
/
[License: Apache-2.0](http://www.apache.org/licenses/LICENSE-2.0)
/
[Ruby](https://github.com/symcalc/symcalc-ruby)
, 
[C++](https://github.com/symcalc/symcalc-cpp)

## Contents
- [Basic example](#basic-example)
- [Install](#install-with-make)
- [Learning SymCalc](#learning-symcalc)
- [Authors](#authors)

## Example
```cpp
#include <symcalc/symcalc.hpp>

using namespace symcalc;

int main(){
    # SymCalc variable
    Equation x ("x");

    # SymCalc function
    Equation fx = pow(x, 2) * 5 - 4 * sin(exp(x));

    # SymCalc derivative
    Equation dfdx = fx.derivative();

    # SymCalc evaluate
    double value = dfdx.eval({{x, 5}});

    std::cout << value << std::endl;
}
```

## Basic usage

1. Define a variable:
```cpp
Equation x("x");
```

2. Define a function:
```cpp
Equation fx = pow(x, 2);
```

3. Evaluate:
```cpp
double value = fx.eval({{x, 4}});
// or
value = fx({{x, 4}});
```

4. Differentiate:
```cpp
Equation dfdx = fx.derivative();
```

5. Multi-variable!:
```cpp
Equation x("x");
Equation y("y");

Equation fxy = pow(x, 2) - 4 * abs(y);

Equation dfdx = fxy.derivative(x);
Equation dfdy = fxy.derivative(y);
```

6. See more on the [website](https://symcalc.site/cpp)!

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

## Learning SymCalc

You can learn more about SymCalc on these resources:

- [SymCalc's Website](https://symcalc.site/cpp)
- [Intro](https://symcalc.site/cpp/intro)
- [Examples](https://symcalc.site/cpp/examples)
- [Docs](https://symcalc.site/cpp/docs)


## Authors

SymCalc is currently developed and maintaned by [Kyryl Shyshko](https://kyrylshyshko.me) ([@kyryloshy](https://github.com/kyryloshy))
