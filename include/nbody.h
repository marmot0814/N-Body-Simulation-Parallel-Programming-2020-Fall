#pragma once

#include <boost/program_options.hpp>
namespace BPO = boost::program_options;

#include <vector>
using namespace std;

#include "particle.h"
#include "displayer.h"
#include "physicalengine.h"

class NBodyEnv {
    vector<Particle> particles;
    Displayer displayer;
    PhysicalEngine engine;
public:
    NBodyEnv(const BPO::variables_map&);
    void run();
};
