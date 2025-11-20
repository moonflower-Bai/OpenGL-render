//#version 450 core
////const uint NUM_PARTICLES = 10000u;
//
//uint hash(uint x) {
//    x = ((x >> 16) ^ x) * 0x45d9f3bu;
//    x = ((x >> 16) ^ x) * 0x45d9f3bu;
//    x = (x >> 16) ^ x;
//    return x;
//}
//
//float rand(uint seed) {
//    return float(hash(seed)) / float(0xffffffffu);
//}
//
//// 和 CPU 保持一致的常量（可以从 C++ 里用 uniform 设置）

//
//// 粒子结构（在 C++ 里要按 std430 对齐规则来写内存）
//struct Particle {
//    vec4 pos;      // xyz: 位置, w 可以不用
//    vec4 vel;      // xyz: 速度
//    vec4 oldPos;   // xyz: 上一帧位置
//    float lambda;
//    float density;
//    float _padA;
//    float _padB;
//};
//
//// 粒子数组 SSBO
//layout(std430, binding = 1) buffer Particles {
//    Particle particles[];
//};
//
//// 每个 cell 装的是“粒子索引”，而不是指针
//// cellParticleIndices 的长度 = gridSize.x * gridSize.y * gridSize.z * maxNeighboursPerCell
//layout(std430, binding = 2) buffer CellParticleIndices {
//    uint cellParticleIndices[];
//};
//
//// 每个 cell 当前装了多少个粒子
//layout(std430, binding = 3) buffer CellCounts {
//    uint cellCounts[];
//};
//
//// 工具函数：世界坐标 -> cell 坐标
//ivec3 getCell(vec3 pos) {
//    return ivec3(
//    int(floor(pos.x / cellSize)),
//    int(floor(pos.y / cellSize)),
//    int(floor(pos.z / cellSize))
//    );
//}
//
//bool isInRange(ivec3 c) {
//    return  c.x >= 0 && c.x < gridSize.x &&
//            c.y >= 0 && c.y < gridSize.y &&
//            c.z >= 0 && c.z < gridSize.z;
//}
//
//uint cellToIndex(ivec3 c) {
//    return uint(c.z + gridSize.z * (c.y + gridSize.y * c.x));
//}
//
//// Poly6 核
//float poly6(float r) {
//    if (r <= 0.0 || r >= h) return 0.0;
//    float factor = 315.0 / (64.0 * 3.14159265358979323846);
//    float x = (h * h - r * r) / (h * h * h);
//    return factor * x * x * x;
//}
//
//// Spiky 梯度
//vec3 spikyGradient(vec3 s, float r) {
//    if (r <= 0.0 || r >= h) return vec3(0.0);
//    float factor = -45.0 / 3.14159265358979323846;
//    float x = (h - r) / (h * h * h);
//    float g = factor * x * x / r;
//    return s * g;
//}
//
//// 边界约束（简化版，不用随机 epsilon）
//vec3 confine(vec3 pos) {
//    float eps = rand(uint(pos.x*73856093u) ^ uint(pos.y*19349663u) ^ uint(pos.z*83492791u)); // 生成随机数以防止粘在边界
//    if(eps > 0.09){
//        eps /= 1000.0;
//    }
//    else{
//        eps /= 100.0;
//    }
//    // 定义有效边界（允许在着色器中修正）
//    vec3 localBoundaryMin = boundaryMin;
//    vec3 localBoundaryMax = boundaryMax;
//
//    // 如果原始边界是退化的（点或零体积），使用默认边界
//    if (all(equal(localBoundaryMin, localBoundaryMax))) {
//        localBoundaryMin = vec3(0.0);
//        localBoundaryMax = vec3(32.0, 32.0, 32.0);
//    }
//
//    // 应用边界约束
//    vec3 p = pos;
//    if (p.x < localBoundaryMin.x) p.x = localBoundaryMin.x;
//    if (p.y < localBoundaryMin.y) p.y = localBoundaryMin.y;
//    if (p.z < localBoundaryMin.z) p.z = localBoundaryMin.z;
//    if (p.x > localBoundaryMax.x) p.x = localBoundaryMax.x;
//    if (p.y > localBoundaryMax.y) p.y = localBoundaryMax.y;
//    if (p.z > localBoundaryMax.z) p.z = localBoundaryMax.z;
//    return p;
//}
//

#version 450 core
//const uint NUM_PARTICLES = 10000u;
const float PI = 3.14159265358979323846;
const float POLY6_COEFF   = 315.0 / (64.0 * PI);  // 只是常数部分，h 相关单独算
const float SPIKY_COEFF   = -45.0 / PI;

uint hash(uint x) {
    x = ((x >> 16) ^ x) * 0x45d9f3bu;
    x = ((x >> 16) ^ x) * 0x45d9f3bu;
    x = (x >> 16) ^ x;
    return x;
}

float rand(uint seed) {
    return float(hash(seed)) / float(0xffffffffu);
}

layout(std140, binding = 0) uniform SimParams {
    float dt;
    float h;
    float mass;
    float rho;
    float neighbourRadius;
    float lambdaEpsilon;

    ivec3 gridSize;
    float cellSize;

    vec3 boundaryMin;  // 一般是 (0,0,0)
    float _pad0;
    vec3 boundaryMax;  // 对应你 Simulator::boundary
    float _pad1;

    int maxNeighboursPerCell; // 每个 cell 能装多少粒子
    int numParticles;         // 粒子总数
    int pbfNumIters;          // PBF 迭代次数
    int _pad2;
};

