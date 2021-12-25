#define _CRT_SECURE_NO_WARNINGS
#include <cstdarg>
#include <cassert>
#include <cinttypes>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <typeindex>

#include "static.h"

//sub(mul(3,add(1,2)),div(5,add(1,4))
//3*(1 + 1) - 5/(1+4)

const bool b = (4 <= 1) <= 2;

int main(int argc, const char* argv[]) {
    
    
    parse_test();
    return 0;
}