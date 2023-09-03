import symnmf
from sklearn.metrics import silhouette_score
from sklearn.metrics.pairwise import pairwise_distances
import numpy as np
import math
from copy import *
import sys
def input_data(vector_file):
    ret_vectors = []
    with open(vector_file, 'r') as file:
        for line in file:
            values = line.strip().split(',')
            vector = [float(number) for number in values]
            ret_vectors.append(vector)
    return ret_vectors
def initialize_H(W, k):
    np.random.seed(0)
    m = np.mean(W)  
    upper_bound = 2 * np.sqrt(m / k)   
    n = len(W)  
    H = np.random.uniform(0, upper_bound, size=(n, k))
    return H.tolist()
def euclidian_distance(v1, v2): # method for calculating the euclidian 
# distance between 2 vectors of the same length
    ret = sum((i1 - i2) ** 2 for i1, i2 in zip(v1, v2))
    return float(math.sqrt(ret))
def kmeans(k, max_iter, vector_list):
    vector_length = len(vector_list) #amount of vectors (N)
    vector_size = len(vector_list[0]) #length of vector (d)
    clusters = [deepcopy(vector) for vector in vector_list[:k]]  # initializing the cluster list as the first K data points
    cluster_indexes = [0 for _ in range(vector_length)]
    convergence = [0 for _ in range(k)]  # checking if the clusters have converged
    loop_counter = 0
    EPSILON = 0.0001

    while loop_counter < max_iter:
        for i in range(vector_length):  # finding the closest cluster to each vector
            min_dist = euclidian_distance(vector_list[i], clusters[0])
            min_index = 0
            for j in range(1, k):
                distance = euclidian_distance(vector_list[i], clusters[j])
                if distance < min_dist:
                    min_index = j
                    min_dist = distance
            cluster_indexes[i] = min_index
        for i in range(k):  # updating the clusters
            prev_cluster = clusters[i][:]  # saving the vector before update
            count = 0
            update_vec = [0 for _ in range(vector_size)]
            for j in range(vector_length):
                if cluster_indexes[j] == i:
                    for s in range(vector_size):
                        update_vec[s] += vector_list[j][s]
                    count += 1
            if count > 0:  # at least 1 vector belongs to the cluster
                for s in range(vector_size):
                    clusters[i][s] = float(update_vec[s]/count)
            dist_converge = euclidian_distance(prev_cluster, clusters[i])
            if dist_converge < EPSILON:
                convergence[i] = -1
            count_converge = 0
            if i == k-1:  # end of the cluster array, need to check convergence for them
                count_converge = convergence.count(-1)
            if count_converge == k:
                loop_counter = max_iter
            else:
                count_converge = 0
        loop_counter += 1
    return clusters
def symnfcompare(x_list, d, n,k):
    W = symnmf.fit(x_list, d, n)  # Call the wrapped function with the list

    # Initialize H
    m = np.mean(W)
    np.random.seed(0)
    H = np.random.uniform(0, 2 * np.sqrt(m / k), size=(n, k))  


    W = np.array(W, dtype=np.float64)  # Convert W to a NumPy array
    H = np.array(H, dtype=np.float64)  # Convert H to a NumPy array
    
    # Constants for the update rule
    beta = 0.5
    epsilon = 1e-4  # Convergence threshold
    # Iterative update of H
    converged = False
    max_iterations = 300  # You can adjust this
    iteration = 0

    W = np.array(W)  # Convert W to a NumPy array   

    while not converged and iteration < max_iterations:
        WH = np.dot(W, H)
        H_new = H * (1-beta+beta*((WH) / (np.dot(np.dot(H, H.T), H))) )
        
        # Check for convergence
        if np.linalg.norm(H_new - H, 'fro')**2 < epsilon:
            converged = True
        H =  H_new
        iteration += 1
    return (H)
def main():
    k = int(sys.argv[1])
    file_name = sys.argv[2]
    vectors = input_data(file_name)
    N = len(vectors)  # rows 
    D = len(vectors[0])  # columns
    cluster_centroids = kmeans(3,300,vectors)
    kmeans_distances = pairwise_distances(vectors, cluster_centroids)
    kmeans_labels = np.argmin(kmeans_distances, axis=1)
    kmeans_score = silhouette_score(vectors, kmeans_labels)
    x = np.loadtxt(file_name, delimiter=',')
    x_list = x.tolist()
    sym_labels = symnfcompare(x_list,D,N,k)
    sym_labels = np.argmax(sym_labels, axis=1)
    # print (kmeans_labels)
    # print ("--------------------------")
    # print (sym_labels)
    # W = Sym.norm(vectors,D,N)
    # H = initialize_H(W,k)
    # final = Sym.symnmf(H,W)
    # sym_labels = np.argmax(final, axis=1)  # Assign each element to the cluster with highest association score
    
    sym_score = silhouette_score(vectors, sym_labels)
    formatted_element_kmeans = "{:.4f}".format(kmeans_score)
    formatted_element_sym = "{:.4f}".format(sym_score)
    print("nmf: ", formatted_element_sym)
    print("kmeans: ", formatted_element_kmeans)
    
    
if __name__ == "__main__":
    main()

