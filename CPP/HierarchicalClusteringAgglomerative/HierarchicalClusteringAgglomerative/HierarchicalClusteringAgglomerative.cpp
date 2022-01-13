// HierarchicalClusteringAgglomerative.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "VecMatrix.h"
#include "MemMatrix.h"
#include "hierarchical_clustering.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include <chrono> 

#include "Dependencies\alglib3_16_0\dataanalysis.h"
#include "Dependencies\alglib3_16_0\stdafx.h"
#include <stdlib.h>


using namespace alglib;
using namespace std::chrono;


double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

VecMatrix copyMatrix(MemMatrix& mem_dist_mtx)
{
    vector<size_t> mem_dist_mtx_size = mem_dist_mtx.getSize();
    VecMatrix dist_mtx(mem_dist_mtx_size[0], mem_dist_mtx_size[1]);

    for (size_t i = 0; i < mem_dist_mtx_size[0]; i++) {
        for (size_t j = 0; j < mem_dist_mtx_size[1]; j++) {
            dist_mtx(i, j) = mem_dist_mtx(i, j);
        }
    }

    return dist_mtx;
}

MemMatrix copyMatrix(VecMatrix& dist_mtx, bool remove_dist_mtx = false)
{
    vector<size_t> dist_mat_size = dist_mtx.getSize();
    MemMatrix mem_dist_mtx(dist_mat_size[0], dist_mat_size[1]);

    for (size_t i = 0; i < dist_mat_size[0]; i++) {
        for (size_t j = 0; j < dist_mat_size[1]; j++) {
            mem_dist_mtx(dist_mat_size[0] - i - 1, j) = dist_mtx(dist_mat_size[0] - i - 1, j);
        }

        if (remove_dist_mtx) {
            dist_mtx.removeLastRow(true);
        }
    }

    return mem_dist_mtx;
}

VecMatrix createRandomDistanceMatrix(size_t msize)
{
    VecMatrix dist_mtx(msize, msize);

    vector<size_t> dist_mat_size = dist_mtx.getSize();
    for (size_t i = 0; i < dist_mat_size[0]; i++) {
        for (size_t j = 0; j < i; j++) {
            double val = fRand(1, 30);
            dist_mtx(i, j) = val;
            dist_mtx(j, i) = val;
        }
    }

    return dist_mtx;
}

VecMatrix getDistanceMatrix(string file_name)
{
    VecMatrix re;
    ifstream is(file_name);
    string str_line;

    // Read the next line from File untill it reaches the end.
    while (getline(is, str_line)) {
        if (str_line.size() <= 0)
            continue;

        istringstream sstr_vec(str_line);
        string str_vec;
        Vec dVec;
        while (std::getline(sstr_vec, str_vec, ',')) {
            dVec.push_back(stod(str_vec));
        }

        re.addRow(dVec);
    }

    //Close The File
    is.close();

    return re;
}

void test_matrix(Matrix& dist_mtx, size_t n_clusters = 2, bool code_optz = false, bool print_result = false)
{
    auto start = high_resolution_clock::now();
    vector<size_t> res = my_hierarchical_clustering_agglomerative(dist_mtx, n_clusters, code_optz);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<chrono::milliseconds>(stop - start) / 1000.0;

    cout << "run time: " << duration.count() << " [sec]" << endl;

    if (print_result) {
        copy(res.begin(), res.end(), ostream_iterator<size_t>(std::cout, " "));
    }
    cout << endl;
}

void test_matrix(alglib_impl::ae_matrix& dist_mtx, size_t n_clusters = 2, bool print_result = false)
{
    //https://www.alglib.net/dataanalysis/clustering.php#header0
    //https://www.alglib.net/translator/man/manual.cpp.html#sub_clusterizersetdistances
    clusterizerstate s;
    ahcreport rep;
    integer_1d_array cidx;
    integer_1d_array cz;
    real_2d_array d = &dist_mtx;

    auto start = high_resolution_clock::now();

    clusterizercreate(s);
    clusterizersetahcalgo(s, 1); //1     single linkage
    clusterizersetdistances(s, d, true);
    clusterizerrunahc(s, rep);
    clusterizergetkclusters(rep, n_clusters, cidx, cz);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start) / 1000.0;

    cout << "run time: " << duration.count() << " [sec]" << endl;

    if (print_result) {
        for (size_t j = 0; j < cidx.length(); j++) {
            cout << cidx[j] << " ";
        }
    }

    cout << endl;
}

void ae_matrix_double_print(alglib_impl::ae_matrix& mat)
{
    for (size_t i = 0; i < mat.rows; i++) {
        for (size_t j = 0; j < mat.cols; j++) {
            cout << mat.ptr.pp_double[i][j] << "\t";
        }
        cout << endl;
    }
}

void ae_matrix_double_zero_init(alglib_impl::ae_matrix& mat, size_t row, size_t col)
{
    alglib_impl::ae_state _alglib_env_state;
    alglib_impl:ae_state_init(&_alglib_env_state);
    alglib_impl::ae_matrix_init(&mat, row, col, alglib_impl::DT_REAL, &_alglib_env_state, ae_true);

    for (size_t i = 0; i < mat.rows; i++) {
        for (size_t j = 0; j < mat.cols; j++) {
            mat.ptr.pp_double[i][j] = 0;
        }
    }
}

void ae_matrix_double_copy_vals(alglib_impl::ae_matrix& mat, Matrix& dist_mtx, bool remove_dist_mtx = false)
{
    vector<size_t> dist_mat_size = dist_mtx.getSize();
    if (mat.rows != dist_mat_size[0] || mat.cols != dist_mat_size[1]) {
        return;
    }

    for (size_t i = 0; i < dist_mat_size[0]; i++) {
        for (size_t j = 0; j < dist_mat_size[1]; j++) {
            mat.ptr.pp_double[dist_mat_size[0] - i - 1][j] = dist_mtx(dist_mat_size[0] - i - 1, j);
        }

        if (remove_dist_mtx) {
            dist_mtx.removeLastRow(true);
        }
    }
}

int main()
{
    size_t n_clusters = 2;
    bool print_result = false;
    bool code_optz = false;
    size_t mtx_size = 1000;

    for (int a = 0; a <= 1; a++)
    {
        code_optz = code_optz ? false : true;
        VecMatrix vec_mat = createRandomDistanceMatrix(mtx_size); //getDistanceMatrix("data_2_4.txt"); //createRandomDistanceMatrix(1000);
        MemMatrix mem_mat = copyMatrix(vec_mat);

        alglib_impl::ae_matrix ae_mat = { 0 };
        ae_matrix_double_zero_init(ae_mat, mem_mat.getRowSize(), mem_mat.getColSize());
        ae_matrix_double_copy_vals(ae_mat, mem_mat);

        cout << ((code_optz) ? "with" : "without") << " code optimization" << endl;
        cout << "vec matrix test: " << endl;
        test_matrix(vec_mat, n_clusters, code_optz, print_result);
        cout << endl;

        cout << "mem matrix test: " << endl;
        test_matrix(mem_mat, n_clusters, code_optz, print_result);
        cout << endl;

        cout << "alglib test: " << endl;
        test_matrix(ae_mat, n_clusters, print_result);
        cout << endl;

        alglib_impl::ae_matrix_destroy(&ae_mat);

        cout << "===================================================" << endl << endl;
    }

    return 0;
}