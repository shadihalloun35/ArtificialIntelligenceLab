#pragma once

struct Matrix
{
	int dimension;
	int** adj;
	int colors = 1;
};

class Base
{
	public:
		static void printMatrix(Matrix mtx);
		static void createMatrix(Matrix* mtx, int nodeNum);
};
