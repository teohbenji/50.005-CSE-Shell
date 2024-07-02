#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1000

int main() {
    char command[MAX_COMMAND_LENGTH];
    int daemon_count = 0;

    // Construct the command to count dspawn daemon processes
    snprintf(command, sizeof(command), "ps -efj | grep dspawn | grep -Ev 'tty|pts' | wc -l");

    // Execute the command and read its output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to execute command");
        return 1;
    }

    // Read the output of the command
    if (fgets(command, sizeof(command), fp) != NULL) {
        // Convert output to integer
        daemon_count = atoi(command);
        printf("Number of live daemon processes: %d\n", daemon_count);
    } else {
        printf("Error reading command output.\n");
    }

    // Close the pipe
    pclose(fp);

    return 0;
}
