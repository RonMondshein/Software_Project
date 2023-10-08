import math
import sys
args = sys.argv
epsilon = 0.001

# recieve command line arguments
if len(args) == 4:
    k = args[1]
    iter = args[2]
    filename = args[3]
else:
    k = args[1]
    iter = 200
    filename = args[2]

# Open the file in read mode
with open(filename, 'r') as f:
    # Read the lines of the file into a list
    lines = f.readlines()

# check the input values
try:
    k = int(k)
except:
    print("Invalid number of clusters!")
    exit(1)
if (k < 2 or k >= len(lines)):
    print("Invalid number of clusters!")
    exit(1)
try:
    iter = int(iter)
except:
    print("Invalid maximum iteration!")
    exit(1)
if (iter < 2 or iter >= 1000):
    print("Invalid maximum iteration!")
    exit(1)

#initialize the main array
n_array = [[] for i in range(len(lines))]

for i in range(len(lines)):
    n_array[i] = [float(x) for x in lines[i].strip().split(",")]

#initialize the miu array that includes the centroids
miu_array = [[[[]]] for i in range(k)]
for i in range(k):
    miu_array[i] = [[x for x in n_array[i]]]
    miu_array[i].append([])

#calculate the euclidean distance between two points
def euclidean_distance(x, miu):
    sum = 0
    for i in range(len(x)):
        sum += (x[i] - miu[i])**2
    return math.sqrt(sum)

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

#find the closest centroid to the point x
def argmin(miu_array, x):
    min = euclidean_distance(x, miu_array[0][0])
    min_centroid = miu_array[0]
    for i in range(1, len(miu_array)):
        if(euclidean_distance(x, miu_array[i][0]) < min):
            min = euclidean_distance(x, miu_array[i][0])
            min_centroid = miu_array[i]
    min_centroid[1].append(x)

#copy the centroids to a new array
def copy_centroids(miu_array):
    mean_centroids = []
    for centroid in miu_array:
        mean_centroids.append(centroid[0])
    return mean_centroids

#main part of the program
convergence = False
iter_cnt = 0
#while the convergence is not reached and the number of iterations is less than the maximum number of iterations
while convergence == False and iter_cnt < iter:
    for i in range(len(n_array)):
        argmin(miu_array, n_array[i])
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
    
#print the output
for i in range(len(miu_array)):
    for j in range(len(miu_array[i][0])):
        if(j == len(miu_array[i][0]) - 1):
            print("%.4f" % miu_array[i][0][j], end = "\n")
        else:
            print("%.4f" % miu_array[i][0][j], end = ",")
print("")
exit(0)