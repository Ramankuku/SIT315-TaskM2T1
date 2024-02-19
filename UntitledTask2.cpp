#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <thread>

using namespace std;

// Function to generate a random matrix of size N x N
void generateRandomMatrix(int** matrix, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; N < N; ++j) {
            matrix[i][j] = rand() % 100;  // Assign random values between 0 and 99
        }
    }
}

// Function to perform matrix multiplication C = A * B for a specific range of rows
void matrixMultiplyRange(int** A, int** B, int** C, int startRow, int endRow, int N) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to perform matrix multiplication C = A * B using 7 threads
void matrixMultiplyParallel(int** A, int** B, int** C, int N) {
    vector<thread> threads;
    int rowsPerThread = N / 5;

    for (int i = 0; i < 5; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == 4) ? N : startRow + rowsPerThread;
        threads.emplace_back(matrixMultiplyRange, A, B, C, startRow, endRow, N);
    }

    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    int N;
    cout << "Enter the size of the matrices (N): ";
    cin >> N;

    // Dynamically allocate memory for matrices A, B, and C
    int** A = new int*[N];
    int** B = new int*[N];
    int** C = new int*[N];
    for (int i = 0; i < N; ++i) {
        A[i] = new int[N];
        B[i] = new int[N];
        C[i] = new int[N];
    }

    // Seed for random number generation
    srand(static_cast<unsigned>(time(0)));

    // Generate random matrices A and B
    generateRandomMatrix(A, N);
    generateRandomMatrix(B, N);

    // Perform matrix multiplication with 7 threads and measure time
    clock_t start = clock();
    matrixMultiplyParallel(A, B, C, N);
    clock_t end = clock();

    // Write the result to a file
    ofstream outputFile("output_matrix_parallel_7threads.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                outputFile << C[i][j] << " ";
            }
            outputFile << endl;
        }
        outputFile.close();
    } else {
        cout << "Unable to open the output file." << endl;
    }

    // Output the time taken
    cout << "Matrix multiplication with 7 threads completed in "
         << (double)(end - start) / CLOCKS_PER_SEC << " seconds." << endl;

    // Free allocated memory
    for (int i = 0; i < N; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
