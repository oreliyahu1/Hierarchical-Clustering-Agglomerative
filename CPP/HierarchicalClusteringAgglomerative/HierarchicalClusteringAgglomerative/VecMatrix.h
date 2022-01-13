#ifndef	VecMatrix_H
#define VecMatrix_H

#include "Matrix.h"

using namespace std;

typedef vector<double> Vec;
typedef vector<Vec> Mat;

class VecMatrix : public Matrix
{
	private:
		Mat m_Mat;

	public:
		// constructor with no params
		VecMatrix();

		// constructor with size params
		VecMatrix(size_t m, size_t n);

		// copy constructor
		VecMatrix(VecMatrix& M);

		// move constructor
		VecMatrix(VecMatrix&& M);

		void operator = (VecMatrix& M);

		void operator = (VecMatrix&& M);

		double& operator () (size_t i, size_t j);

		void setCopyCol(size_t columnToChange, size_t columnToCopy);

		void setCopyRow(size_t rowToChange, size_t rowToCopy);

		void setCol(size_t i, Vec& vec);

		Vec getRow(size_t i);

		void setRow(size_t i, Vec& vec);

		Vec getCol(size_t i);

		void addRow(Vec row);

		void removeLastRow(bool mem_free = false);

		void removeLastCol(bool mem_free = false);
	
		void print();

		friend ostream& operator << (ostream& out, VecMatrix& M);

		size_t getRowSize();

		size_t getColSize();

		vector<size_t> getSize();
};


#endif
