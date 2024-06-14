// Include the shell header file for necessary constants and function declarations
#include "shell.h"

// Function to read a command from the user input
void read_command(char **cmd)
{
  // Define a character array to store the command line input
  char line[MAX_LINE];
  // Initialize count to keep track of the number of characters read
  int count = 0, i = 0;
  // Array to hold pointers to the parsed command arguments
  char *array[MAX_ARGS], *command_token;

  // Infinite loop to read characters until a newline or maximum line length is reached
  for (;;)
  {
    // Read a single character from standard input
    int current_char = fgetc(stdin);
    // Store the character in the line array and increment count
    line[count++] = (char)current_char;
    // If a newline character is encountered, break out of the loop
    if (current_char == '\n')
      break;
    // If the command exceeds the maximum length, print an error and exit
    if (count >= MAX_LINE)
    {
      printf("Command is too long, unable to process\n");
      exit(1);
    }
  }
  // Null-terminate the command line string
  line[count] = '\0';

  // If only the newline character was entered, return without processing
  if (count == 1)
    return;

  // Use strtok to parse the first token (word) of the command
  command_token = strtok(line, " \n");

  // Continue parsing the line into words and store them in the array
  while (command_token != NULL)
  {
    array[i++] = strdup(command_token);  // Duplicate the token and store it
    command_token = strtok(NULL, " \n"); // Get the next token
  }

  // Copy the parsed command and its parameters to the cmd array
  for (int j = 0; j < i; j++)
  {
    cmd[j] = array[j];
  }
  // Null-terminate the cmd array to mark the end of arguments
  cmd[i] = NULL;
}

// Function to display the shell prompt
void type_prompt()
{
  // Use a static variable to check if this is the first call to the function
  static int first_time = 1;
  if (first_time)
  {
    // Clear the screen on the first call
#ifdef _WIN32
    system("cls"); // Windows command to clear screen
#else
    system("clear"); // UNIX/Linux command to clear screen
#endif
    first_time = 0;
  }
  fflush(stdout); // Flush the output buffer
  printf("$$ ");  // Print the shell prompt
}

// The main function where the shell's execution begins
int main(void)
{
  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS];
  int child_status;
  pid_t pid;

  type_prompt();     // Display the prompt
  read_command(cmd); // Read a command from the user

  // If the command is "exit", break out of the loop to terminate the shell
  if (strcmp(cmd[0], "exit") == 0)
    // break;
    return 0;

  // Formulate the full path of the command to be executed
  char full_path[PATH_MAX];
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
  {

    snprintf(full_path, sizeof(full_path), "%s/bin/%s", cwd, cmd[0]);
  }
  else
  {
    printf("Failed to get current working directory.");
    exit(1);
  }

  execv(full_path, cmd);

  // If execv returns, command execution has failed
  printf("Command %s not found\n", cmd[0]);
  exit(0);

  // Free the allocated memory for the command arguments before exiting
  for (int i = 0; cmd[i] != NULL; i++)
  {
    free(cmd[i]);
  }
  memset(cwd, '\0', sizeof(cwd)); // clear the cwd array

  return 0;
}
