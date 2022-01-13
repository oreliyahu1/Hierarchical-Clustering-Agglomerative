#ifndef	Matrix_H
#define Matrix_H

#include <iostream>
#include <vector>
using namespace std;


class Matrix
{
public:

	~Matrix() { };

	virtual double& operator () (size_t i, size_t j) = 0;

	virtual void print() = 0;

	friend ostream& operator << (ostream& out, Matrix& M) {}

	virtual void setCopyCol(size_t columnToChange, size_t columnToCopy) = 0;

	virtual void setCopyRow(size_t rowToChange, size_t rowToCopy) = 0;

	virtual void removeLastRow(bool mem_free = false) = 0;

	virtual void removeLastCol(bool mem_free = false) = 0;

	virtual size_t getRowSize() = 0;

	virtual size_t getColSize() = 0;

	virtual vector<size_t> getSize() = 0;
};


#endif
