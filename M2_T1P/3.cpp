#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

const int N = 15; // matrix size
const int THREADS = 2; // number of threads

int A[N][N];
int B[N][N];
int C[N][N];
int thread_count = 0;

pthread_mutex_t mutex;

void* multiply(void* arg) {
    int thread_num = thread_count++;
    for (int i = thread_num * N / THREADS; i < (thread_num + 1) * N / THREADS; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            pthread_mutex_lock(&mutex);
            C[i][j] += sum;
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);

    // initialize matrices A and B with random values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    // calculate matrix C = A * B
    auto start = high_resolution_clock::now();

    pthread_t threads[THREADS];
    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, multiply, NULL);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // write matrix C to file
    ofstream outfile("matrix_multiplication1.txt");
    if (outfile.is_open()) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                outfile << C[i][j] << " ";
            }
            outfile << endl;
        }
        outfile.close();
    }
    else {
        cout << "Unable to open file" << endl;
    }

    pthread_mutex_destroy(&mutex);

    // print execution time
    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}
