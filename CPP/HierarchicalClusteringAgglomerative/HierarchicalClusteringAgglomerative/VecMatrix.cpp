#include "VecMatrix.h"

// constructor with no params
VecMatrix::VecMatrix()
{
	m_Mat.resize(0, Vec(0, 0));
}

// constructor with no params
VecMatrix::VecMatrix(size_t m, size_t n)
{
	m_Mat.resize(m, Vec(n, 0));
}

// move constructor
VecMatrix::VecMatrix(VecMatrix&& M)
{
	m_Mat = M.m_Mat;
}

// copy constructor
VecMatrix::VecMatrix(VecMatrix& M)
{
	m_Mat = M.m_Mat;
}

void VecMatrix::operator = (VecMatrix& M)
{
	m_Mat = M.m_Mat;
}

void VecMatrix::operator = (VecMatrix&& M)
{
	m_Mat = M.m_Mat;
}

double& VecMatrix::operator () (size_t i, size_t j)
{
	return m_Mat[i][j];
}

void VecMatrix::setCopyCol(size_t columnToChange, size_t columnToCopy)
{
	if (m_Mat.size() == 0) {
		return;
	}

	if (m_Mat[0].size() <= columnToChange || m_Mat[0].size() <= columnToCopy) {
		return;
	}

	for (size_t w = 0; w < m_Mat.size(); w++) {
		m_Mat[w][columnToChange] = m_Mat[w][columnToCopy];
	}
}

void VecMatrix::setCopyRow(size_t rowToChange, size_t rowToCopy)
{
	if (m_Mat.size() <= rowToChange || m_Mat.size() <= rowToCopy) {
		return;
	}

	m_Mat[rowToChange] = m_Mat[rowToCopy];
}

void VecMatrix::setRow(size_t i, Vec& vec)
{
	if (m_Mat.size() <= i) {
		return;
	}

	if (m_Mat[0].size() != vec.size()) {
		return;
	}

	m_Mat[i] = vec;
}

Vec VecMatrix::getRow(size_t i)
{
	if (m_Mat.size() <= i) {
		return Vec(0);
	}

	return m_Mat[i];
}

void VecMatrix::setCol(size_t i, Vec& vec)
{
	if (m_Mat.size() != vec.size()) {
		return;
	}

	if (m_Mat[0].size() <= i) {
		return;
	}

	for (size_t j = 0; j < m_Mat.size(); j++)
		m_Mat[j][i] = vec[j];
}

Vec VecMatrix::getCol(size_t i)
{
	Vec ret;
	if (m_Mat.size() == 0 || m_Mat[0].size() <= i) {
		return ret;
	}

	for (size_t j = 0; j < m_Mat.size(); j++)
		ret.push_back(m_Mat[j][i]);
	return ret;
}

void VecMatrix::addRow(Vec row)
{
	if (m_Mat.size() == 0) {
		m_Mat.push_back(row);
		return;
	}

	if (m_Mat[0].size() != row.size()) {
		return;
	}

	m_Mat.push_back(row);
}

void VecMatrix::removeLastRow(bool mem_free)
{
	if (m_Mat.size() == 0) {
		return;
	}

	m_Mat.pop_back();
}

void VecMatrix::removeLastCol(bool mem_free)
{
	if (m_Mat.size() == 0 || m_Mat[0].size() == 0) {
		return;
	}

	for (size_t j = 0; j < m_Mat.size(); j++)
		m_Mat[j].pop_back();
}


void VecMatrix::print()
{
	for (size_t i = 0; i < m_Mat.size(); i++) {
		for (size_t j = 0; j < m_Mat[i].size(); j++)
			cout << m_Mat[i][j] << "\t";
		cout << endl;
	}
}

ostream & operator<<(ostream & out, VecMatrix& M)
{
	for (size_t i = 0; i < M.m_Mat.size(); i++) {
		for (size_t j = 0; j < M.m_Mat[i].size(); j++)
			out << M.m_Mat[i][j] << "\t";
		out << endl;
	}
	return out;
}

vector<size_t> VecMatrix::getSize()
{
	vector<size_t> mat_size(2);
	mat_size[0] = m_Mat.size();
	mat_size[1] = (m_Mat.size()) ? m_Mat[0].size() : 0;
	return mat_size;
}

size_t VecMatrix::getRowSize()
{
	return m_Mat.size();
}

size_t VecMatrix::getColSize()
{
	return (m_Mat.size()) ? m_Mat[0].size() : 0;
}