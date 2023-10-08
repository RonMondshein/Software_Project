import pandas as pd
import numpy as np
import math
import sys
import mykmeanssp as kmeans

args = sys.argv

if len(args) > 6 or len(args) < 5:
    print("An Error Has Occurred")
    exit(1)
# recieve command line arguments
if len(args) == 6:
    k = args[1]
    iter = args[2]
    epsilon = args[3]
    filename1 = args[4]
    filename2 = args[5]

else:
    k = args[1]
    iter = 300
    epsilon = args[2]
    filename1 = args[3]
    filename2 = args[4]

# Read the two input files
df1 = pd.read_csv(filename1, header=None)
df2 = pd.read_csv(filename2, header=None)
df1[0] = df1[0].astype(float)
df2[0] = df2[0].astype(float)


pd.set_option('display.max_rows', None)

# Perform inner join using the first column as the key
merged_df = pd.merge(df1, df2, on=0, how='inner')
sorted_df = merged_df.sort_values(by=[0])

num_of_lines = len(sorted_df)

# check the input values
try:
    k = int(k)
except:
    print("Invalid number of clusters!")
    try:
        iter = int(iter)
    except:
        print("Invalid maximum iteration!")
        exit(1)
   
    if (iter < 2 or iter >= 1000):
        print("Invalid maximum iteration!")
    exit(1)
if (k < 2 or k >= num_of_lines):
    print("Invalid number of clusters!")
    try:
        iter = int(iter)
    except:
        print("Invalid maximum iteration!")
        exit(1)
    if (iter < 2 or iter >= 1000):
        print("Invalid maximum iteration!")
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
n_array = []
for _, row in sorted_df.iterrows():
    n_array.append(row.values[1:])
i=0
d = n_array[0].size
#indexes array
indexes = []
#cluster array
clusters = []
#distance array
distances = []

#calculate the euclidean distance between two points
def euclidean_distance(x, miu):
    sum = 0
    for i in range(len(x)):
        sum += (x[i] - miu[i])**2
    return math.sqrt(sum)

#choose the first centroid randomly
np.random.seed(0)
first_centroid_index = np.random.choice(range(len(n_array)))
center = n_array[first_centroid_index]
indexes.append(first_centroid_index)
clusters.append(n_array[first_centroid_index])
distances = [euclidean_distance(x, center) for x in n_array]

#Choose k-1 new centers using weighted probability distribution
for _ in range(k - 1):
    distances = []
    probabilities = []
    distances = [min([euclidean_distance(x, center) for center in clusters]) for x in n_array]
    probabilities = distances / np.sum(distances)
    new_center_index = np.random.choice(range(len(n_array)), p=probabilities)
    indexes.append(new_center_index)
    clusters.append(n_array[new_center_index])

clusters_np = np.empty((0,d), float) 
for i in range(len(clusters)):
    clusters_np = np.append(clusters_np, [clusters[i]], axis=0)
final_centroids = kmeans.fit(sorted_df.iloc[:, 1:].values.tolist(), clusters_np.tolist(), k, iter, (float)(epsilon), d, len(n_array))
if final_centroids is None:
    print("An Error Has Occurred")
    exit(1)


#print the output
output_indexes = ",".join(str(x) for x in indexes)
print(output_indexes)

for row in final_centroids:
    row_str = ",".join("{:.4f}".format(value) for value in row)
    print(row_str, end='\n')

exit(0)
