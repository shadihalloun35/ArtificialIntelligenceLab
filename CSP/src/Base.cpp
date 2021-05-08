#include "Base.h"
#include <iostream>
using namespace std;

void Base::printMatrix(Matrix mtx)
{
	for (int i = 0; i < mtx.dimension; i++) {
		for (int j = 0; j < mtx.dimension; j++)
			cout << mtx.adj[i][j];
		cout << endl;
	}
}

void Base::createMatrix(Matrix * mtx, int nodeNum)
{
	mtx->dimension = nodeNum;
	mtx->adj = new int*[nodeNum];
	for (int i = 0; i < nodeNum; ++i)
		mtx->adj[i] = new int[nodeNum];
	for (int i = 0; i < nodeNum; i++)
	{
		for (int j = 0; j < nodeNum; j++)
			mtx->adj[i][j] = 0;
	}
}