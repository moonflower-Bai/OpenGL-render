#ifndef FLUID_SIMULATOR_H
#define FLUID_SIMULATOR_H

#include "Particle.h"

class Simulator{ // 使用拉格朗日法求粒子在每一帧的更新
public:
    Simulator() = default;
	explicit Simulator(int particleNums);
    ~Simulator() = default;

    void init(int particleNums);
	float poly6Value(float, float h); // 计算核函数
	Eigen::Vector3f spikyGradient(Eigen::Vector3f, float r, float h);

	void runPBF(); // 执行PBF算法
	void prologue(); // 初始化粒子状态(拉格朗日法)
    void update(); // 当更新帧时，调用该函数更新流体状态(拉格朗日法)
	void epilogue();

	const std::vector<Particle>& getParticles(); // 获得粒子对象
//	const Particle* getParticles(); // 获得粒子对象
//	const int getParticleNums();
	const std::vector<Eigen::Vector3f>& getParticlePos();

	Eigen::Vector3f getBoundingBox();

private:
	// ----------- 参数设置 ----------
//    int dx = 64, dy = 64, dz = 64; // 采样范围，默认64*64*64
	int screen_x = 800, screen_y = 600;
	float screenToWorldRatio = 20.0; // 屏幕坐标与世界坐标的比率，默认10.0
	Eigen::Vector3f boundary = Eigen::Vector3f(screen_x / screenToWorldRatio, screen_y, screen_x / screenToWorldRatio); // 边界大小，防止超出边界，默认64*64*64
//	Eigen::Vector3f boundary = Eigen::Vector3f(30, 50, 30);
	float epsilon = 1e-5; // 浮点数误差，默认1e-5
	float neighbourRadius = 1.01f; // 邻居搜索半径，默认1.05，单位：世界坐标单位

	// ----------- 网格坐标计算 ----------
	float cellSize = 2.51; // 单位：世界坐标单位
	float cellRecpr = 1.0 / cellSize; // 单位：世界坐标单位^-1
	Eigen::Vector3i gridSize = Eigen::Vector3i(roundUp(boundary.x(), 1), roundUp(boundary.y(), 1), roundUp(boundary.z(), 1)); // 网格大小，默认64*64*64
//	std::vector<std::vector<Particle*>> gridToParticles((gridSize.x() * gridSize.y() * gridSize.z()));
	std::vector<Particle*> gridToParticles[2000000];
	/*
	 * @brief: 确保网格数量能够覆盖整个场景
	 * @param arg1: 需要向上取整的数
	 * @param arg2: 需要向上取整的倍数
	 * @return: 向上取整后的结果（整数加一，防止丢失信息，** 不用ceil的原因 **）
	 * 对于 dx=64, dy=64, dz=64, boundary=64*64*64, cellSize=2.51, cellRecpr=1.0/2.51
	 * 64/2.51=26.4，向上取整为27，所以最终网格大小为27*27*27
	 */
	int roundUp(float, float);

	// ----------- 网格参数 ----------
	int maxNeighbour = 40;

	// ----------- 粒子参数 ----------
    float dt = 0.05f; // 更新时间间隔
	float particleRadius = 1.1f, particleRadiusInWorld = particleRadius / screenToWorldRatio; // 粒子半径，默认3.0，单位：世界坐标单位
    std::vector<Particle> particles; // 粒子对象
//	Particle particles[2000000];
//	int particleNums;
	std::vector<Eigen::Vector3f> particlePos;
    Eigen::Vector3f gravity;

	// ----------- 粒子更新 ----------
	Eigen::Vector3f confineParticle(Eigen::Vector3f pos); // 限制粒子在边界内，对超出边界的粒子的位置进行修正
	Eigen::Vector3i getCell(Eigen::Vector3f pos);

	// ----------- PBF参数 ----------
	int pbfNumIters = 5; // PBF迭代次数，默认5次
	float h = 1.1; // 粒子核函数的半径，确定粒子相互作用的范围，默认1.1，单位：世界坐标单位
	float mass = 1.0, rho = 1.0; // 粒子质量，默认1.0，单位：世界坐标单位, 粒子静止密度，默认1.0(水)，单位：世界坐标单位
	float lambdaEpsilon = 100.0; // 求解拉格朗日乘子的参数，防止求解零矩阵，默认100.0
	/* 以下是XSPH对PBF的修正
	 * XSPH基本思想：
	 *  让粒子倾向于向周围粒子的平均位置移动
	 *  通过添加一个与邻居粒子位置差相关的修正项，抑制非物理运动。
	 *
	 */
	float corrDeltaQCooff = 0.3f, corrK = 0.001; // XSPH参数，默认0.3, 0.001

	bool isInRange(const Eigen::Vector3i &cell);

	unsigned int computeMortonCode(Eigen::Vector3f pos);
};
#endif