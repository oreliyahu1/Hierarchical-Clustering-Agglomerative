#ifndef	MemMatrix_H
#define MemMatrix_H

#include "Matrix.h"
using namespace std;


class MemMatrix : public Matrix
{
	private:
		double** m_pMat;
		size_t m_cRowMemSize;
		size_t m_cColMemSize;
		size_t m_cRow;
		size_t m_cCol;

	public:

		// constructor with no params
		MemMatrix();

		// constructor with size params
		MemMatrix(size_t m, size_t n);

		// copy constructor
		MemMatrix(MemMatrix& M);

		// move constructor
		MemMatrix(MemMatrix&& M);

		//destructor
		~MemMatrix();

		void operator = (MemMatrix& M);

		void operator = (MemMatrix&& M);

		double& operator () (size_t i, size_t j);

		double* operator [] (size_t i);

		void print();

		friend ostream& operator << (ostream& out, MemMatrix& M);

		void setCopyCol(size_t columnToChange, size_t columnToCopy);

		void setCopyRow(size_t rowToChange, size_t rowToCopy);

		void removeLastRow(bool mem_free = false);

		void removeLastCol(bool mem_free = false);

		size_t getRowSize();

		size_t getColSize();

		vector<size_t> getSize();
};


#endif
