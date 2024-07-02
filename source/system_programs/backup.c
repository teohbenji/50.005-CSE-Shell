#include "system_program.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/*
The backup system program must be able to automatically zip a directory whose name matches environment variable BACKUP_DIR,
and move this zipped directory to [PROJECT_DIR]/archive/. 
BACKUP_DIR must be set up by the shell before calling backup. 
The filename of the zipped file must include the correct datetime indicating when the backup command was executed.
*/

#define PROJECT_DIR "./archive" // Change this to your actual project directory

// Function to get the current date and time in Year, Month, Date, Hour, Min, Second
void get_current_datetime(char *buffer, int size) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, size, "%04d%02d%02d%02d%02d%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
}

// Function to execute the backup process
int execute(char **args) {
    // Retrieve the backup directory from the environment variable
    char *backup_dir = getenv("BACKUP_DIR");
    if (backup_dir == NULL) {
        fprintf(stderr, "Error: BACKUP_DIR environment variable is not set.\n");
        return 1;
    }
    // Get the current date and time for the backup filename
    char datetime[20];
    get_current_datetime(datetime, sizeof(datetime));
    // Create the zip filename with the current datetime
    char zip_filename[256];
    snprintf(zip_filename, sizeof(zip_filename), "backup-%s.zip", datetime);

    // Create the full path for the archive file
    char archive_path[512];
    int archive_path_len = snprintf(archive_path, sizeof(archive_path), "%s/%s", PROJECT_DIR, zip_filename);
    if (archive_path_len >= sizeof(archive_path)) {
        fprintf(stderr, "Archive path buffer too small.\n");
        return 1;
    }

    // Print the backup process initiation message
    printf("Creating backup of '%s' at '%s'\n", backup_dir, archive_path);

    // Create the zip command to compress the backup directory
    char zip_command[512];
    int zip_command_len = snprintf(zip_command, sizeof(zip_command), "zip -r %s %s", zip_filename, backup_dir);
    if (zip_command_len >= sizeof(zip_command)) {
        fprintf(stderr, "Zip command buffer too small.\n");
        return 1;
    }

    // Execute the zip command and print its output
    FILE *fp = popen(zip_command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to run zip command.\n");
        return 1;
    }

    // Read the output of the zip command
    char output[1035];
    while (fgets(output, sizeof(output) - 1, fp) != NULL) {
        printf("%s", output);
    }

    // Check the result of the zip command execution
    int result = pclose(fp);
    if (result != 0) {
        fprintf(stderr, "Failed to zip directory %s.\n", backup_dir);
        return 1;
    }

    // Create the move command to move the zipped file to the archive directory
    char move_command[512];
    int move_command_len = snprintf(move_command, sizeof(move_command), "mv %s %s", zip_filename, archive_path);
    if (move_command_len >= sizeof(move_command)) {
        fprintf(stderr, "Move command buffer too small.\n");
        return 1;
    }

    // Execute the move command to move the zipped file
    result = system(move_command);
    if (result != 0) {
        fprintf(stderr, "Failed to move zipped file to archive.\n");
        return 1;
    }

    printf("Backup created successfully.\n");
    return 0;
}

// Main function to call the execute function
int main(int argc, char **args) {
    return execute(args);
}