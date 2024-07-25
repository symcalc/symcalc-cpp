# SymCalc C++

![SymCalc Logo](/symcalc_logo.png)

SymCalc is a library that introduces mathematics to code, where you can declare, evaluate, and differentiate any possible maths function with a single call.

[Website](https://symcalc.site/cpp)
/
[License: Apache-2.0](http://www.apache.org/licenses/LICENSE-2.0)
/
[Languages: Ruby and C++]

## Contents
- [Basic example](#basic-example)
- [Install](#install-with-make)
- [Learning SymCalc](#learning-symcalc)
- [Contact](#contact)

## Basic example
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


## Install with make

1. *Download the source code with git or wget:*
```bash
git clone https://github.com/symcalc/symcalc-cpp; cd symcalc-cpp
```

or

```bash
wget https://symcalc.site/cpp/source_latest.zip; unzip source_latest.zip; cd symcalc_cpp_source
```

2. *Build SymCalc with make*
```bash
make
```

3. *Install SymCalc*
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

SymCalc is currently developed and maintaned by [Kyryl Shyshko](https://kyrylshyshko.me)(@kyryloshy)
