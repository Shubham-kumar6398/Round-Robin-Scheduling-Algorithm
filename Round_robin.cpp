#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int id;            // Process ID
    int burstTime;     // Burst time of the process
    int arrivalTime;   // Arrival time of the process
    int remainingTime; // Remaining burst time
};

void roundRobinScheduling(vector<Process>& processes, int quantum) {
    queue<int> processQueue;
    vector<int> waitingTime(processes.size(), 0);
    vector<int> turnaroundTime(processes.size(), 0);

    int currentTime = 0;
    int n = processes.size();
    int completed = 0;
    vector<bool> isInQueue(n, false);  // Track if a process is already in the queue

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    while (completed < n) {
        if (processQueue.empty()) {
            // If no process is ready, move to the next process arrival time
            currentTime = processes[completed].arrivalTime;
            processQueue.push(completed);
            isInQueue[completed] = true;
        }

        int currentProcessIndex = processQueue.front();
        Process &currentProcess = processes[currentProcessIndex];
        processQueue.pop();

        // Process executes for either its remaining time or the quantum, whichever is smaller
        int timeSlice = min(quantum, currentProcess.remainingTime);
        currentTime += timeSlice;
        currentProcess.remainingTime -= timeSlice;

        // Enqueue any process that has arrived by the current time
        for (int i = 0; i < n; i++) {
            if (!isInQueue[i] && processes[i].arrivalTime <= currentTime) {
                processQueue.push(i);
                isInQueue[i] = true;
            }
        }

        // If the process has remaining time, requeue it
        if (currentProcess.remainingTime > 0) {
            processQueue.push(currentProcessIndex);
        } else {
            // Process is finished
            turnaroundTime[currentProcessIndex] = currentTime - currentProcess.arrivalTime;
            waitingTime[currentProcessIndex] = turnaroundTime[currentProcessIndex] - currentProcess.burstTime;
            completed++;
        }
    }

    // Print results
    cout << "Process ID | Arrival Time | Burst Time | Waiting Time | Turnaround Time\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << processes[i].id << "\t\t"
             << processes[i].arrivalTime << "\t\t"
             << processes[i].burstTime << "\t\t"
             << waitingTime[i] << "\t\t"
             << turnaroundTime[i] << "\n";
    }

    // Calculate average waiting time and turnaround time
    double totalWaitingTime = 0, totalTurnaroundTime = 0;
    for (int i = 0; i < n; i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnaroundTime += turnaroundTime[i];
    }

    cout << "Average Waiting Time: " << totalWaitingTime / n << "\n";
    cout << "Average Turnaround Time: " << totalTurnaroundTime / n << "\n";
}

int main() {
    int n, quantum;
    cout << "Enter the number of processes: ";
    cin >> n;
    cout << "Enter the time quantum: ";
    cin >> quantum;

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter the arrival time and burst time for process P" << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime;  // Initialize remaining time
    }

    roundRobinScheduling(processes, quantum);

    return 0;
}
