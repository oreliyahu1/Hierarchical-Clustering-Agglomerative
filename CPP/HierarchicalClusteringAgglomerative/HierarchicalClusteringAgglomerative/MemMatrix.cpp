#include "MemMatrix.h"


// constructor with no params
MemMatrix::MemMatrix() : m_pMat(nullptr), m_cRow(0), m_cCol(0), m_cRowMemSize(0), m_cColMemSize(0) {}

// constructor with size params
MemMatrix::MemMatrix(size_t m, size_t n) : m_pMat((double**)malloc(sizeof(double*) * m)), m_cRow(m), m_cCol(n), m_cRowMemSize(m), m_cColMemSize(n) {
	if (m_pMat == nullptr) {
		throw bad_alloc();
	}

	for (size_t i = 0; i < m_cRow; i++)
	{
		m_pMat[i] = (double*)malloc(sizeof(double) * n);
		if (m_pMat[i] == nullptr) {
			throw bad_alloc();
		}
		memset(m_pMat[i], 0, m_cColMemSize * sizeof(double));
	}
}

// copy constructor
MemMatrix::MemMatrix(MemMatrix& M) : m_cRow(M.m_cRow), m_cCol(M.m_cCol), m_cRowMemSize(M.m_cRowMemSize), m_cColMemSize(M.m_cColMemSize)
{
	m_pMat = (double**)malloc(sizeof(double*) * m_cRow);
	if (m_pMat == nullptr) {
		throw bad_alloc();
	}

	for (size_t i = 0; i < m_cRow; i++)
	{
		m_pMat[i] = (double*)malloc(sizeof(double) * m_cCol);
		if (m_pMat[i] == nullptr) {
			throw bad_alloc();
		}
	}

	for (size_t i = 0; i < m_cRow; i++) {
		for (size_t j = 0; j < m_cCol; j++) {
			m_pMat[i][j] = M.m_pMat[i][j];
		}
	}
}

// move constructor
MemMatrix::MemMatrix(MemMatrix&& M) : m_pMat(M.m_pMat), m_cRow(M.m_cRow), m_cCol(M.m_cCol), m_cRowMemSize(M.m_cRowMemSize), m_cColMemSize(M.m_cColMemSize)
{
	M.m_pMat = nullptr;
}

//destructor
MemMatrix::~MemMatrix()
{
	if (m_pMat) {
		for (size_t i = 0; i < m_cRowMemSize; i++) {
			free(m_pMat[i]);
		}
		free(m_pMat);
		m_pMat = nullptr;
	}
}


void MemMatrix::operator = (MemMatrix& M)
{
	m_cRow = M.m_cRow;
	m_cCol = M.m_cCol;
	m_cRowMemSize = M.m_cRowMemSize;
	m_cColMemSize = M.m_cColMemSize;

	if (m_pMat) {
		for (size_t i = 0; i < m_cRowMemSize; i++) {
			free(m_pMat[i]);
		}
		free(m_pMat);
		m_pMat = nullptr;
	}

	m_pMat = M.m_pMat;
}

void MemMatrix::operator = (MemMatrix&& M)
{
	m_cRow = M.m_cRow;
	m_cCol = M.m_cCol;
	m_cRowMemSize = M.m_cRowMemSize;
	m_cColMemSize = M.m_cColMemSize;
	m_pMat = M.m_pMat;
	M.m_pMat = nullptr;
}

double& MemMatrix::operator () (size_t i, size_t j)
{
	if (m_cRow <= i || m_cCol <= j) {
		throw exception("Oh");
	}

	return m_pMat[i][j];
}

double* MemMatrix::operator [] (size_t i)
{
	return m_pMat[i];
}

void MemMatrix::print()
{
	for (size_t i = 0; i < m_cRow; i++) {
		for (size_t j = 0; j < m_cCol; j++)
			cout << m_pMat[i][j] << "\t";
		cout << endl;
	}
}

ostream& operator<<(ostream& out, MemMatrix& M)
{
	for (size_t i = 0; i < M.m_cRow; i++) {
		for (size_t j = 0; j < M.m_cCol; j++)
			out << M.m_pMat[i][j] << "\t";
		out << endl;
	}
	return out;
}


void MemMatrix::setCopyCol(size_t columnToChange, size_t columnToCopy)
{
	for (size_t i = 0; i < m_cRow; i++) {
		m_pMat[i][columnToChange] = m_pMat[i][columnToCopy];
	}
}

void MemMatrix::setCopyRow(size_t rowToChange, size_t rowToCopy)
{
	memcpy_s(m_pMat[rowToChange], sizeof(double) * m_cCol, m_pMat[rowToCopy], sizeof(double) * m_cCol);
}

void MemMatrix::removeLastRow(bool mem_free)
{
	if (m_cRow == 0) {
		return;
	}

	m_cRow--;
	if (mem_free)
	{
		for (size_t i = m_cRow; i < m_cRowMemSize; i++) {
			free(m_pMat[i]);
		}

		m_cRowMemSize = m_cRow;
		if (m_cRowMemSize)
		{
			m_pMat = (double**)realloc(m_pMat, sizeof(double*) * m_cRowMemSize);
			if (m_pMat == nullptr) {
				throw bad_alloc();
			}
		}
		else
		{
			m_pMat == nullptr;
		}

	}
}

void MemMatrix::removeLastCol(bool mem_free)
{
	if (m_cCol == 0) {
		return;
	}

	m_cCol--;
	if (mem_free)
	{
		for (size_t i = 0; i < m_cRowMemSize; i++) {
			m_pMat[i] = (double*)realloc(m_pMat[i], sizeof(double) * m_cCol);
			if (m_pMat[i] == nullptr) {
				throw bad_alloc();
			}
		}

		m_cColMemSize = m_cCol;
	}
}

size_t MemMatrix::getRowSize()
{
	return m_cRow;
}

size_t MemMatrix::getColSize()
{
	return m_cCol;
}

vector<size_t> MemMatrix::getSize()
{
	return { m_cRow, m_cCol };
}