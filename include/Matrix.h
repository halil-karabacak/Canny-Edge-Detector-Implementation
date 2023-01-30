#pragma once

class Matrix {
public :
	Matrix();
	Matrix(int row, int column, double** data);
	Matrix(int row, int column);

	double normalize();


	int row;
	int col;
	double** mat;
};
