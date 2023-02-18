#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <mpi.h>
#include <chrono>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int DATA_SIZE = 100000;
    vector<int> data;

    if (world_rank == 0) {
        // Generate random data for root process
        data.resize(DATA_SIZE);
        for (int i = 0; i < DATA_SIZE; i++) {
            data[i] = rand() % 10000;
        }
    }

    // Start the clock
    auto start_time = chrono::high_resolution_clock::now();

    // Scatter data to all processes
    vector<int> local_data(DATA_SIZE / world_size);
    MPI_Scatter(data.data(), DATA_SIZE / world_size, MPI_INT, local_data.data(), DATA_SIZE / world_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Find the minimum, maximum, and sum of the local data
    int min_local = *min_element(local_data.begin(), local_data.end());
    int max_local = *max_element(local_data.begin(), local_data.end());
    int sum_local = accumulate(local_data.begin(), local_data.end(), 0);

    // Gather all minimum, maximum, and sum values to the root process
    vector<int> min_values(world_size);
    vector<int> max_values(world_size);
    vector<int> sum_values(world_size);
    MPI_Gather(&min_local, 1, MPI_INT, min_values.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&max_local, 1, MPI_INT, max_values.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&sum_local, 1, MPI_INT, sum_values.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        // Find the overall minimum, maximum, and average values
        int min_val = *min_element(min_values.begin(), min_values.end());
        int max_val = *max_element(max_values.begin(), max_values.end());
        int sum_val = accumulate(sum_values.begin(), sum_values.end(), 0);
        double avg_val = static_cast<double>(sum_val) / DATA_SIZE;

        // End the clock and calculate the elapsed time
        auto end_time = chrono::high_resolution_clock::now();
        auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

        // Output the results and the elapsed time
        cout << "Minimum value: " << min_val << endl;
        cout << "Maximum value: " << max_val << endl;
        cout << "Average value: " << avg_val << endl;
        cout << "Elapsed time: " << elapsed_time << " milliseconds" << endl;
    }

    MPI_Finalize();
}
