#include <windows.h>
#include <stdio.h>

#define NUM_PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

HANDLE chopsticks[NUM_PHILOSOPHERS];

void philosopher(int number) {
    char chopstick_name[20];
    while (1) {
        // Thinking
        printf("Philosopher %d is thinking.\n", number);
        Sleep(1000);

        // Get left chopstick
        sprintf(chopstick_name, "Chopstick%d", number);
        WaitForSingleObject(chopsticks[number], INFINITE);
        printf("Philosopher %d picked up left chopstick %d.\n", number, number);

        // Get right chopstick
        sprintf(chopstick_name, "Chopstick%d", (number + 1) % NUM_PHILOSOPHERS);
        WaitForSingleObject(chopsticks[(number + 1) % NUM_PHILOSOPHERS], INFINITE);
        printf("Philosopher %d picked up right chopstick %d.\n", number, (number + 1) % NUM_PHILOSOPHERS);

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

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)philosopher, (LPVOID)i, 0, &philosopher_ids[i]);
    }

    // Wait for philosophers to finish
    WaitForMultipleObjects(NUM_PHILOSOPHERS, philosophers, TRUE, INFINITE);

    // Clean up
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        CloseHandle(chopsticks[i]);
        CloseHandle(philosophers[i]);
    }

    return 0;
}
