2)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void binarySearch(int arr[], int low, int high, int key) {
    while (low <= high) {
        int mid = (low + high) / 2;

        if (arr[mid] == key) {
            printf("Item %d found at index %d\n", key, mid);
            exit(0);
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    printf("Item %d not found in the array\n", key);
    exit(1);
}

int main() {
    int n;

    printf("Enter the size of the array: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter %d elements in the array:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process
        printf("Child process ID: %d\n", getpid());

        // Sorting the array
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    // Swap the elements
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }

        // Convert the sorted array to strings for execve
        char *sortedArray[n + 2];  // n elements + executable + NULL
        sortedArray[0] = "./binary_search";  // Assuming the binary_search executable is in the same directory
        for (int i = 0; i < n; i++) {
            sortedArray[i + 1] = malloc(12);  // Assuming a maximum of 12 digits for each number
            sprintf(sortedArray[i + 1], "%d", arr[i]);
        }
        sortedArray[n + 1] = NULL;

        execve("./binary_search", sortedArray, NULL);

        // If execve fails
        perror("Execve failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        printf("Parent process ID: %d\n", getpid());

        wait(NULL);  // Wait for the child to finish

        printf("Parent process exiting.\n");
    }

    return 0;
}


BINARY SEARCH.c


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int key = 42;  // Change this value to the key you want to search for

    printf("Sorted array received from parent process:\n");

    for (int i = 1; argv[i] != NULL; i++) {
        printf("%d ", atoi(argv[i]));
    }

    printf("\n");

    binarySearch(argv + 1, 0, argc - 2, key);

    return 0;
}
