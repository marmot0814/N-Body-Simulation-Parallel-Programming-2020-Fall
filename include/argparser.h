#ifndef _ARG_ARGPARSER_H_
#define _ARG_ARGPARSER_H_

#include <boost/program_options.hpp>
namespace BPO = boost::program_options;

#include <iostream>
using namespace std;
BPO::variables_map argparser(int, char **);

#endif  // #define _ARG_ARGPARSE_H_
