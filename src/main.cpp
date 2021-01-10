#include <iostream>

#include "argparser.h"
using namespace std;

#include "nbody.h"

int main(int argc, char **argv) {
    auto args = argparser(argc, argv);
    NBodyEnv(args).run();
}
