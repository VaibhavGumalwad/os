6)ipc using pipes

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define BUFFER_SIZE 256

int main() {
    int pipe_parent_to_child[2];
    int pipe_child_to_parent[2];

    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process

        close(pipe_parent_to_child[0]); // Close reading end of parent-to-child pipe
        close(pipe_child_to_parent[1]); // Close writing end of child-to-parent pipe

        char filename[BUFFER_SIZE];
        printf("Enter the pathname of the file: ");
        fgets(filename, BUFFER_SIZE, stdin);

        // Write the filename to the pipe
        write(pipe_parent_to_child[1], filename, strlen(filename) + 1);

        close(pipe_parent_to_child[1]); // Close writing end after writing

        char file_content[BUFFER_SIZE];

        // Read the file contents from the pipe
        read(pipe_child_to_parent[0], file_content, BUFFER_SIZE);

        printf("Contents of the file received from child process:\n%s", file_content);

        close(pipe_child_to_parent[0]); // Close reading end of child-to-parent pipe
    } else {
        // Child process

        close(pipe_parent_to_child[1]); // Close writing end of parent-to-child pipe
        close(pipe_child_to_parent[0]); // Close reading end of child-to-parent pipe

        char filename[BUFFER_SIZE];

        // Read the filename from the pipe
        read(pipe_parent_to_child[0], filename, BUFFER_SIZE);

        FILE *file = fopen(filename, "r");

        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        char file_content[BUFFER_SIZE];

        // Read the contents of the file
        size_t bytesRead = fread(file_content, 1, BUFFER_SIZE, file);

        // Write the file contents to the pipe
        write(pipe_child_to_parent[1], file_content, bytesRead);

        fclose(file);
        close(pipe_child_to_parent[1]); // Close writing end after writing
        close(pipe_parent_to_child[0]); // Close reading end of parent-to-child pipe
    }

    return 0;
}

===============================ooooooooooooooooooooorrrrrrrrrrrrrrrrrrrr==========================

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
FILE *fp;
int fd1[2],fd2[2],i=0;
char ch1,ch2,str1[100],str2[100],path[100]="/home/mml/Pradnya73/pipe.txt";
int ret1,ret2;
pid_t pid;
ret1=pipe(fd1);
ret2=pipe(fd2);
if(ret1==-1 || ret2==-1)
printf("\nERROR\n");
pid=fork();
if(pid==0)
{
read(fd1[0],str2,100);
fp=fopen(str2,"r");
while(!feof(fp))
{
ch2=fgetc(fp);
write(fd2[1],&ch2,1);
}
}
else
{
write(fd1[1],path,strlen(path)+1);
while(read(fd2[0],&ch1,1)>0)
printf("%c",ch1);
}
}
pipe.txt
Hello World
Output:
mml@mml-Vostro-3470:~/Pradnya73$ gcc -o ipcp.out IPCPipe.c
mml@mml-Vostro-3470:~/Pradnya73$ ./ipcp.out
Hello World
