// Final Version - Reviewed by Aryan & Vanshika
#include <iostream>
#include <vector>

using namespace std;

struct SystemState {
    int numProcesses;
    int numResources;
    vector<int> available;
    vector<vector<int>> maxMatrix;
    vector<vector<int>> allocationMatrix;
    vector<vector<int>> needMatrix;
};

void inputSystemState(SystemState& state) {
    cout << "Enter number of processes: ";
    cin >> state.numProcesses;
    cout << "Enter number of resources: ";
    cin >> state.numResources;

    state.available.resize(state.numResources);
    state.maxMatrix.resize(state.numProcesses, vector<int>(state.numResources));
    state.allocationMatrix.resize(state.numProcesses, vector<int>(state.numResources));
    state.needMatrix.resize(state.numProcesses, vector<int>(state.numResources));

    cout << "\nEnter Available Resources (space-separated):" << endl;
    for (int i = 0; i < state.numResources; i++) {
        cin >> state.available[i];
    }

    cout << "\nEnter Allocation Matrix (row by row):" << endl;
    for (int i = 0; i < state.numProcesses; i++) {
        for (int j = 0; j < state.numResources; j++) {
            cin >> state.allocationMatrix[i][j];
        }
    }

    cout << "\nEnter Max Matrix (row by row):" << endl;
    for (int i = 0; i < state.numProcesses; i++) {
        for (int j = 0; j < state.numResources; j++) {
            cin >> state.maxMatrix[i][j];
            state.needMatrix[i][j] = state.maxMatrix[i][j] - state.allocationMatrix[i][j];
        }
    }
}

bool detectDeadlock(const SystemState& state, vector<int>& deadlockedProcesses) {
    int n = state.numProcesses;
    int m = state.numResources;
    vector<int> work = state.available;
    vector<bool> finish(n, false);

    for(int i = 0; i < n; i++) {
        bool hasResources = false;
        for(int j = 0; j < m; j++) {
            if(state.allocationMatrix[i][j] != 0) {
                hasResources = true; break;
            }
        }
        if(!hasResources) finish[i] = true;
    }

    bool progress = true;
    while(progress) {
        progress = false;
        for(int i = 0; i < n; i++) {
            if(!finish[i]) {
                bool canProceed = true;
                for(int j = 0; j < m; j++) {
                    if(state.needMatrix[i][j] > work[j]) {
                        canProceed = false; break;
                    }
                }
                if(canProceed) {
                    for(int j = 0; j < m; j++) work[j] += state.allocationMatrix[i][j];
                    finish[i] = true;
                    progress = true; 
                }
            }
        }
    }

    bool isDeadlocked = false;
    for(int i = 0; i < n; i++) {
        if(!finish[i]) {
            isDeadlocked = true;
            deadlockedProcesses.push_back(i);
        }
    }
    return isDeadlocked;
}

void suggestRecovery(const SystemState& state, const vector<int>& deadlockedProcesses) {
    cout << "\n--- Deadlock Recovery Subsystem ---" << endl;
    cout << "Deadlocked Processes: ";
    for(int p : deadlockedProcesses) cout << "P" << p << " ";
    cout << endl;

    int victim = deadlockedProcesses[0];
    int maxResources = 0;
    
    for(int p : deadlockedProcesses) {
        int totalAllocated = 0;
        for(int j = 0; j < state.numResources; j++) {
            totalAllocated += state.allocationMatrix[p][j];
        }
        if(totalAllocated > maxResources) {
            maxResources = totalAllocated;
            victim = p;
        }
    }

    cout << "[SUGGESTION] Terminate Process P" << victim << " to break the circular wait." << endl;
    cout << "Reason: P" << victim << " holds the most resources (" << maxResources << " total units)." << endl;
    cout << "Freeing these will likely allow other processes to complete." << endl;
}

int main() {
    int choice;
    do {
        cout << "\n=========================================" << endl;
        cout << "   Automated Deadlock Detection Tool" << endl;
        cout << "=========================================" << endl;
        
        SystemState state;
        inputSystemState(state);
        
        cout << "\n[System] Running Banker's Algorithm based Detection..." << endl;
        
        vector<int> deadlockedProcesses;
        bool hasDeadlock = detectDeadlock(state, deadlockedProcesses);
        
        if(hasDeadlock) {
            cout << "\n[X] ALERT: Deadlock Detected! System is in an unsafe state." << endl;
            suggestRecovery(state, deadlockedProcesses);
        } else {
            cout << "\n[+] SUCCESS: System is safe. No deadlock detected." << endl;
        }
        
        cout << "\nDo you want to test another system state? (1 for Yes, 0 for No): ";
        cin >> choice;
        
    } while(choice != 0);
    
    cout << "Exiting tool. Goodbye!" << endl;
    return 0;
}