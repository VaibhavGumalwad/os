7) server IPC USING SHARED MEMORY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    // Create or get the shared memory segment
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Write a message to the shared memory
    const char *message = "Hello from the server!";
    strncpy(shm_ptr, message, SHM_SIZE);

    printf("Server: Message written to shared memory: %s\n", message);

    // Detach from the shared memory segment
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}
 
7)client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    // Get the shared memory segment
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Read the message from the shared memory
    printf("Client: Message read from shared memory: %s\n", shm_ptr);

    // Detach from the shared memory segment
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Optionally, remove the shared memory segment (uncomment the line below)
    // if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    //     perror("shmctl");
    //     exit(EXIT_FAILURE);
    // }

    return 0;
}


=====================OOOOOOOOOOOOOOOOOOOOORRRRRRRRRRRRRRRRRRRRR========================

SERVER.C
#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<sys/shm.h>  
#include<string.h>  
int main()  
{  
int i;  
void *shared_memory;  
char buff[100];  
int shmid;  
shmid=shmget((key_t)2345, 1024, 0666|IPC_CREAT);   
//creates shared memory segment with key 2345, having size 1024 bytes. IPC_CREAT is used to create the shared segment if it does not exist. 0666 are the permissions on the shared segment  
printf("Key of shared memory is %d\n",shmid);  
shared_memory=shmat(shmid,NULL,0);   
//process attached to shared memory segment  
printf("Process attached at %p\n",shared_memory);   
//this prints the address where the segment is attached with this process  
printf("Enter some data to write to shared memory\n");  
read(0,buff,100); //get some input from user  
strcpy(shared_memory,buff); //data written to shared memory  
printf("You wrote : %s\n",(char *)shared_memory);  
}


CLIENT.C

#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<sys/shm.h>  
#include<string.h>  
int main()  
{  
int i;  
void *shared_memory;  
char buff[100];  
int shmid;  
shmid=shmget((key_t)2345, 1024, 0666);  
printf("Key of shared memory is %d\n",shmid);  
shared_memory=shmat(shmid,NULL,0); //process attached to shared memory segment  
printf("Process attached at %p\n",shared_memory);  
printf("Data read from shared memory is : %s\n",(char *)shared_memory);  
} 
