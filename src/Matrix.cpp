#include "matrix.h"
#include <iostream>


CV::Utils::Matrix::Matrix() {
	row = 0;
	col = 0;
	mat = 0;
}

CV::Utils::Matrix::Matrix(int row, int column) {
	this->row = row;
	this->col = column;

	this->mat = new double* [row];

	for (int i = 0; i < row; i++) {
		mat[i] = new double[col];
		for (int j = 0; j < col; j++) {
			mat[i][j] = 0;
		}
	}
}

double CV::Utils::Matrix::normalize() {
	double maxInt = 0;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (mat[i][j] > maxInt)
				maxInt = mat[i][j];
		}
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			mat[i][j] = mat[i][j] * 255 / maxInt;
		}
	}
	return maxInt;
}
