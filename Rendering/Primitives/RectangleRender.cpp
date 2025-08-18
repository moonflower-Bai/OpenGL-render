//
// Created by Jingren Bai on 25-7-26.
//

#include "RectangleRender.h"

template<int N>
RectangleRender<N>::RectangleRender(Eigen::Matrix<float, N, 3, Eigen::RowMajor> vertices,
									Eigen::Matrix<float, 2, 3, Eigen::RowMajor> indicates) :
									vertices(vertices), indicates(indicates){
	init();
}

template<int N>
void RectangleRender<N>::render() {

}

template<int N>
Eigen::Matrix<float, N, 3, Eigen::RowMajor> RectangleRender<N>::getVertices() {
	return Eigen::Matrix<float, N, 3, Eigen::RowMajor>();
}

template<int N>
Eigen::Matrix<float, 2, 3, Eigen::RowMajor> RectangleRender<N>::getIndicates() {
	return Eigen::Matrix<float, 2, 3, Eigen::RowMajor>();
}

template<int N>
void RectangleRender<N>::init() {

}
