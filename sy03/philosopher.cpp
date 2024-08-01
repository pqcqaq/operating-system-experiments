#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_PHILOSOPHERS 5

void philosopher(int number, int direction) {
    HANDLE chopsticks[NUM_PHILOSOPHERS];
    char chopstick_name[20];

    // Open existing semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sprintf(chopstick_name, "Chopstick%d", i);
        chopsticks[i] = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, chopstick_name);
        if (chopsticks[i] == NULL) {
            printf("Failed to open semaphore %s\n", chopstick_name);
            return;
        }
    }

    while (1) {
        // Thinking
        printf("Philosopher %d is thinking.\n", number);
        Sleep(1000);

        if (direction == 0) {
            // Get left chopstick
            WaitForSingleObject(chopsticks[number], INFINITE);
            printf("Philosopher %d picked up left chopstick %d.\n", number, number);

            // Get right chopstick
            WaitForSingleObject(chopsticks[(number + 1) % NUM_PHILOSOPHERS], INFINITE);
            printf("Philosopher %d picked up right chopstick %d.\n", number, (number + 1) % NUM_PHILOSOPHERS);
        } else {
            // Get right chopstick
            WaitForSingleObject(chopsticks[(number + 1) % NUM_PHILOSOPHERS], INFINITE);
            printf("Philosopher %d picked up right chopstick %d.\n", number, (number + 1) % NUM_PHILOSOPHERS);

            // Get left chopstick
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

    // Close semaphore handles
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        CloseHandle(chopsticks[i]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: philosopher.exe <philosopher_number> <direction>\n");
        return 1;
    }

    int number = atoi(argv[1]);
    int direction = atoi(argv[2]);

    if (number < 0 || number >= NUM_PHILOSOPHERS) {
        printf("Invalid philosopher number. Must be between 0 and %d\n", NUM_PHILOSOPHERS - 1);
        return 1;
    }

    if (direction != 0 && direction != 1) {
        printf("Invalid direction. Must be 0 or 1\n");
        return 1;
    }

    philosopher(number, direction);

    return 0;
}
