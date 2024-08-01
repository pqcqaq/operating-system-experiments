#include <windows.h>
#include <stdio.h>

#define NUM_PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

HANDLE chopsticks[NUM_PHILOSOPHERS];

void philosopher(int number, int direction) {
    char chopstick_name[20];
    while (1) {
        // Thinking
        printf("Philosopher %d is thinking.\n", number);
        Sleep(1000);

        if (direction == 0) {
            // Get left chopstick
            sprintf(chopstick_name, "Chopstick%d", number);
            WaitForSingleObject(chopsticks[number], INFINITE);
            printf("Philosopher %d picked up left chopstick %d.\n", number, number);

            // Get right chopstick
            sprintf(chopstick_name, "Chopstick%d", (number + 1) % NUM_PHILOSOPHERS);
            WaitForSingleObject(chopsticks[(number + 1) % NUM_PHILOSOPHERS], INFINITE);
            printf("Philosopher %d picked up right chopstick %d.\n", number, (number + 1) % NUM_PHILOSOPHERS);
        } else {
            // Get right chopstick
            sprintf(chopstick_name, "Chopstick%d", (number + 1) % NUM_PHILOSOPHERS);
            WaitForSingleObject(chopsticks[(number + 1) % NUM_PHILOSOPHERS], INFINITE);
            printf("Philosopher %d picked up right chopstick %d.\n", number, (number) % NUM_PHILOSOPHERS);
                        // Get right chopstick
            sprintf(chopstick_name, "Chopstick%d", (number + 1) % NUM_PHILOSOPHERS);
            WaitForSingleObject(chopsticks[(number + 1) % NUM_PHILOSOPHERS], INFINITE);
            printf("Philosopher %d picked up right chopstick %d.\n", number, (number + 1) % NUM_PHILOSOPHERS);

            // Get left chopstick
            sprintf(chopstick_name, "Chopstick%d", number);
            WaitForSingleObject(chopsticks[number], INFINITE);
            printf("Philosopher %d picked up left chopstick %d.\n", number, number);
        }

        // Eating
        printf("Philosopher %d is eating.\n", number);
        Sleep(1000);

        // Release chopsticks
        ReleaseSemaphore(chopsticks[number], 1, NULL);
        ReleaseSemaphore(chopsticks[(number + 1) % NUM_PHILOSOPHERS], 1, NULL);
        printf("Philosopher %d released chopsticks.\n", number);
    }
}

int main() {
    HANDLE philosophers[NUM_PHILOSOPHERS];
    DWORD philosopher_ids[NUM_PHILOSOPHERS];

    // Create chopstick semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        char chopstick_name[20];
        sprintf(chopstick_name, "Chopstick%d", i);
        chopsticks[i] = CreateSemaphore(NULL, 1, 1, chopstick_name);
    }

    // Create philosopher processes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        char command[100];
        int direction = (i == NUM_PHILOSOPHERS - 1) ? 1 : 0;
        sprintf(command, "philosopher.exe %d %d", i, direction);
        
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            printf("CreateProcess failed (%d).\n", GetLastError());
            return 1;
        }

        philosophers[i] = pi.hProcess;
        philosopher_ids[i] = pi.dwProcessId;
    }

    // Let philosophers run for a while
    Sleep(10000);  // Run for 10 seconds

    // Terminate philosopher processes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if (TerminateProcess(philosophers[i], 0)) {
            printf("Philosopher %d terminated.\n", i);
        } else {
            printf("Failed to terminate philosopher %d.\n", i);
        }
    }

    // Wait for all processes to finish
    WaitForMultipleObjects(NUM_PHILOSOPHERS, philosophers, TRUE, INFINITE);

    // Clean up
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        CloseHandle(chopsticks[i]);
        CloseHandle(philosophers[i]);
    }

    return 0;
}
