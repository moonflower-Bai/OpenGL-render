#include "FluidSimulator.h"

#include <cmath>
#include <random>
#include <unordered_map>
#include "utils/log.cpp"

#define PI 3.14159265358979323846

Simulator::Simulator(int particleNums) {
	LOG_INFO << "FluidSimulator init";
	init(particleNums);
}

// ----------- 运算函数 -----------
namespace {
	struct glm_ivec3_hash {
		std::size_t operator()(const Eigen::Vector3i& v) const {
			return std::hash<int>()(v.x()) ^
				   std::hash<int>()(v.y()) ^
				   std::hash<int>()(v.z());
		}
	};
}


Eigen::Vector3i Simulator::getCell(Eigen::Vector3f pos) {
	return Eigen::Vector3i{
			static_cast<int>(pos.x() * cellRecpr),
			static_cast<int>(pos.y() * cellRecpr),
			static_cast<int>(pos.z() * cellRecpr)
	};
}
int Simulator::roundUp(float val, float step) {
	return static_cast<int>(std::floor(val * cellRecpr / step + 1) * step);
}
const std::vector<Particle>& Simulator::getParticles() {
	return particles;
}
//const Particle* Simulator::getParticles() {
//	return particles;
//}
//const int Simulator::getParticleNums() {
//	return particleNums;
//}
Eigen::Vector3f Simulator::confineParticle(Eigen::Vector3f pos){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, epsilon); // 创建一个随机数生成器, 范围在0到1e-5之间
	Eigen::Vector3f minBound = Eigen::Vector3f(0, 0, 0);
	Eigen::Vector3f maxBound = Eigen::Vector3f(boundary.x(), boundary.y(), boundary.z());
	if(pos.x() < minBound.x()) pos.x() = minBound.x() + dis(gen);
	if(pos.y() < minBound.y()) pos.y() = minBound.y() + dis(gen);
	if(pos.z() < minBound.z()) pos.z() = minBound.z() + dis(gen);
	if(pos.x() > maxBound.x()) pos.x() = maxBound.x() - dis(gen);
	if(pos.y() > maxBound.y()) pos.y() = maxBound.y() - dis(gen);
	if(pos.z() > maxBound.z()) pos.z() = maxBound.z() - dis(gen);
	return pos;
}
bool Simulator::isInRange(const Eigen::Vector3i &cell) {
	return cell.x() >= 0 && cell.x() < gridSize.x() &&
		   cell.y() >= 0 && cell.y() < gridSize.y() &&
		   cell.z() >= 0 && cell.z() < gridSize.z();
}
float Simulator::poly6Value(float r, float h) { // 计算核函数
	if(0 >= r || r >= h) return 0.0f;
	const float ploy6Factor = 315.0f / 64.0f / PI;
	const float x = (h * h - r * r) / h * h * h;
	return ploy6Factor * x * x * x;
}

