import numpy as np


def my_hierarchical_clustering_agglomerative(dist_mtx, n_clusters=2, fast = False):
    if (fast):
        return my_hierarchical_clustering_agglomerative_optz(dist_mtx, n_clusters)
    return my_hierarchical_clustering_agglomerative_wo_optz(dist_mtx, n_clusters)

    
def my_hierarchical_clustering_agglomerative_optz(dist_mtx, n_clusters=2):
    num_of_points = dist_mtx.shape[0];
    
    # clusters as size of the points, in the end it will be size 'n_clusters'
    clusters_list = [[i] for i in range(num_of_points)]
    result_clusters = np.zeros(shape=(num_of_points), dtype=np.int32)

    dist_mtx = np.copy(dist_mtx)
    dist_mtx[dist_mtx == 0] = np.nan
    
    nearest = np.zeros(shape=(num_of_points), dtype=np.float64)
    nearest_vals = np.zeros(shape=(num_of_points), dtype=np.float64)
    for i in range(num_of_points):
        min_values_location = np.where(dist_mtx[:,i]==np.nanmin(dist_mtx[:,i]))
        nearest[i] = min_values_location[0][0]
        nearest_vals[i] = dist_mtx[min_values_location[0][0], i]


    n_clusters_current = num_of_points
    # iterate until we have n_clusters clusters
    while n_clusters_current > n_clusters:
        min_values_location = np.where(nearest_vals==np.nanmin(nearest_vals))
        min_i = min_values_location[0][0]
        min_j = np.int64(nearest[min_i])
        if min_j < min_i:
            min_i, min_j = min_j, min_i

        # single-linkage clustering, replace the col&row as min of i, j
        dist_mtx[:,min_i] = np.minimum(dist_mtx[:, min_i], dist_mtx[:, min_j])
        dist_mtx[min_i,:] = np.minimum(dist_mtx[min_i, :], dist_mtx[min_j, :])

        # unite the two closest clusters
        clusters_list[min_i].extend(clusters_list[min_j])
        clusters_list[min_j].clear()

        nearest[min_j] = np.nan
        nearest_vals[min_j] = np.nan
        nearest[nearest == min_j] = min_i

        min_values_location = np.where(dist_mtx[:,min_i]==np.nanmin(dist_mtx[:,min_i]))
        nearest[min_i] = min_values_location[0][0]
        nearest_vals[min_i] = dist_mtx[min_values_location[0][0], min_i]

        n_clusters_current -= 1


    cluster_in = 0
    # post-processing, calculating the clusters vector
    for cluster_i, cluster in enumerate(clusters_list):
        if len(cluster) == 0:
            continue

        for i in cluster:
            result_clusters[i] = cluster_in

        cluster_in += 1

    return result_clusters


def my_hierarchical_clustering_agglomerative_wo_optz(dist_mtx, n_clusters=2):
    # clusters as size of the points, in the end it will be size 'n_clusters'
    clusters_list = [[i] for i in range(dist_mtx.shape[0])]
    result_clusters = np.zeros(shape=(dist_mtx.shape[0]), dtype=np.int32)

    dist_mtx = np.copy(dist_mtx)
    dist_mtx[dist_mtx == 0] = np.nan

    # iterate until we have n_clusters clusters
    while len(clusters_list) > n_clusters:
        # find the index of the min distance
        min_values_location = np.where(dist_mtx==np.nanmin(dist_mtx))
        min_i, min_j = min_values_location[0][0], min_values_location[1][0]

        # single-linkage clustering, replace the col&row as min of i, j
        dist_mtx[:,min_i] = np.minimum(dist_mtx[:, min_i], dist_mtx[:, min_j])
        dist_mtx[min_i,:] = np.minimum(dist_mtx[min_i, :], dist_mtx[min_j, :])

        # unite the two closest clusters
        clusters_list[min_i].extend(clusters_list[min_j])

        # remove redundant col&row
        dist_mtx[:,min_j] = dist_mtx[:,-1]
        dist_mtx[min_j,:] = dist_mtx[-1,:]
        clusters_list[min_j] = clusters_list[-1]

        dist_mtx = dist_mtx[:-1,:-1]
        clusters_list.pop()


    # post-processing, calculating the clusters vector
    for cluster_i, cluster in enumerate(clusters_list):
        for i in cluster:
            result_clusters[i] = cluster_i


    return result_clusters