TARGET			    = nbody
SRC_DIR			    = src/
INC_DIR			    = include/
OBJ_DIR             = objs/

CC				    = g++-10 -std=c++17
CCFLAGS			    = -Iinclude -Ilib -Ofast -Wall -fPIC -lm -fopenmp -g3 -mavx2
GLFLAGS			    = -lGL -lGLEW -lglfw -lSOIL -lm

NVCC			    = nvcc
CUDA_LINK_FLAGS	    = -rdc=true -gencode=arch=compute_50,code=sm_50 -Xcompiler '-fPIC' -lgomp
CUDA_COMPILE_FLAGS  = -Iinclude -Ilib --device-c -gencode=arch=compute_50,code=sm_50 -Xcompiler '-fPIC' -O3

COMMFLAGS           = $(GLFLAGS) -lboost_program_options

CPP_SRC				= $(shell find src/ -name '*.cpp' -type f -printf "%f\n")
CU_SRC				= $(shell find src/ -name '*.cu' -type f -printf "%f\n")
SRC                 = $(CPP_SRC) $(CU_SRC)

CPP_OBJ				= $(CPP_SRC:%.cpp=$(OBJ_DIR)%.o)
CU_OBJ				= $(CU_SRC:%.cu=$(OBJ_DIR)%.o)
OBJ                 = $(CPP_OBJ) $(CU_OBJ)

.PHONY: clean dirs

all: $(shell mkdir -p $(OBJ_DIR)) $(TARGET)

$(TARGET): $(OBJ)
	$(NVCC) $^ $(CUDA_LINK_FLAGS) $(COMMFLAGS) -o $@

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	$(CC) $< $(CCFLAGS) $(COMMFLAGS) -c -o $@

$(OBJ_DIR)%.o : $(SRC_DIR)%.cu
	$(NVCC) $< $(CUDA_COMPILE_FLAGS) $(COMMFLAGS) -c -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