// ----------- PBF -----------
void Simulator::init(int particleNums) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-0.25f, 0.25f); // 创建一个随机数生成器, 范围在-0.25到0.25之间
	LOG_INFO << "FluidSimulator init";
	Eigen::Vector3f initPos = Eigen::Vector3f(10.0f, 2.0f, 10.0f);
	LOG_INFO << "boundary = " << boundary << ", grid = " << gridSize << ", cellSize = " << cellSize << ". ";
	float CubeSize = std::ceil(pow(particleNums, 1.0f / 3.0f));
	float spacing = 1.0f;
	int numPerRow = (int)(CubeSize / spacing) + 1;
	int numPerFloor = numPerRow * numPerRow;
	for (int i = 0; i < particleNums; i++) {
		int floor = i / numPerFloor;
		int row = (i % numPerFloor) / numPerRow;
		int col = (i % numPerFloor) % numPerRow;
		Eigen::Vector3f pos = Eigen::Vector3f(static_cast<float>(col) * spacing + dis(gen),
								  static_cast<float>(floor) * spacing + dis(gen),
								  static_cast<float>(row) * spacing + dis(gen)) + initPos;
		particles.emplace_back(pos);
//		particles[++(Simulator::particleNums)] = Particle(pos);
//		LOG_INFO << "pointer: " << &(particles.back());
	}
	LOG_INFO << "row: " << numPerRow << ", col: " << numPerRow << ", floor: " << numPerFloor;
	LOG_INFO << "Init particle nums is " << particleNums << ". " << "Finished init.";
}
void Simulator::runPBF() {
	prologue();
	for(int i = 1; i <= pbfNumIters; i++){
		update();
	}
	epilogue();
}
#pragma omp parallel for
void Simulator::prologue() {
	int gridNum = gridSize.x() * gridSize.y() * gridSize.z();
	for(int i = 0; i <= gridNum; i++){
		gridToParticles[i].clear();
	}
	for (auto& p : particles) {
		p.neighbour.clear(); // 清空邻居粒子
		p.oldPos = p.pos; // 备份粒子坐标
	}
	for (auto& p : particles){
		Eigen::Vector3f g = Eigen::Vector3f(0.0f, -9.8f, 0.0f); // 重力加速度
		Eigen::Vector3f pos_i = p.pos;
		Eigen::Vector3f vel_i = p.vel;
		vel_i += g * dt; // 更新速度
		pos_i += vel_i * dt; // 更新位置
		p.pos = confineParticle(pos_i); // 检查粒子是否在边界内
	}
	// 初始化网格变量
//	std::unordered_map<Eigen::Vector3i, std::vector<Particle*>, glm_ivec3_hash> gridToParticles;
//	int* gridToParticles = new int[gridSize.x() * gridSize.y() * gridSize.z()];
	std::function vec2grid = [](Eigen::Vector3i pos, Eigen::Vector3i grid){
		return int(grid.y() * grid.z() * pos.x() + grid.z() * pos.y() + pos.z());
	};
	for(auto& p:particles){
		Eigen::Vector3i grid = getCell(p.pos); // 计算粒子所在的网格
		gridToParticles[vec2grid(grid, gridSize)].push_back(&p); // 将粒子添加到网格中
	}
	for(int i = 0; i < gridSize.x() * gridSize.y() * gridSize.z(); i++) {
		auto& cellParticles = gridToParticles[i];
		// 计算每个粒子的Morton码
		std::sort(cellParticles.begin(), cellParticles.end(),
				  [this](Particle* a, Particle* b) {
					  return computeMortonCode(a->pos) < computeMortonCode(b->pos);
				  });
	}
	/*
	 * 遍历所有粒子，计算粒子所在的网格，并将粒子添加到网格中
	 * 通过检查粒子所在网格与临近网格，可以以较高效率获取邻居粒子
	 *
	 * 优化：遍历所有网格，而不是遍历所有粒子，对于同一网格内的粒子进行并行处理，因为同一网格内的粒子互不影响
	 */
	for(auto& p:particles){
		Eigen::Vector3i grid = getCell(p.pos); // 计算粒子所在的网格
		Eigen::Vector3f pos_i = p.pos;

		float sqNeighbour = neighbourRadius * neighbourRadius;
		std::vector<int> operatorNumber = {0, 1, -1};
		for(int i:operatorNumber){
			for(int j:operatorNumber) {
				for (int k:operatorNumber) {
					Eigen::Vector3i grid_i = grid + Eigen::Vector3i{i, j, k}; // 遍历周边网格
					if(!isInRange(grid_i)) continue;
					for(auto J:gridToParticles[vec2grid(grid_i, gridSize)]){
						if(J == &p) continue;
						Eigen::Vector3f pos_j = J->pos;
						float r_sq = (pos_i - pos_j).squaredNorm();
						if(r_sq < sqNeighbour && pos_i != pos_j){ // 粒子在半径范围内，数量小于最大邻居数量，且距离小于邻居半径，则添加到邻居列表中
							p.neighbour.push_back(J);
						}
						if(p.neighbour.size() >= maxNeighbour){
							break;
						}
					}
				}
				if(p.neighbour.size() >= maxNeighbour){
					break;
				}
			}
			if(p.neighbour.size() >= maxNeighbour){
				break;
			}
		}
	}
}
#pragma omp parallel for
void Simulator::update() {    // 在这里不写 while-loop，因为渲染不在这里
	float ref_poly6 = poly6Value(0.33f, h);
	float inf_ref_poly6 = 1.0f / ref_poly6;
	/*
	* 借鉴SPH和PBF论文，考虑单个粒子的受力情况
	* 1.粒子间的作用力
	* (1) 通过粒子的密度关系，求解粒子的约束坐标
	* (2) 按照PBH算法的顺序依次计算参数并最终更新
	* 2.粒子遇到外界边界和固定物体的作用力
	* 3.不可压缩的粒子的实现
	* 4.粒子粘滞系数带来的受力
	*/
	// 计算拉格朗日乘数
	for(auto& p: particles){
		Eigen::Vector3f pos_i = p.pos;
		Eigen::Vector3f grad_i = Eigen::Vector3f::Zero();
		float densityConstraint = 0.0f; // 密度约束
		float sumSqrGrad = 0.0f; // 梯度平方和
		int size = std::min(static_cast<int>(p.neighbour.size()), maxNeighbour);
		for(int j = 0; j < size; j++){
			auto q = p.neighbour[j];
			if(q == &p) continue; // 自身不计算
			Eigen::Vector3f s = pos_i - q->pos; // 计算粒子间的距离
			float r = s.norm();
			float poly6 = poly6Value(r, h); // 计算核函数
			Eigen::Vector3f grad = spikyGradient(s, r, h);
			grad_i += grad; // 累加梯度
			densityConstraint += poly6; // 累加密度约束
			sumSqrGrad += grad.dot(grad); // 累加梯度平方和
		}
		p.density = ((mass * densityConstraint / rho) - 1.0f); // 更新粒子密度
		sumSqrGrad += grad_i.dot(grad_i);
		p.lambda = (-1.0f * p.density) / (sumSqrGrad + lambdaEpsilon); // 计算拉格朗日乘子
	}
	// 计算粒子位置增量
	for(auto& p: particles){
		Eigen::Vector3f posDelta = Eigen::Vector3f::Zero();
		Eigen::Vector3f pos_i = p.pos;
		float lambda_i = p.lambda;
		Eigen::Vector3f grad_i = Eigen::Vector3f::Zero();
		int size = std::min(static_cast<int>(p.neighbour.size()), maxNeighbour);
		for(int j = 0; j < size; j++){
			auto q = p.neighbour[j];
			if(q == &p) continue; // 自身不计算
			float lambda_j = q->lambda;
			Eigen::Vector3f s = pos_i - q->pos;
			float r = s.norm();
			// 粒子压力矫正因子
			float poly6 = poly6Value(r, h);
			float scorr_ij = poly6 * inf_ref_poly6;
			scorr_ij = scorr_ij * scorr_ij;
			scorr_ij = scorr_ij * scorr_ij * (-0.001f);
			posDelta += (lambda_i + lambda_j + scorr_ij) * spikyGradient(s, r, h); // 计算位置增量
			// ** 在更新位置的时候，邻居粒子位置会变化，所以预存的梯度是错误的 **
		}
		posDelta /= rho; // 平均位置增量, 除以水的密度
		p.pos += posDelta; // 更新粒子位置
	}
}
void Simulator::epilogue() {
	for(auto& p: particles){
		auto pos = p.pos;
		p.pos = confineParticle(pos);
	}
	for(auto& p: particles){
		p.vel = (p.pos - p.oldPos) / dt;
	}
}

const std::vector<Eigen::Vector3f> &Simulator::getParticlePos() {
	particlePos.clear();
	for(auto& p: particles){
		particlePos.push_back(p.pos);
	}
	return particlePos;
}

Eigen::Vector3f Simulator::spikyGradient(Eigen::Vector3f s, float r, float h){
	if(0 >= r || r >= h) return Eigen::Vector3f::Zero();
	const float spikyGradientFactor = -45.0f / PI;
	const float x = (h - r) / (h * h * h);
	const float g_factor = spikyGradientFactor * x * x;
	return s * g_factor / r;
}

Eigen::Vector3f Simulator::getBoundingBox() {
	return boundary;
}

unsigned int Simulator::computeMortonCode(Eigen::Vector3f pos) {
	// 将世界坐标转换为网格内的相对坐标
	Eigen::Vector3i gridPos = getCell(pos);

	// 交错位编码（简化示例）
	unsigned int result = 0;
	for (int i = 0; i < 10; i++) {  // 假设网格大小在1024x1024x1024内
		result |=
				((gridPos.x() >> i) & 1) << (i*3) |
				((gridPos.y() >> i) & 1) << (i*3+1) |
				((gridPos.z() >> i) & 1) << (i*3+2);
	}
	return result;
}
