import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# TODO: look up how to reference the csv given bus values for V and I as well as SOH from model for predicting SOC

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
# min_I = data.I.min()
# max_I = data.I.max()
# Convert min_I and max_I to integers
min_I = int(data.I.min())
max_I = int(data.I.max())

# for i in range(0, 101):
#     for j in range(-55, 56):

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

        # ypred = localWeightRegression(X, mcolB, 0.02)

        # find the variance of ypred and colB looping through different k values
        # find the optimal k value that minimizes the variance

        # Initialize lists to store k values and corresponding variances
        k_values = []
        variances = []

        # Range of k values to try
        k_range = np.linspace(0.01, 0.1, 10)  # Adjust as needed

        for k in k_range:
            ypred = localWeightRegression(X, mcolB, k)
            variance = np.var(ypred - colB)  # Calculate variance between ypred and colB
            k_values.append(k)
            variances.append(variance)

        # Find the index of the minimum variance
        min_variance_index = np.argmin(variances)
        optimal_k = k_values[min_variance_index]
        min_variance = variances[min_variance_index]

        print("For i =", i, "and j =", j)
        print("Optimal k:", optimal_k)
        print("Min Variance:", min_variance)

        # Plot the variance vs. k values
        # plt.plot(k_values, variances)
        # plt.xlabel('k')
        # plt.ylabel('Variance')
        # plt.title('Variance vs. k')
        # plt.show()

        # append the data to a csv
        export = pd.DataFrame()
        export['V'] = colA.round(3)
        export['I'] = j
        export['SOH'] = i
        export['SOC'] = np.round(ypred,3)
        export.to_csv('new csv.csv', mode='a', header=False)

        # xsort = X.copy()
        # xsort.sort(axis=0)
        # plt.scatter(colA, colB, color='blue')
        # #                V           SOC
        # plt.plot(xsort[:, 1], ypred[X[:, 1].argsort(0)], color='yellow', linewidth=5)
        # plt.xlabel('V')
        # plt.ylabel('SOC')
        # plt.show()

# sample code for compressing the csv and prepping it to be flashed onto the microcontroller:
# import zlib
#
# # Read CSV data
# data = pd.read_csv('new csv.csv')
#
# # Convert data to binary format
# binary_data = data.to_string().encode('utf-8')
#
# # Compress binary data
# compressed_data = zlib.compress(binary_data)
#
# # Write compressed data to file
# with open('compressed_data.bin', 'wb') as f:
#     f.write(compressed_data)

# To read the compressed data onto the microcontroller's memory using C++, you need to develop code that can read the binary file and decompress it. Here's a general outline of the steps involved:
#
# Read the Binary File: Open the compressed binary file for reading in binary mode (std::ios::binary). Read the contents of the file into a buffer.
#
# Decompress the Data: Use zlib or another suitable decompression library to decompress the binary data. You'll need to allocate memory to store the decompressed data. Decompress the data from the buffer into this memory.
#
# Process the Decompressed Data: Once you have the decompressed data in memory, you can process it as needed. This may involve parsing the data, converting it back to its original format (e.g., CSV), and performing any necessary operations.
#
# Close File and Free Memory: Close the file after reading and release any dynamically allocated memory used for decompressed data.
# #include <iostream>
# #include <fstream>
# #include <vector>
# #include <zlib.h>
# int main() {
#     // Open the compressed binary file for reading
#     std::ifstream file("compressed_data.bin", std::ios::binary | std::ios::ate);
#     if (!file.is_open()) {
#         std::cerr << "Failed to open file for reading." << std::endl;
#         return 1;
#     }
#
#     // Determine file size
#     std::streamsize size = file.tellg();
#     file.seekg(0, std::ios::beg);
#
#     // Read the entire file into a buffer
#     std::vector<char> buffer(size);
#     if (!file.read(buffer.data(), size)) {
#         std::cerr << "Failed to read from file." << std::endl;
#         return 1;
#     }
#
#     // Close the file
#     file.close();
#
#     // Decompress the data
#     std::vector<char> decompressed_buffer(size); // Assuming the decompressed size is the same as compressed size
#     uLongf decompressed_size = decompressed_buffer.size();
#     int result = uncompress((Bytef*)decompressed_buffer.data(), &decompressed_size, (const Bytef*)buffer.data(), size);
#     if (result != Z_OK) {
#         std::cerr << "Failed to decompress data. Error code: " << result << std::endl;
#         return 1;
#     }
#
#     // Now you have the decompressed data in decompressed_buffer
#     // Process the decompressed data as needed
#
#     return 0;
# }