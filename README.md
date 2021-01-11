[![Build Status](https://travis-ci.org/marmot0814/N-Body-Simulation-Parallel-Programming-2020-Fall.svg?branch=master)](https://travis-ci.org/marmot0814/N-Body-Simulation-Parallel-Programming-2020-Fall)
# N Body Simulation
A final project in 2020 Fall NCTU Parallel Programming based on [this repo](https://github.com/matb4r/n-body) with OpenMP and CUDA version.
- [report]()
- [slide](https://docs.google.com/presentation/d/1xL9ga6e59owWczONMR_5e4UecDjnTiF__IqyikZfgmI/edit#slide=id.gb3c27cc280_0_21)

## Structure
```
N-Body-Simulation-Parallel-Programming-2020-Fall/
├── data
│   ├── fragment.glsl
│   ├── particle.png
│   └── vertex.glsl
├── include
│   ├── argparser.h
│   ├── camera.h
│   ├── displayer.h
│   ├── nbody.h
│   ├── particle.h
│   ├── physicalengine.h
│   └── shader.h
├── lib
│   ├── GLEW
│   ├── GLFW
│   ├── GLM
│   └── SOIL
├── Makefile
├── README.md
└── src
    ├── argparser.cpp
    ├── camera.cpp
    ├── displayer.cpp
    ├── main.cpp
    ├── nbody.cpp
    ├── particle.cpp
    ├── physicalengine.cpp
    ├── physicalengineKernel.cu
    └── shader.cpp
```
## Development Environment
- OS: Ubuntu 20.04
- CPU: Intel(R) Core(TM) i7-8700 CPU @ 3.20GHz 6 cores 12 threads
- GPU: GeForce GTX 1080 Ti

## Requirement
- C++
- OpenCL
    - libboost-all-dev
    - libglfw3-dev
    - libsoil-dev
    - libglew-dev
    - nvidia-cuda-toolkit
- make
- CUDA 10.1

## Build
### Clone this repo
```
git clone https://github.com/marmot0814/N-Body-Simulation-Parallel-Programming-2020-Fall.git
cd N-Body-Simulation-Parallel-Programming-2020-Fall/ && make
```
## Usage
```
nbody:
  -h [ --help ]                         produce help message
  -n [ --number ] number(int) (=1000)   number of particles
  -m [ --mass ] mass(double) (=0.01)    mass of particles
  -r [ --color ] R(double)[0, 1] G(double)[0, 1] B(double)[0, 1] (=0.0 1.0 1.0)
                                        color of particles
  -d [ --window ] height(int) width(int) (=800 600)
                                        size of window
  -f [ --fullscreen ]                   enable full screen
```
### Keyboard
| Key        | Effect           |
| ------------- |-------------|
|W|Move the camera forward|
|S|Move the camera backward|
|A|Move the camera left|
|D|Move the camera right|
|SPACE|New particles|
|F1|Sequential computing mode|
|F2|SIMD computing mode|
|F3|OpenMP computing mode|
|F4|SIMD OpenMP computing mode|
|F5|CUDA computing mode|
|F6|CUDA* computing mode|
|ESC|Close simulator|

## demo
![](https://i.imgur.com/oqYEH1C.gif)
