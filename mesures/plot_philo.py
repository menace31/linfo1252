import numpy as np
import matplotlib.pyplot as plt


threads = np.array([2,4,8,16,32,64])
nThreads = len(threads)
nVersions = 3

data = np.genfromtxt("intel_philo.csv", dtype=None, delimiter=',', skip_header=1)

means = np.zeros((nThreads,nVersions))
sigma = np.zeros((nThreads,nVersions))
results = np.zeros((nThreads*nVersions,7))


for i in range(nVersions*nThreads):
    for j in range(7):
        results[i][j] = data[i][j]
    means[i % nThreads,i//nThreads] = np.mean(results[i,2:])
    sigma[i % nThreads,i//nThreads] = np.std(results[i,2:])

colors=["blue","orange","green"]
for i in range(nVersions):
    plt.errorbar(threads,means[:,i],yerr=sigma[:,i],color=colors[i],linewidth=2)
    plt.plot(threads,means[:,i],"o",color=colors[i])
plt.xlim(left=0)
plt.ylim(bottom=0)
plt.grid()
plt.title("Problème des philosophes",fontweight="bold")
plt.xlabel("Nombre total de threads")
plt.ylabel("Temps moyen[s]")
plt.legend(["POSIX","Test&Set","Test&Test&Set"],loc=2)
plt.show()
