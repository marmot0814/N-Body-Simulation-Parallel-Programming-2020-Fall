#pragma once

#include "particle.h"
#include "displayer.h"

#include <vector>
using namespace std;

class PhysicalEngine {
public:
    void next(vector<Particle> &, COMPUTE_MODE);
};