// 粒子结构（在 C++ 里要按 std430 对齐规则来写内存）
struct Particle {
    vec4 pos;      // xyz: 位置, w 可以不用
    vec4 vel;      // xyz: 速度
    vec4 oldPos;   // xyz: 上一帧位置
    float lambda;
    float density;
    float _padA;
    float _padB;
};

// 粒子数组 SSBO
layout(std430, binding = 1) buffer Particles {
    Particle particles[];
};

// 每个 cell 装的是“粒子索引”，而不是指针
// cellParticleIndices 的长度 = gridSize.x * gridSize.y * gridSize.z * maxNeighboursPerCell
layout(std430, binding = 2) buffer CellParticleIndices {
    uint cellParticleIndices[];
};

// 每个 cell 当前装了多少个粒子
layout(std430, binding = 3) buffer CellCounts {
    uint cellCounts[];
};

// 工具函数：世界坐标 -> cell 坐标
ivec3 getCell(vec3 pos) {
    return ivec3(
    int(floor(pos.x / cellSize)),
    int(floor(pos.y / cellSize)),
    int(floor(pos.z / cellSize))
    );
}

bool isInRange(ivec3 c) {
    return  c.x >= 0 && c.x < gridSize.x &&
    c.y >= 0 && c.y < gridSize.y &&
    c.z >= 0 && c.z < gridSize.z;
}

uint cellToIndex(ivec3 c) {
    return uint(c.z + gridSize.z * (c.y + gridSize.y * c.x));
}

// Poly6 核
//float poly6(float r) {
//    if (r <= 0.0 || r >= h) return 0.0;
//    float factor = 315.0 / (64.0 * 3.14159265358979323846);
//    float x = (h * h - r * r) / (h * h * h);
//    return factor * x * x * x;
//}
// Poly6 核：保持公式不变，但：
// 1）π 系数用 POLY6_COEFF，全局 const，所有调用共享
// 2）避免 pow，全部用乘法
float poly6(float r) {
    if (r <= 0.0 || r >= h) return 0.0;

    // 预计算 h^2, h^3，减少重复乘法
    float h2 = h * h;
    float h3 = h2 * h;
    float h9 = h3 * h3 * h3;

    // x = (h^2 - r^2) / h^3
    float r2 = r * r;
    float x = (h2 - r2) / h3;
    float x2 = x * x;

    // 原公式：(315 / (64 π h^9)) * (h^2 - r^2)^3
    // 这里写成 POLY6_COEFF * x^3，其中 x 自带 1/h^3
    return POLY6_COEFF * x2 * x;
}

// Spiky 梯度
//vec3 spikyGradient(vec3 s, float r) {
//    if (r <= 0.0 || r >= h) return vec3(0.0);
//    float factor = -45.0 / 3.14159265358979323846;
//    float x = (h - r) / (h * h * h);
//    float g = factor * x * x / r;
//    return s * g;
//}
// 1）用 inversesqrt(r^2) 替代 sqrt + /r
// 2）预计算 1/h^6，减少 pow / 多次乘法
vec3 spikyGradient(vec3 s, float r) {
    if (r <= 0.0 || r >= h) return vec3(0.0);

    float r2 = r * r;
    // 避免 1/0
    float invR = inversesqrt(max(r2, 1e-12));

    // (h - r)^2
    float hr  = h - r;
    float hr2 = hr * hr;

    // 1 / h^6 = 1 / (h^2 * h^2 * h^2)
    float h2     = h * h;
    float invH6  = 1.0 / (h2 * h2 * h2);

    // 原公式：∇W = -45/(π h^6) * (h - r)^2 * (s / r)
    float scale = SPIKY_COEFF * invH6 * hr2 * invR;

    return s * scale;
}

// 边界约束（简化版，不用随机 epsilon）
//vec3 confine(vec3 pos) {
//    float eps = rand(uint(pos.x*73856093u) ^ uint(pos.y*19349663u) ^ uint(pos.z*83492791u)); // 生成随机数以防止粘在边界
//    if(eps > 0.09){
//        eps /= 1000.0;
//    }
//    else{
//        eps /= 100.0;
//    }
//    // 定义有效边界（允许在着色器中修正）
//    vec3 localBoundaryMin = boundaryMin;
//    vec3 localBoundaryMax = boundaryMax;
//
//    // 如果原始边界是退化的（点或零体积），使用默认边界
//    if (all(equal(localBoundaryMin, localBoundaryMax))) {
//        localBoundaryMin = vec3(0.0);
//        localBoundaryMax = vec3(32.0, 32.0, 32.0);
//    }
//
//    // 应用边界约束
//    vec3 p = pos;
//    if (p.x < localBoundaryMin.x) p.x = localBoundaryMin.x;
//    if (p.y < localBoundaryMin.y) p.y = localBoundaryMin.y;
//    if (p.z < localBoundaryMin.z) p.z = localBoundaryMin.z;
//    if (p.x > localBoundaryMax.x) p.x = localBoundaryMax.x;
//    if (p.y > localBoundaryMax.y) p.y = localBoundaryMax.y;
//    if (p.z > localBoundaryMax.z) p.z = localBoundaryMax.z;
//    return p;
//}
vec3 confine(vec3 pos) {
    vec3 minB = boundaryMin;
    vec3 maxB = boundaryMax;

    // 如果边界退化成点 / 未设置，给个默认 AABB
    if (all(equal(minB, maxB))) {
        minB = vec3(0.0);
        maxB = vec3(32.0, 32.0, 32.0);
    }

    // 在边界内收一点，让粒子不要直接贴在边界面上
    // 可以按 h 调节这个 margin
    float margin = 0.5 * h;
    minB += vec3(margin);
    maxB -= vec3(margin);

    // 直接 clamp
    vec3 p = clamp(pos, minB, maxB);
    return p;
}


