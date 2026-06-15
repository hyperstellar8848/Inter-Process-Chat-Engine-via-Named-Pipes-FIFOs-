#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

int r_fd, w_fd;
char username[50];
int is_first_user = 0;

// Thread 1: Read from keyboard and write to output FIFO (Send)
void *send_func(void *arg) {
    char buf[1024];
    char msg[1100];

    while (1) {
        if (fgets(buf, sizeof(buf), stdin) == NULL) break;
        buf[strcspn(buf, "\n")] = '\0'; // Remove newline character

        if (strcmp(buf, "quit") == 0) {
            write(w_fd, "quit", 5);
            break;
        }

        // Format message: [username] message
        snprintf(msg, sizeof(msg), "[%s] %s", username, buf);
        write(w_fd, msg, strlen(msg) + 1);
    }
    exit(0); // Terminate the whole program on quit
}

// Thread 2: Read from input FIFO and print to screen (Receive)
void *recv_func(void *arg) {
    char buf[1100];

    while (1) {
        int bytes_read = read(r_fd, buf, sizeof(buf));
        if (bytes_read <= 0) break;

        // If peer sent quit
        if (strcmp(buf, "quit") == 0) {
            printf("\nPartner left the chat. Type 'quit' to exit.\n");
            break;
        }
        
        // Display message and flush stdout
        printf("%s\n", buf);
        fflush(stdout);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <username>\n", argv[0]);
        return 1;
    }
    strncpy(username, argv[1], sizeof(username) - 1);

    // 1. Create FIFOs. If FIFO1 is successfully created, this is User 1
    if (mkfifo(FIFO1, 0666) == 0) {
        is_first_user = 1; 
    }
    mkfifo(FIFO2, 0666);

    printf("Welcome %s! Connecting...\n", username);

    // 2. Open FIFOs in structural order to break the system blocking lock
    if (is_first_user) {
        // User 1: Reads from fifo1, Writes to fifo2
        w_fd = open(FIFO2, O_WRONLY);
        r_fd = open(FIFO1, O_RDONLY);
    } else {
        // User 2: Reads from fifo2, Writes to fifo1
        r_fd = open(FIFO2, O_RDONLY);
        w_fd = open(FIFO1, O_WRONLY);
    }

    if (r_fd < 0 || w_fd < 0) {
        perror("Open failed");
        return 1;
    }

    printf("Connected! Start chatting.\n");

    // 3. Create threads for asynchronous operation
    pthread_t t1, t2;
    pthread_create(&t1, NULL, send_func, NULL);
    pthread_create(&t2, NULL, recv_func, NULL);

    // 4. Wait for threads to finish (pthread_join)
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // 5. Clean up and unlink files from system disk
    close(r_fd);
    close(w_fd);
    if (is_first_user) {
        unlink(FIFO1);
        unlink(FIFO2);
    }

    return 0;
}
