//
// Created by Jingren Bai on 25-7-26.
//

#ifndef LEARNOPENGL_RECTANGLERENDER_H
#define LEARNOPENGL_RECTANGLERENDER_H

#include "cmake-build-debug-visual-studio/vcpkg_installed/x64-windows/include/Eigen/Eigen"

template<int N>
class RectangleRender {
private:
	Eigen::Matrix<float, N, 3, Eigen::RowMajor> vertices;
	Eigen::Matrix<float, 2, 3, Eigen::RowMajor> indicates;
public:
	RectangleRender(Eigen::Matrix<float, N, 3, Eigen::RowMajor> vertices,
					Eigen::Matrix<float, 2, 3, Eigen::RowMajor> indicates);
	void render();
	Eigen::Matrix<float, N, 3, Eigen::RowMajor> getVertices();
	Eigen::Matrix<float, 2, 3, Eigen::RowMajor> getIndicates();
	void init();
};




#endif //LEARNOPENGL_RECTANGLERENDER_H
