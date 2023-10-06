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

def distance(a, b):
    distance = 0
    for i in range(len(a)):
        distance = distance +  ((a[i]-b[i])**2) 
    return math.sqrt(distance)


#Calculates new centroid for requested cluster
def centroidCalculator(cluster):
    newCentroid = [0 for x in cluster[0]]
    for vector in cluster:
        for i in range(len(vector)):
            newCentroid[i] = newCentroid[i] + vector[i]
    
    for i in range(len(newCentroid)):
        newCentroid[i] = newCentroid[i]/len(cluster)
    return newCentroid



#Updates all centroids
def updateCentroids(clustersList, centroidsList,K):
    
    epsilon = 1e-4
    epsilonIndicator = True
    for i in range(K):
        newCentroid = centroidCalculator(clustersList[i])
        if (distance(newCentroid,centroidsList[i]) >= epsilon):
            epsilonIndicator = False
        centroidsList[i] = newCentroid
    return epsilonIndicator

def kmeans(K,iter,vectorsList,D,N):      
    centroidsList = [i for i in vectorsList[:K]]#array of centroids with k length
    clustersList = [[centroid] for centroid in centroidsList] # k reshimot
    iterCounter = 0
    epsilonIndicator = False
    while not epsilonIndicator and iterCounter<iter:
        for i in range(N): #run over vector list
            currVector = vectorsList[i]
            minDist = float('inf')
            minIndex = -1
            for j in range(K): #run over centroids list
                dist = distance(currVector, centroidsList[j])
                if(dist < minDist):#finding relevant cluster
                    minDist = dist
                    minIndex = j
            for h in range (K):
              if clustersList[h].count(currVector) >= 1:
              #  if currVector in clustersList[h]:
                
                   clustersList[h].remove(currVector)
            for h in range (K):
                if minIndex == h:        
                    clustersList[h].append(currVector)#updating relevant cluster
        
        iterCounter += 1    

        epsilonIndicator = updateCentroids(clustersList, centroidsList,K)#check if all the cluster has changed according to the terms 
    return (centroidsList)

# def print_matrix(H):
#     for row in H:
#         print(",".join("{:.4f}".format(number) for number in row))

def symnfcompare(x_list, d, n,k):
    W = symnmf.norm(x_list, d, n)  # Call the wrapped function with the list
        
    # Initialize H
    m = np.mean(W)
    np.random.seed(0)
    H = np.random.uniform(0, 2 * np.sqrt(m / k), size=(n, k)) 
    H = H.tolist() 

    H = symnmf.symnmf(W, H, d, n, k)
    # print_matrix(H)
 
    return (H)
def main():
    k = int(sys.argv[1])
    file_name = sys.argv[2]
    vectors = input_data(file_name)
    N = len(vectors)  # rows 
    D = len(vectors[0])  # columns
    cluster_centroids = kmeans(k,300,vectors,D,N)
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
    print("nmf:", formatted_element_sym)
    print("kmeans:", formatted_element_kmeans)
    
    
if __name__ == "__main__":
    main()

