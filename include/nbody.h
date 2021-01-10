#pragma once

#include <boost/program_options.hpp>
namespace BPO = boost::program_options;

#include <vector>
using namespace std;

#include "particle.h"

class NBodyEnv {
    vector<Particle> particles;
public:
    NBodyEnv(BPO::variables_map &args);
    void run();
};
