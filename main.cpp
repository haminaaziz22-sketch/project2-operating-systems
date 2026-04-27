#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

// This function simulates one process.
// Each process runs as its own thread.
void processThread(int pid, int burstTime) {
    cout << "[Process " << pid << "] Started. Burst time = "
         << burstTime << " seconds." << endl;

    // Simulates CPU burst time
    this_thread::sleep_for(chrono::seconds(burstTime));

    cout << "[Process " << pid << "] Finished." << endl;
}

// This function simulates one philosopher.
// Each philosopher is a thread.
void philosopher(int id, vector<mutex>& forks) {
    int leftFork = id;
    int rightFork = (id + 1) % 5;

    for (int cycle = 1; cycle <= 3; cycle++) {
        cout << "[Philosopher " << id << "] Thinking... Cycle "
             << cycle << endl;

        this_thread::sleep_for(chrono::milliseconds(500 + rand() % 1000));

        cout << "[Philosopher " << id << "] Waiting for forks..." << endl;

        // Deadlock prevention:
        // Always pick up the lower-numbered fork first.
        int firstFork = min(leftFork, rightFork);
        int secondFork = max(leftFork, rightFork);

        forks[firstFork].lock();
        cout << "[Philosopher " << id << "] Picked up fork "
             << firstFork << endl;

        forks[secondFork].lock();
        cout << "[Philosopher " << id << "] Picked up fork "
             << secondFork << endl;

        cout << "[Philosopher " << id << "] Eating..." << endl;
        this_thread::sleep_for(chrono::milliseconds(500 + rand() % 1000));

        forks[secondFork].unlock();
        cout << "[Philosopher " << id << "] Released fork "
             << secondFork << endl;

        forks[firstFork].unlock();
        cout << "[Philosopher " << id << "] Released fork "
             << firstFork << endl;
    }

    cout << "[Philosopher " << id << "] Finished all cycles." << endl;
}

int main() {
    srand(time(nullptr));

    cout << "===== PART 1: PROCESS SIMULATION =====" << endl;

    ifstream inputFile("processes.txt");

    if (!inputFile) {
        cout << "Error: Could not open processes.txt" << endl;
        return 1;
    }

    vector<thread> processThreads;
    int pid, burstTime;

    // Read each process from the input file.
    while (inputFile >> pid >> burstTime) {
        processThreads.push_back(thread(processThread, pid, burstTime));
    }

    inputFile.close();

    // Wait for all process threads to finish.
    for (auto& t : processThreads) {
        t.join();
    }

    cout << "All process threads completed." << endl;

    cout << endl;
    cout << "===== PART 2: DINING PHILOSOPHERS =====" << endl;

    vector<mutex> forks(5);
    vector<thread> philosophers;

    // Create 5 philosopher threads.
    for (int i = 0; i < 5; i++) {
        philosophers.push_back(thread(philosopher, i, ref(forks)));
    }

    // Wait for all philosopher threads to finish.
    for (auto& t : philosophers) {
        t.join();
    }

    cout << "Dining Philosophers simulation completed." << endl;

    return 0;
}
