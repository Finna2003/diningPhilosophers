#include <iostream>
#include <windows.h>
#include <process.h>

#define NUM_PHILOSOPHERS 7
HANDLE forks[NUM_PHILOSOPHERS];
enum PhilosopherState { THINKING, HUNGRY, EATING };
enum PhilosopherState philosopherStates[NUM_PHILOSOPHERS];

unsigned int __stdcall philosopher(void* arg) {
    int philosopher_id = *(int*)arg;
    srand(GetTickCount());

    while (true) {
        // Philosopher thinks for a while
        philosopherStates[philosopher_id] = THINKING;
        std::cout << "Philosopher " << philosopher_id << " is thinking" << std::endl;
        Sleep(rand() % 900 + 100);

        // Philosopher gets hungry and tries to pick up forks
        philosopherStates[philosopher_id] = HUNGRY;
        std::cout << "Philosopher " << philosopher_id << " is hungry" << std::endl;
        WaitForSingleObject(forks[philosopher_id], INFINITE);
        WaitForSingleObject(forks[(philosopher_id + 1) % NUM_PHILOSOPHERS], INFINITE);

        // Philosopher eats for a while
        philosopherStates[philosopher_id] = EATING;
        std::cout << "Philosopher " << philosopher_id << " is eating" << std::endl;
        Sleep(rand() % 900 + 100);

        // Philosopher is done eating, puts down forks and starts over
        std::cout << "Philosopher " << philosopher_id << " is done eating" << std::endl;
        ReleaseMutex(forks[philosopher_id]);
        ReleaseMutex(forks[(philosopher_id + 1) % NUM_PHILOSOPHERS]);
    }

    return 0;
}

int main() {
    HANDLE philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        philosopherStates[i] = THINKING;
        forks[i] = CreateMutex(NULL, FALSE, NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i] = (HANDLE)_beginthreadex(NULL, 0, philosopher, &philosopher_ids[i], 0, NULL);
    }

    while (true) {
        // Print the current state of each philosopher
        std::cout << "Current philosophers states: ";
        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            if (philosopherStates[i] == THINKING) {
                std::cout << "T ";
            }
            else if (philosopherStates[i] == HUNGRY) {
                std::cout << "H ";
            }
            else {
                std::cout << "E ";
            }
        }
        std::cout << std::endl;

        // Wait for a while before printing the states again
        Sleep(1000);
    }

    return 0;
}
