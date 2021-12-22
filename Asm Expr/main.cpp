#define _CRT_SECURE_NO_WARNINGS
#include <cstdarg>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <list>

#include "static.h"

//sub(mul(3,add(1,2)),div(5,add(1,4))
//3*(1 + 1) - 5/(1+4)

int main(int argc, const char* argv[]) {
    parse_test();
    return 0;
}