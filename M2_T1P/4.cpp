#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

const int N = 100; // matrix size
const int NUM_THREADS = 8; // number of threads to use

int A[N][N];
int B[N][N];
int C[N][N];

// struct to pass parameters to thread function
struct thread_data {
    int thread_id;
    int num_threads;
};

// function for each thread to execute
void *matrix_multiply(void *arg) {
    thread_data *data = (thread_data *)arg;
    int tid = data->thread_id;
    int num_threads = data->num_threads;

    // calculate range of rows for this thread to work on
    int row_start = tid * N / num_threads;
    int row_end = (tid + 1) * N / num_threads;

    for (int i = row_start; i < row_end; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    pthread_exit(NULL);
}

int main()
{
    srand(time(NULL));

    // initialize matrices A and B with random values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    // calculate matrix C = A * B using multiple threads
    pthread_t threads[NUM_THREADS];
    thread_data data[NUM_THREADS];

    auto start = high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; i++) {
        data[i].thread_id = i;
        data[i].num_threads = NUM_THREADS;
        int rc = pthread_create(&threads[i], NULL, matrix_multiply, (void *)&data[i]);
        if (rc) {
            cout << "Error: unable to create thread, " << rc << endl;
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // write matrix C to file
    ofstream outfile("matrix_multiplication2.txt");
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

    // print execution time
    cout << "Execution time with " << NUM_THREADS << " threads: " << duration.count() << " microseconds" << endl;

    return 0;
}
