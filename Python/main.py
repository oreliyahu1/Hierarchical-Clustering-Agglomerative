import os
import numpy as np
import time
from datetime import datetime
from sklearn.cluster import AgglomerativeClustering
import hierarchical_clustering as hc
from sklearn.neighbors import DistanceMetric
import matplotlib.pyplot as plt


def save_data(file, npy_array):
    np.save(file + '.npy', npy_array)


def load_data(file):
    if not os.path.exists(file + '.npy'):
        npy_array = np.loadtxt(file, delimiter=",", dtype=np.float64)
        save_data(file, npy_array)
        return npy_array
    return np.load(file + '.npy')


def random_distance_matrix(number_of_points, dimension=2):
    points_array = np.random.uniform(low=0, high=400, size=(number_of_points, dimension))
    points_array = np.around(points_array, decimals=2)
    dist = DistanceMetric.get_metric('euclidean')
    return dist.pairwise(points_array)
   
 
def print_plot(name, t_test, t_alg, t_my_alg):
    plt.figure()
    plt.plot(t_test, t_alg)
    plt.plot(t_test, t_my_alg)
    plt.title(name)
    plt.ylabel('Time [Sec]')
    plt.xlabel('Distance Matrix Size')
    plt.legend(['sklearn library', 'my alg'], loc='upper left')
    plt.savefig(str(datetime.now().timestamp()) + '.png')
    plt.show()


def hc_test(distance_matrix, n_clusters=2, code_optz = False):
    print("Clustring")
    start_time = time.time()
    clustering = AgglomerativeClustering(n_clusters=n_clusters, linkage='single', affinity='precomputed').fit(distance_matrix)
    clustering_l = clustering.labels_
    hc_total_time = time.time() - start_time
    print("Completed")
    print("--- %s seconds ---" % hc_total_time)
    
    print("My Clustring")
    start_time = time.time()
    my_clustering = hc.my_hierarchical_clustering_agglomerative(distance_matrix, n_clusters=n_clusters, fast=code_optz)
    my_hc_total_time = time.time() - start_time
    print("Completed")
    print("--- %s seconds ---" % my_hc_total_time)
    return hc_total_time, my_hc_total_time


def full_hc_test(n_clusters=2, matrix_jump_range=range(1, 25 + 1), add_mtx=0, j_mtx=200, code_optz=False):
    t_alg = []
    t_my_alg = []
    t_test = []

    for i in matrix_jump_range:
        numberofp = add_mtx + i * j_mtx
        print("Create distance matrix")
        start_time = time.time()
        distance_matrix = random_distance_matrix(numberofp)   
        total_time = time.time() - start_time
        t_test.append(numberofp)
        print("Distance size: " + str(distance_matrix.shape))
        print("Completed")
        print("--- %s seconds ---" % total_time)
     
        hc_total_time, my_hc_total_time = hc_test(distance_matrix, n_clusters, code_optz)
        t_alg.append(hc_total_time)
        t_my_alg.append(my_hc_total_time)
        print("--------------------------------------\n")
    
    return t_test, t_alg, t_my_alg


def test_from_file(file, n_clusters=2, code_optz = False):
    print("Loading file")
    start_time = time.time()
    distance_matrix = load_data(file)
    total_time = time.time() - start_time
    print("Distance size: " + str(distance_matrix.shape))
    print("Completed")
    print("--- %s seconds ---" % total_time)
    
    hc_total_time, my_hc_total_time = hc_test(distance_matrix, n_clusters, code_optz)
    print("--------------------------------------\n")
    
    return hc_total_time, my_hc_total_time
    
    
if __name__ == '__main__':
    code_optz = True
    
    '''
    test_from_file('data_2_4.txt', code_optz=code_optz)
    '''

    list_all = []
    for n_clusters in range(2, 5 + 1):
        t_alg = []
        t_my_alg = []
        t_test = []

        # 0-1000, J:50
        tt, ta, tma = full_hc_test(n_clusters=n_clusters, matrix_jump_range=range(1, 20 + 1), j_mtx=50, code_optz=code_optz)
        t_test.extend(tt)
        t_alg.extend(ta)
        t_my_alg.extend(tma)
        
        # 1000-5000, J:500
        tt, ta, tma = full_hc_test(n_clusters=n_clusters, matrix_jump_range=range(1, 8 + 1), add_mtx=1000, j_mtx=500, code_optz=code_optz)
        t_test.extend(tt)
        t_alg.extend(ta)
        t_my_alg.extend(tma)

        # 5000-10000, J:1000
        tt, ta, tma = full_hc_test(n_clusters=n_clusters, matrix_jump_range=range(1, 5 + 1), add_mtx=5000, j_mtx=1000, code_optz=code_optz)
        t_test.extend(tt)
        t_alg.extend(ta)
        t_my_alg.extend(tma)
        
        list_all.append([[n_clusters], t_alg, t_my_alg, t_test])

        print_plot("Test Results K=" + str(n_clusters), t_test, t_alg, t_my_alg)

    
