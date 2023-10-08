import sys
import pandas as pd
import math
from symnmf import analysis as analysis_symnmf
from sklearn.metrics import silhouette_score

#HW1
#calculate the euclidean distance between two points
def euclidean_distance(x, miu):
    sum = 0
    for i in range(len(x)):
        sum += (x[i] - miu[i])**2
    return math.sqrt(sum)
#HW1
#calculate the mean of the centroids and update the current centroid
def centroids_mean(miu):
    num_of_points = len(miu[1])
    d = len(miu[1][0])
    mean_array = [0 for i in range(d)]
    for j in range(d):
        sum = 0
        for centroid in miu[1]:
            sum += centroid[j]
        mean_array[j] = sum/num_of_points
    miu[0] = mean_array
#HW1
#find the closest centroid to the point x
def argmin(miu_array, x):
    min = euclidean_distance(x, miu_array[0][0])
    min_centroid = miu_array[0]
    min_place = 0
    for i in range(1, len(miu_array)):
        if(euclidean_distance(x, miu_array[i][0]) < min):
            min = euclidean_distance(x, miu_array[i][0])
            min_centroid = miu_array[i]
            min_place = i
    min_centroid[1].append(x)
    return min_place
#HW1
#copy the centroids to a new array
def copy_centroids(miu_array):
    mean_centroids = []
    for centroid in miu_array:
        mean_centroids.append(centroid[0])
    return mean_centroids
def printError():
    print("An Error Has Occurred")
    exit(1)

def makeClusterFromH(H):
    cluster = []
    for i in range(len(H)):   
        cluster.append(H[i].index(max(H[i])))
    return cluster

if __name__ == "__main__":
    if len(sys.argv) != 3:
        printError()
    k = sys.argv[1]
    try:
        k = int(k)
        if k < 1:
            printError()
    except ValueError:
        printError() 
    filename = sys.argv[2]
    H = analysis_symnmf(k, filename)
    H_cluster = makeClusterFromH(H)
    df = pd.read_csv(filename, header=None)
    clusters = [[] for i in range(int(k))]
    for i in range(len(H_cluster)):
        place = H_cluster[i]
        clusters[place].append(df.iloc[i].tolist())
    silhouette_avg = silhouette_score(df, H_cluster)
    print("nmf: %.4f"% silhouette_avg)

    #HW1
    epsilon = 0.001
    #initialize the main array
    n_array = df.values.tolist()

    #initialize the miu array that includes the centroids
    miu_array = [[[[]]] for i in range(k)]
    for i in range(k):
        miu_array[i] = [[x for x in n_array[i]]]
        miu_array[i].append([])
        #main part of the program

    convergence = False
    iter_cnt = 0
    iter = 300
    places = [0 for i in range(len(n_array))]
    #while the convergence is not reached and the number of iterations is less than the maximum number of iterations
    while convergence == False and iter_cnt < iter:
        for i in range(len(n_array)):  
            places[i] = argmin(miu_array, n_array[i])
        old_centroids = copy_centroids(miu_array)
        iter_cnt += 1
        for j in range(len(miu_array)):
            centroids_mean(miu_array[j])
        new_centroids = copy_centroids(miu_array)
        convergence = True
        for i in range(len(miu_array)):
            delta = euclidean_distance(old_centroids[i], new_centroids[i])
            if delta >= epsilon:
                convergence = False
                break
        for centroid in miu_array:
            centroid[1] = []
    silhouette_avg_kmeans = silhouette_score(n_array, places)
    print("kmeans: %.4f"% silhouette_avg_kmeans)
