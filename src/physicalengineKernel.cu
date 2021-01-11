#include <cuda.h>
#include "physicalengine.h"


__global__ void calculateForceKernelFast(Particle* p, int N) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= N) return;

    const float mass = p[idx].mass;
    const float mass2 = mass * mass;
    glm::vec3 F{};
    for (int i = idx, j = 0; j < N; ++j) {
        if (i == j) continue;
        glm::vec3 location_difference = p[j].location - p[i].location;
        float distance = glm::length(location_difference) + 0.1f;
        // printf("%f\n", distance);
        F += location_difference / (distance * distance * distance);
    }
    p[idx].force = F * mass2;
}

__global__ void calculateForceKernel(Particle* p, int N) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= N) return;

    for (int i = idx, j = 0; j < N; ++j) {
        if (i == j) continue;
        glm::vec3 location_difference = p[j].location - p[i].location;
        float distance = glm::length(location_difference) + 0.1f;
        p[idx].force += p[i].mass * p[j].mass * location_difference / (distance * distance * distance);
    }
}

__global__ void updatePositionKernel(Particle* p, int N, float delta_time) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= N) return;
    int i = idx;
    p[i].acceleration = p[i].force / p[i].mass;
    p[i].velocity += p[i].acceleration * delta_time;
    p[i].location += p[i].velocity * delta_time;
    p[i].force = glm::vec3(0, 0, 0);
}

Particle* cu_p = nullptr;

void PhysicalEngine::nextCUDA(vector<Particle> &p, GLfloat delta_time) {
    static Particle *cu_p = nullptr;
    int N = p.size();
    if (!cu_p)
        cudaMalloc((void**)&cu_p, N * sizeof(Particle));
    int M = 32;
    cudaMemcpy(cu_p, &p[0], N * sizeof(Particle), cudaMemcpyHostToDevice);
    calculateForceKernel<<<N / M + 1, M>>>(cu_p, N);
    updatePositionKernel<<<N / M + 1, M>>>(cu_p, N, delta_time);
    cudaMemcpy(&p[0], cu_p, N * sizeof(Particle), cudaMemcpyDeviceToHost);
    // cudaFree(cu_p);
}

void PhysicalEngine::nextCUDAFast(vector<Particle> &p, GLfloat delta_time) {
    static Particle *cu_p = nullptr;
    int N = p.size();
    if (!cu_p)
        cudaMalloc((void**)&cu_p, N * sizeof(Particle));
    int M = 32;
    cudaMemcpy(cu_p, &p[0], N * sizeof(Particle), cudaMemcpyHostToDevice);
    calculateForceKernelFast<<<N / M + 1, M>>>(cu_p, N);
    updatePositionKernel<<<N / M + 1, M>>>(cu_p, N, delta_time);
    cudaMemcpy(&p[0], cu_p, N * sizeof(Particle), cudaMemcpyDeviceToHost);
    // cudaFree(cu_p);
}
