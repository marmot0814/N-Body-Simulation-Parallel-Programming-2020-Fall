#include "nbody.h"

NBodyEnv::NBodyEnv(const BPO::variables_map &args)
    : displayer(Displayer(args)), engine(PhysicalEngine()) {

    int n = args["number"].as<int>();
    double m = args["mass"].as<double>();

    particles.resize(n);
    for (int i = 0 ; i < n ; i++)
        particles[i] = Particle(m);
}
void NBodyEnv::run() {

    while (!displayer.stop()) {
        if (displayer.getRestart()) {
            displayer.setRestart(false);
            for (int i = 0 ; i < particles.size() ; i++)
                particles[i] = Particle(particles[i].mass);
        }
        displayer.update(particles);
        engine.next(particles, displayer.getComputeMode(), displayer.getDeltaTime());
    }

}
