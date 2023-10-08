import numpy as np
import matplotlib
matplotlib.use('Agg')
from matplotlib.font_manager import FontProperties
import matplotlib.pyplot as plt
from  sklearn.cluster import KMeans
from sklearn.datasets import load_iris
from sklearn.preprocessing import StandardScaler

font = FontProperties()
font.set_family('serif')
font.set_name('Times New Roman')
font.set_style('italic')
font.set_size(15)

iris = load_iris()
data = StandardScaler().fit_transform(iris.data)
k_values = range(1, 11)
values = []

for k in range(1, 11):
    kmeans = KMeans(n_clusters = k, n_init = 10 ,init = 'k-means++', random_state=0)
    kmeans.fit(data)
    values.append(kmeans.inertia_)
average_dispersion = [values/len(data) for values in values]

difference = [average_dispersion[i] - average_dispersion[i+1] for i in range(len(average_dispersion) - 1)]
elbow_index = difference.index(max(difference)) + 1

fig, ax = plt.subplots(1, 1)
plt.plot(range(1, 11), average_dispersion)
plt.title('Elbow Method for selection of optimal "K" clusters', color="brown", weight = 'bold', fontproperties=font)
ax.set_xlabel('$\longrightarrow$K', weight = 'bold', ha='right')
ax.set_ylabel('Average Dispersion\n$\longrightarrow$', weight = 'bold')
circle = plt.Circle((k_values[elbow_index], average_dispersion[elbow_index]), radius = 0.3, color='black', fill=False, linestyle = 'dashed')
ax.add_artist(circle)
ax.set_aspect('equal', adjustable='datalim')
ax.annotate(
    "", xy=(k_values[elbow_index] + 0.2,average_dispersion[elbow_index] + 0.2), xytext=(3, 3),
    arrowprops=dict(arrowstyle="->", linestyle='dashed', linewidth=1)
)
ax.text(3.2, 3.2, 'Elbow Point', fontsize=10, ha='center', va='center')

plt.savefig('elbow.png')
