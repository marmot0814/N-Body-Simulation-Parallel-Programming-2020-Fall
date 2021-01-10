#include "physicalengine.h"

void PhysicalEngine::next(vector<Particle> &p, COMPUTE_MODE compute_mode, GLfloat delta_time) {
    switch(compute_mode) {
        case COMPUTE_MODE::SERIAL:
            nextSerial(p, delta_time);
            break;
        case COMPUTE_MODE::SIMD:
            nextSIMD(p, delta_time);
            break;
        case COMPUTE_MODE::OPENMP:
            nextOpenMP(p, delta_time);
            break;
        case COMPUTE_MODE::SIMD_OPENMP:
            nextSIMDOpenMP(p, delta_time);
            break;
        case COMPUTE_MODE::CUDA:
            nextCUDA(p, delta_time);
            break;
        case COMPUTE_MODE::CUDA_FAST:
            nextCUDAFast(p, delta_time);
        default:
            break;
    }
}

void PhysicalEngine::nextSerial(vector<Particle> &p, GLfloat delta_time) {
    int N = p.size();
    for (int i = 0 ; i < N ; i++)
        calculateOneForce(i, p, delta_time);
    for (int i = 0 ; i < N ; i++)
        updateOnePosition(i, p, delta_time);
}

void PhysicalEngine::nextOpenMP(vector<Particle> &p, GLfloat delta_time) {
    int N = p.size();
#pragma omp parallel for
    for (int i = 0 ; i < N ; i++)
        calculateOneForce(i, p, delta_time);
#pragma omp parallel for
    for (int i = 0 ; i < N ; i++)
        updateOnePosition(i, p, delta_time);
}

void PhysicalEngine::nextSIMD(vector<Particle> &p, GLfloat delta_time) {
    static float *x = nullptr, *y, *z;
    int N = p.size();
    if (!x) {
        x = (float*)malloc((N + 16) * sizeof(float));
        y = (float*)malloc((N + 16) * sizeof(float));
        z = (float*)malloc((N + 16) * sizeof(float));
    }
    for (int i = 0; i < N; i++) {
        x[i] = p[i].location.x;
        y[i] = p[i].location.y;
        z[i] = p[i].location.z;
    }
    for (int i = 0 ; i < N ; i++)
        calculateOneForceSIMD(i, p, delta_time, x, y, z);
    for (int i = 0 ; i < N ; i++)
        updateOnePosition(i, p, delta_time);
}

void PhysicalEngine::nextSIMDOpenMP(vector<Particle> &p, GLfloat delta_time) {
    static float *x = nullptr, *y, *z;
    int N = p.size();
    if (!x) {
        x = (float*)malloc((N + 16) * sizeof(float));
        y = (float*)malloc((N + 16) * sizeof(float));
        z = (float*)malloc((N + 16) * sizeof(float));
    }
    for (int i = 0; i < N; i++) {
        x[i] = p[i].location.x;
        y[i] = p[i].location.y;
        z[i] = p[i].location.z;
    }
#pragma omp parallel for
    for (int i = 0 ; i < N ; i++)
        calculateOneForceSIMD(i, p, delta_time, x, y, z);
#pragma omp parallel for
    for (int i = 0 ; i < N ; i++)
        updateOnePosition(i, p, delta_time);
}

void PhysicalEngine::calculateOneForce(int i, vector<Particle> &p, GLfloat delta_time) {
    int N = p.size();
    for (int j = 0; j < N; j++) {
        if (i == j) continue;
        glm::vec3 location_difference = p[j].location - p[i].location;
        float distance = glm::length(location_difference) + 0.1f;
        glm::vec3 force = (p[i].mass * p[j].mass) *
                          location_difference /
                          (distance * distance * distance);

        p[i].force += force;
    }
}

void PhysicalEngine::updateOnePosition(int i, vector<Particle> &p, GLfloat delta_time) {
    p[i].acceleration = p[i].force / p[i].mass;
    p[i].velocity += p[i].acceleration * delta_time;
    p[i].location += p[i].velocity * delta_time;
    p[i].force = glm::vec3(0, 0, 0);
}

void PhysicalEngine::calculateOneForceSIMD(int i, vector<Particle> &p, GLfloat delta_time, float *x, float *y, float *z) {

    const __m256i allO = _mm256_set1_epi8(-1);
    const __m256 allEps = _mm256_set1_ps(0.1);
    const __m256 allZf = _mm256_set1_ps(0.0);
    const float mass2 = p[i].mass * p[i].mass;

    __m256 curX = _mm256_set1_ps(x[i]);
    __m256 curY = _mm256_set1_ps(y[i]);
    __m256 curZ = _mm256_set1_ps(z[i]);
    __m256 FX = allZf;
    __m256 FY = allZf;
    __m256 FZ = allZf;
    int N = p.size();
    for (int j = 0; j < N; j += 8) {
        __m256 locX = _mm256_maskload_ps(x + j, allO);
        __m256 locY = _mm256_maskload_ps(y + j, allO);
        __m256 locZ = _mm256_maskload_ps(z + j, allO);

        __m256 difX = _mm256_sub_ps(locX, curX);
        __m256 difY = _mm256_sub_ps(locY, curY);
        __m256 difZ = _mm256_sub_ps(locZ, curZ);

        __m256 len2 = _mm256_mul_ps(difX, difX);
        len2 = _mm256_add_ps(len2, _mm256_mul_ps(difY, difY));
        len2 = _mm256_add_ps(len2, _mm256_mul_ps(difZ, difZ));

        __m256 len = _mm256_add_ps(_mm256_sqrt_ps(len2), allEps);

        __m256 len3 = _mm256_mul_ps(_mm256_mul_ps(len, len), len);

        __m256 forceX = _mm256_div_ps(difX, len3);
        __m256 forceY = _mm256_div_ps(difY, len3);
        __m256 forceZ = _mm256_div_ps(difZ, len3);

        FX = _mm256_add_ps(FX, forceX);
        FY = _mm256_add_ps(FY, forceY);
        FZ = _mm256_add_ps(FZ, forceZ);
    }

    FX = _mm256_hadd_ps(FX, FX);
    FX = _mm256_hadd_ps(FX, FX);
    float tmp[8];
    _mm256_maskstore_ps(tmp, allO, FX);
    p[i].force.x = tmp[0] + tmp[4];
    
    FY = _mm256_hadd_ps(FY, FY);
    FY = _mm256_hadd_ps(FY, FY);
    _mm256_maskstore_ps(tmp, allO, FY);
    p[i].force.y = tmp[0] + tmp[4];
    
    FZ = _mm256_hadd_ps(FZ, FZ);
    FZ = _mm256_hadd_ps(FZ, FZ);
    _mm256_maskstore_ps(tmp, allO, FZ);
    p[i].force.z = tmp[0] + tmp[4];
    p[i].force *= mass2;
}
