#pragma once

#include <boost/program_options.hpp>
namespace BPO = boost::program_options;

#include <iostream>
using namespace std;
BPO::variables_map argparser(int, char **);
