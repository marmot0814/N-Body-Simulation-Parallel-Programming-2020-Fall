TARGET			    = nbody
SRC_DIR			    = src/
INC_DIR			    = include/

CC				    = g++-10 -std=c++17
CCFLAGS			    = -Iinclude -Ofast -Wall -fPIC -lm -fopenmp -g3 -mavx2 
GLFLAGS			    = -lGL -lGLEW -lglfw -ltbb -lSOIL -lm

NVCC			    = nvcc
CUDA_LINK_FLAGS	    = -rdc=true -gencode=arch=compute_50,code=sm_50 -Xcompiler '-fPIC' -lgomp
CUDA_COMPILE_FLAGS  = --device-c -gencode=arch=compute_50,code=sm_50 -Xcompiler '-fPIC' -O3

COMMFLAGS           = $(GLFLAGS) -lboost_program_options

SRC				    = $(wildcard $(SRC_DIR)*.cpp)
OBJ				    = $(SRC:%.cpp=%.o)

.PHONY: clean

$(TARGET):  $(OBJ)
	$(NVCC) $^ $(CUDA_LINK_FLAGS) $(COMMFLAGS) -o $@

%.o : %.cpp
	$(CC) $< $(CCFLAGS) $(COMMFLAGS) -c -o $@

%.o : %.cu
	$(NVCC) $< $(CUDA_COMPILE_FLAGS) $(COMMFLAGS) -c -o $@

clean:
	rm -rf $(OBJ) $(TARGET)
