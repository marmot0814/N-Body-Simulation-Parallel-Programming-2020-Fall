#include "argparser.h"

BPO::variables_map argparser(int argc, char **argv) {
    BPO::options_description bOptions("nbody");
    bOptions.add_options()(
        "help,h",
        "produce help message"
    )(
        "number,n",
        BPO::value<int>()
            ->default_value(1000)
            ->value_name("number(int)"),
        "number of particles"
    )(
        "mass,m",
        BPO::value<double>()
            ->default_value(0.01)
            ->value_name("mass(double)"),
        "mass of particles"
    )(
        "color,r",
        BPO::value<vector<double>>()
            ->multitoken()
            ->default_value(
                vector<double>{0.0, 1.0, 1.0},
                "0.0 1.0 1.0"
            )
            ->value_name("R(double)[0, 1] G(double)[0, 1] B(double)[0, 1]"),
        "color of particles"
    )(
        "window,d",
        BPO::value<vector<int>>()
            ->multitoken()
            ->default_value(
                vector<int>{800, 600},
                "800 600"
            )
            ->value_name("height(int) width(int)"),
        "size of window"
    )(
        "fullscreen,f",
        BPO::bool_switch()
            ->default_value(false),
        "enable full screen"
    );

    BPO::variables_map mVMap;
    BPO::store(BPO::parse_command_line(argc, argv, bOptions), mVMap);
    BPO::notify(mVMap);

    if (mVMap.count("help")) {
        cout << bOptions << endl;
        exit(1);
    }

    return mVMap;
}
