#include <iostream>
#include <vector>
#include <chrono>
#include <string>
using namespace std;

int main() {
    const int DATA_NUM = 100000;
    
    // Start the clock
    auto start_time = chrono::high_resolution_clock::now();
    
    //vector<int> data;
    //data.resize(a);
    int* data = (int*)malloc(DATA_NUM * sizeof(int));

    int smallest,largest,total;
    float avg_val;
    smallest = 9999; largest = 0;total =0;
    for(int i=0; i<DATA_NUM;i++ ){
    data[i] = rand() % 10000;
    if (data[i] <smallest){
    smallest = data[i];}
    if (data[i] > largest){
    largest = data[i];
    }
    total += data[i];
    }
    avg_val = float(total)/float(DATA_NUM);
    
    auto end_time = chrono::high_resolution_clock::now();
        auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout << "Minimum value: " << smallest << endl;
        cout << "Maximum value: " << largest << endl;
        cout << "Average value: " << avg_val << endl;
        cout << "Elapsed time: " << elapsed_time << " milliseconds" << endl;
    //data.clear();
    free(data);

    }
