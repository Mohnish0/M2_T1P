#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 2; // matrix size

int main()
{
    srand(time(NULL));

    // initialize matrices A and B with random values
    int A[N][N];
    int B[N][N];
    int C[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    // calculate matrix C = A * B
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    // write matrix C to file
    ofstream outfile("output1.txt");
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

    return 0;
}
