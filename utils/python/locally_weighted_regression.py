import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# TODO: look up how to reference the csv given bus values for V and I as well as SOH from model for predicting SOC
# TODO: What is the best K value to use for the locally weighted regression
# TODO: How to compress the output for microcontroller use

# Gaussian kernel
def kernel(point, xmat, k):
    m,n = np.shape(xmat)
    weights = np.mat(np.eye((m)))
    
    for j in range(m):
        diff = point - X[j]
        weights[j, j] = np.exp(diff * diff.T / (-2.0 * k**2))
    
    return weights

def localWeight(point, xmat, ymat, k):
    wt = kernel(point, xmat, k)
    W = (X.T * (wt*X)).I * (X.T * wt * ymat.T)
    return W

def localWeightRegression(xmat, ymat, k):
    m,n = np.shape(xmat)
    ypred = np.zeros(m)
    
    for i in range(m):
        ypred[i] = xmat[i] * localWeight(xmat[i], xmat, ymat, k)
        
    return ypred

data = pd.read_csv('../../res/model_data/batemo_model_data.csv')
data = data[data.V >= 2.9]

min_SOH = data.SOH.min()
max_SOH = data.SOH.max()
min_I = data.I.min()
max_I = data.I.max()

# for i in range(min_SOH, max_SOH + 1):
#     for j in range(min_I, max_I + 1):

for i in range(90,91):
    for j in range(10,11):

        data = data[data.SOH == i]
        data = data[data.I == j]

        colA = np.array(data.V)
        colB = np.array(data.SOC)

        mcolA = np.mat(colA)
        mcolB = np.mat(colB)

        m = np.shape(mcolB)[1]
        one = np.ones((1, m), dtype = int)

        # horizontal stacking
        X = np.hstack((one.T, mcolA.T))
        print(X.shape)

        ypred = localWeightRegression(X, mcolB, 0.1)

        # append the data to a csv
        export = pd.DataFrame()
        export['V'] = colA.round(3)
        export['I'] = j
        export['SOH'] = i
        export['SOC'] = np.round(ypred,3)
        export.to_csv('new csv.csv', mode='a', header=False)

        xsort = X.copy()
        xsort.sort(axis=0)
        plt.scatter(colA, colB, color='blue')
        #                V           SOC
        plt.plot(xsort[:, 1], ypred[X[:, 1].argsort(0)], color='yellow', linewidth=5)
        plt.xlabel('V')
        plt.ylabel('SOC')
        plt.show()

