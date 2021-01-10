TARGET			    = nbody
SRC_DIR			    = src/
INC_DIR			    = include/
OBJ_DIR             = objs/

CC				    = g++ -std=c++17
CCFLAGS			    = -Iinclude -Ilib -Ofast -Wall -fPIC -lm -fopenmp -g3 -mavx2
GLFLAGS			    = -lGL -lGLEW -lglfw -lSOIL -lm

NVCC			    = nvcc
CUDA_LINK_FLAGS	    = -rdc=true -gencode=arch=compute_50,code=sm_50 -Xcompiler '-fPIC' -lgomp
CUDA_COMPILE_FLAGS  = --device-c -gencode=arch=compute_50,code=sm_50 -Xcompiler '-fPIC' -O3

COMMFLAGS           = $(GLFLAGS) -lboost_program_options

SRC				    = $(shell find src/ -name '*.cpp' -type f -printf "%f\n")
OBJ				    = $(SRC:%.cpp=$(OBJ_DIR)%.o)

.PHONY: clean dirs

all: $(shell mkdir -p $(OBJ_DIR)) $(TARGET)

$(TARGET): $(OBJ)
	$(NVCC) $^ $(CUDA_LINK_FLAGS) $(COMMFLAGS) -o $@

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	$(CC) $< $(CCFLAGS) $(COMMFLAGS) -c -o $@

%.o : %.cu
	$(NVCC) $< $(CUDA_COMPILE_FLAGS) $(COMMFLAGS) -c -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
