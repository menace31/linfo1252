import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob

print("start")
filelistes = glob.glob("*.csv")
for filename in filelistes:
    file = pd.read_csv(filename)
    f = pd.DataFrame(data=file)
    for j in [2]:
        f = f[f['n_prog']==j]
        means = []
        ecart_type = []
        N = [1,2,4,8,16,32,64]
        for i in N:
            temps = []
            temps.append(f[f['nthreads']==i]['time1'])
            temps.append(f[f['nthreads']==i]['time2'])
            temps.append(f[f['nthreads']==i]['time3'])
            temps.append(f[f['nthreads']==i]['time4'])
            temps.append(f[f['nthreads']==i]['time5'])
            means.append(np.mean(temps))
            ecart_type.append(np.std(temps))

        plt.errorbar(N,means,yerr=ecart_type)
    plt.title(filename)
    plt.xlabel("nthread")
    plt.ylabel("time")
    #plt.plot(time[:5])
    plt.show()
