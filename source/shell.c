// Include the shell header file for necessary constants and function declarations
#include "shell.h"

extern char **environ; //Retrieve environment vars

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
  //printf("$$ ");  // Print the shell prompt

  char cwd[1024]; // Buffer to hold the current working directory
  // Get the current working directory and print the shell prompt
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("$$ %s $$ ", cwd);
  } else {
      // If getting the current working directory fails, print an error and fallback prompt
      perror("getcwd");
      printf("$$ ");
  }
}

// Helper function to figure out how many builtin commands are supported by the shell
int num_builtin_functions()
{
    return sizeof(builtin_commands) / sizeof(char *);
};


int shell_cd(char **args){
    if (args[1] == NULL) {
        fprintf(stderr, "cd: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("CSEShell");
        }
    }
  return 0;
}

int shell_help(char **args){
  printf("CSEShell Interface\n");
  printf("Usage: command arguments\n");
  printf("The following commands are implemented within the shell:\n");

  // Print each command from builtin_commands array
  for (int i = 0; i < num_builtin_functions(); i++) {
      printf("  %s\n", builtin_commands[i]);
  }
  return 0;

}
int shell_exit(char **args){
    // If the command is "exit", return 1
    return 1;
}
int shell_usage(char **args){
  if (args[1] == NULL){
    printf("Command not given. Type usage <command>.\n");
  }
  else if (strcmp(args[1], "cd") == 0){
    printf("Type: cd directory_name to change the current working directory of the shell\n");
  }
  else if (strcmp(args[1], "help") == 0){
    printf("Type: help for supported commands\n");
  }
  else if (strcmp(args[1], "exit") == 0){
    printf("Type: exit to terminate the shell gracefully\n");
  }
  else if (strcmp(args[1], "usage") == 0){
    printf("Type: usage cd/help/exit\n");
  }
  else if (strcmp(args[1], "env") == 0){
    printf("Type: env to list all registered env variables\n");
  }
  else if (strcmp(args[1], "setenv") == 0){
    printf("Type: setenv ENV=VALUE to set a new env variable\n");
  }
  else if (strcmp(args[1], "unsetenv") == 0){
    printf("Type: unsertenv ENV to remove this env from the list of env variables\n");
  }
  else if (strcmp(args[1], "clear") == 0){
    printf("The command you gave: clear, is not part of CSEShell's builtin command\n");
  }
  return 0;
}
int list_env(char **args){
  char **env = environ; // Pointer to the array of environment strings
    
  while (*env) { // Loop until NULL pointer is encountered
      printf("%s\n", *env); // Print the current environment variable
      env++; // Move to the next environment variable
  }
  
  return 0;
}
int set_env_var(char **args){
  // Check if args[1] provided
  if (args[1] == NULL) {
    printf("Usage: setenv <variable>=<value>\n");
    return 0;
  }

  // Get position of '=' in args[1]
  char *equals_pos = strchr(args[1], '=');

  if (equals_pos == NULL) {
    // '=' not found
    printf("Usage: setenv <variable>=<value>\n");
    return 0;
  }

  // Separation of variable and value
  *equals_pos = '\0'; //set = to null term, points equal pos to start of value after =
  char *variable = args[1];
  char *value = equals_pos + 1; //value starts after '='

  if (value[0] != '"' || value[strlen(value) - 1] != '"') {
    //Value is not enclosed properly by ""
    printf("Usage: setenv <variable>=<value>\n");
    return 0;
  }

  value[strlen(value) - 1] = '\0'; // Remove the closing quote
  value += 1; // Skip the opening quote

  // Allocate memory for str 2 accounts for = and null terminator
  char *env_var = malloc(strlen(variable) + strlen(value) + 2); 
  
  if (env_var == NULL) {
    // Failed to allocate memory
    perror("CSEShell: ");
    return 0;
  } 

  // Construct the environment variable string
  sprintf(env_var, "%s=%s", variable, value);

  if (putenv(env_var) != 0) {
    // Failed to set environment variable
    perror("CSEShell: ");
    free(env_var); //dealloc memory
    return 0;
  }

  return 0;
}

int unset_env_var(char **args) {
    // Check if args[1] provided
    if (args[1] == NULL) {
        printf("Usage: unsetenv <variable>\n");
        return 0;
    }

    // Get the variable name
    char *variable = args[1];

    // Check if env variable exists
    if (getenv(variable) == NULL) {
        // Env variable does not exist
        return 0;
    }

    // Unset the env variable
    if (unsetenv(variable) != 0) {
        // Failed to unset env variable
        perror("CSEShell: ");
        return 0;
    }

    return 0;
}

void clear_cmd(char *cmd[]){
    // Free the allocated memory for the command arguments before exiting
  for (int i = 0; cmd[i] != NULL; i++)
  {
    free(cmd[i]);
    cmd[i] = NULL;
  }
}

int execute_builtin_function(char **args){
// Loop through our command list and check if the commands exist in the builtin command list
  for (int command_index = 0; command_index < num_builtin_functions(); command_index++)
  {
      if (strcmp(args[0], builtin_commands[command_index]) == 0) // Assume args[0] contains the first word of the command
      {
      // We will create new process to run the function with the specific command except for builtin commands.
      // These have to be done by the shell process. 
      return (*builtin_command_func[command_index])(args);
      }
  }
  return -1;
}

// The main function where the shell's execution begins
int main(void)
{
  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS];
  int child_status;
  pid_t pid;
  char cwd[1024]; //Stores current working directory

  while(1){
    type_prompt();     // Display the prompt
    read_command(cmd); // Read a command from the user

      // If the command is empty, skip to the next iteration
    if (cmd[0] == NULL || strcmp(cmd[0], "") == 0) {
        continue;
    }

    // exit will return 1, the rest  of builtin commands will return 0, non-builtin commands will return -1
    int builtin_status = execute_builtin_function(cmd);
    
    if (builtin_status == 1){
      clear_cmd(cmd);
      break;
    } else if (builtin_status == 0){
      clear_cmd(cmd);
      continue;
    }

    pid = fork(); //Fork a child process

    if (pid < 0) {
      // Failed to fork child process
      exit(1);
    } else if (pid == 0) {
      //Child process

      // Formulate the full path of the command to be executed
      char full_path[PATH_MAX];
      // Get home directory
      const char *home_dir = getenv("HOME");
      if (getcwd(cwd, sizeof(cwd)) != NULL)
      {
        //snprintf(full_path, sizeof(full_path), "%s/bin/%s", cwd, cmd[0]); //previous relative path
        snprintf(full_path, sizeof(full_path), "%s/programming-assignment-1-2024-ci02-r-b/bin/%s", home_dir, cmd[0]); //absolute path
      }
      else
      {
        printf("Failed to get current working directory.");
        exit(1);
      }

      execv(full_path, cmd);

      // If execv returns, command execution has failed
      printf("Command %s not found\n", cmd[0]);
      exit(1);

    } else {
      //Parent process shell (pid > 0)

      // Wait for the child process to complete
      waitpid(pid, &child_status, 0);

      // Check if child process terminated normally
      if (WIFEXITED(child_status)) {
        printf("Child process exited with status %d.\n", WEXITSTATUS(child_status));
      } else {
        printf("Child process terminated abnormally.\n");
      }
    }

    // Free the allocated memory for the command arguments before exiting
    for (int i = 0; cmd[i] != NULL; i++)
    {
      free(cmd[i]);
      cmd[i] = NULL;
    }
    memset(cwd, '\0', sizeof(cwd)); // clear the cwd array

  }

  return 0;
}
