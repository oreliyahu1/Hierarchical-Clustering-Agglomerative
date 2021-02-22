#ifndef CLUSTERING_H
#define CLUSTERING_H
#include "Matrix.h"
#include <list>

using namespace std;

inline vector<size_t> get_non_zero_min_index(Matrix& dist_mtx)
{
	vector<size_t> min_index;
	vector<size_t> dist_mat_size = dist_mtx.getSize();
	double min_value = DBL_MAX;

	for (size_t i = 0; i < dist_mat_size[0]; i++) {
		for (size_t j = 0; j < i; j++) {
			double ij_val = dist_mtx(i, j);

			if (ij_val == 0) {
				continue;
			}

			if (ij_val < min_value)
			{
				min_index = { i, j };
				min_value = ij_val;
			}
		}
	}

	return min_index;
}

inline void vec_minimum_row(Matrix& dist_mtx, size_t dest, size_t cmp_to)
{
	for (size_t i = 0; i < dist_mtx.getColSize(); i++) {
		if (dist_mtx(dest, i) > dist_mtx(cmp_to, i)) {
			dist_mtx(dest, i) = dist_mtx(cmp_to, i);
		}
	}
}

inline void vec_minimum_col(Matrix& dist_mtx, size_t dest, size_t cmp_to)
{
	for (size_t i = 0; i < dist_mtx.getRowSize(); i++) {
		if (dist_mtx(i, dest) > dist_mtx(i, cmp_to)) {
			dist_mtx(i, dest) = dist_mtx(i, cmp_to);
		}
	}
}

vector<size_t> my_hierarchical_clustering_agglomerative_optz(Matrix& dist_mtx, size_t n_clusters = 2)
{
	const size_t num_of_points = dist_mtx.getRowSize();
	vector<size_t> result_clusters(num_of_points);
	vector<vector<size_t>> clustersList;

	for (size_t i = 0; i < num_of_points; i++) {
		clustersList.push_back({ i });
	}

	size_t* nearest = new size_t[num_of_points];
	for (size_t i = 0; i < num_of_points; i++) {
		double min_value = DBL_MAX;
		for (size_t j = 0; j < num_of_points; j++) {
			if (i == j) {
				continue;
			}

			double ij_val = dist_mtx(i, j);

			if (ij_val == 0) {
				continue;
			}

			if (ij_val < min_value)
			{
				nearest[i] = j + 1;
				min_value = ij_val;
			}
		}
	}

	size_t n_clusters_current = num_of_points;
	while (n_clusters_current > n_clusters)
	{
		double min_value = DBL_MAX;
		size_t min_i = 0;
		size_t min_j = 0;

		for (size_t i = 0; i < num_of_points; i++)
		{
			if (nearest[i] == 0) {
				continue;
			}

			double ij_val = dist_mtx(nearest[i] - 1, i);

			if (ij_val == 0) {
				continue;
			}

			if (ij_val < min_value)
			{
				min_i = i;
				min_j = nearest[i] - 1;
				min_value = ij_val;
			}
		}

		if (min_j < min_i) {
			swap(min_i, min_j);
		}

		vec_minimum_row(dist_mtx, min_i, min_j);
		vec_minimum_col(dist_mtx, min_i, min_j);

		clustersList[min_i].insert(clustersList[min_i].end(), clustersList[min_j].begin(), clustersList[min_j].end());
		clustersList[min_j].clear();

		nearest[min_j] = 0;
		min_value = DBL_MAX;
		for (size_t i = 0; i < num_of_points; i++)
		{
			if (nearest[i] == 0) {
				continue;
			}

			//if neraset to min_j change to min_i
			if (nearest[i] - 1 == min_j) {
				nearest[i] = min_i + 1;
			}

			//new neraset for min_i
			double ij_val = dist_mtx(min_i, i);

			if (ij_val == 0) {
				continue;
			}

			if (ij_val < min_value)
			{
				min_value = ij_val;
				nearest[min_i] = i + 1;
			}
		}

		n_clusters_current--;
	}

	size_t cluster_i = 0;
	for (size_t i = 0; i < clustersList.size(); i++) {
		if (nearest[i] == 0) {
			continue;
		}

		for (size_t j = 0; j < clustersList[i].size(); j++) {
			result_clusters[clustersList[i][j]] = cluster_i;
		}
		cluster_i++;
	}

	delete[] nearest;

	return result_clusters;
}

vector<size_t> my_hierarchical_clustering_agglomerative_wo_optz(Matrix& dist_mtx, size_t n_clusters = 2)
{
	vector<size_t> dist_mat_size = dist_mtx.getSize();
	vector<size_t> result_clusters(dist_mat_size[0]);
	vector<vector<size_t>> clustersList;

	for (size_t i = 0; i < dist_mat_size[0]; i++) {
		clustersList.push_back({ i });
	}

	while (clustersList.size() > n_clusters)
	{
		vector<size_t> non_zero_min = get_non_zero_min_index(dist_mtx);
		size_t min_i = non_zero_min[1], min_j = non_zero_min[0];

		vec_minimum_row(dist_mtx, min_i, min_j);
		vec_minimum_col(dist_mtx, min_i, min_j);

		clustersList[min_i].insert(clustersList[min_i].end(), clustersList[min_j].begin(), clustersList[min_j].end());

		dist_mtx.setCopyCol(min_j, dist_mat_size[1] - 1);
		dist_mtx.setCopyRow(min_j, dist_mat_size[0] - 1);

		clustersList[min_j] = clustersList[clustersList.size() - 1];
		//clustersList[min_j].assign(clustersList[clustersList.size()-1].begin(), clustersList[clustersList.size() - 1].end());

		dist_mtx.removeLastCol();
		dist_mtx.removeLastRow();
		dist_mat_size = dist_mtx.getSize();
		clustersList.pop_back();
	}

	for (size_t i = 0; i < clustersList.size(); i++) {
		for (size_t j = 0; j < clustersList[i].size(); j++) {
			result_clusters[clustersList[i][j]] = i;
		}
	}

	return result_clusters;
}

vector<size_t> my_hierarchical_clustering_agglomerative(Matrix& dist_mtx, size_t n_clusters = 2, bool fast = false)
{
	if (fast) {
		return my_hierarchical_clustering_agglomerative_optz(dist_mtx, n_clusters);
	}
	return my_hierarchical_clustering_agglomerative_wo_optz(dist_mtx, n_clusters);
}

#endif