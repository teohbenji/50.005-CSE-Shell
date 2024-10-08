# CSEShell

SUTD 50.005 Computer System Engineering Project. CSEShell is a simple, custom shell for Unix-based systems, designed to provide an interface for executing system programs. This project includes a basic shell implementation, a set of system programs (`find`, `ld`, `ldr`, `sys`, `dspawn`, `dcheck`, `backup`), and some test files.

## Directory Structure

The project is organized as follows:

- `bin/` - Contains compiled executables for system programs.
  - `find` - Program to find files.
  - `ld` - Program for listing the contents of the current directory.
  - `ldr` - Program for listing the contents of the current directory recursively.
- `cseshell` - The main executable for the CSEShell.
- `.cseshellrc` - Config file for the CSEShell.
- `.dspawn.log` - Log file created by dspawn.
- `files/` - Contains various test files used with the shell and system programs.
  - `combined.txt`, `file1.txt`, `file2.txt`, ... - Test text files.
  - `notes.pdf` - A PDF file for testing.
  - `ss.png` - An image file.
- `makefile` - Makefile for building the CSEShell and system programs.
- `source/` - Source code for the shell and system programs.
  - `shell.c` and `shell.h` - Source and header files for the shell.
  - `system_programs/` - Source code and header for the system programs.

## Building the Project

To build the CSEShell and system programs, run the following command in the root directory:

```bash
make
```

This will compile the source code and place the executable files in the appropriate directories.

## Running CSEShell

After building, you can start the shell by running:

```bash
./cseshell
```

From there, you can execute built-in commands and any of the included system programs (e.g., `find`, `ld`, `ldr`,`sys`, `dspawn`, `dcheck`, `backup`).

## System Programs

- `find.c` - Searches for files in a directory.
- `ld.c` - List the contents of the curent directory.
- `ldr.c` - List the contents of the current directory recursively.
- `sys.c` - Prints information about the OS, memory, user, CPU, storage, uptime and GPU. 
- `dspawn.c` - Creates a daemon process that clears and writes to dspawn.log
- `dcheck.c` - Prints the number of live daemons spawned from dspawn
- `backup.c` - Zips a directory matching the environment variable `BACKUP_DIR`, and move it to `[PROJECT_DIR]/archive/`

Each program can be executed from the CSEShell once it is running. This starter code only allows the shell to execute a command once before exiting because `execv` replace the entire process' address space. Students need to fix this and allow the shell to prompt for more commands in Programming Assignment 1.

## Files Directory

The `files/` directory contains various text, PDF, and image files for testing the functionality of the CSEShell and its system programs.

## Makefile

The Makefile contains rules for compiling the shell and system programs. You can clean the build by running:

```bash
make clean
```

## Source Directory

Contains all the necessary source code for the shell and system programs. It is divided into the shell implementation (`shell.c`, `shell.h`) and system programs (`system_programs/`).

## Builtin Functions

### `cd`
Changes the current directory of the shell to the provided file path. If no path is given, it switches to the user's home directory.

##### Usage:
```code
$ cd <directory_path>
```

### `help`
Lists all built-in commands of the shell

##### Usage:
```code
$ help 
```

### `exit`
Exits the shell progam gracefully.

##### Usage:
```code
$ exit 
```

### `usage`
Briefly explains how to use the specified shell command. 

##### Usage:
```code
$ usage <command> 
```

### `env`
Lists all environment variables in the current shell session.

##### Usage:
```code
$ env 
```

### `setenv`
Creates or edits an environment variable within the current shell session.

##### Usage:
```code
$ setenv <variable>="<value>" 
```

### `unsetenv`
Removes an environment variable from the shell.

##### Usage:
```code
$ unsetenv <variable>
```


## Additional Features

### `.cseshellrc` Configuration File

The configuration file contains the path variable, commands that will be executed upon initialisation and color customisation.

Edit the shell's executable search path as follows:

```code
PATH=<cseshell_location>/bin:/usr/bin
```

Commands on each line following `PATH=` will execute on shell startup. These commands include builtin programs and programs in usr/bin.
```code
<command>
<command> <arg>
```


Modify the shell's username and time for every prompt as follows. The colors `RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, PINK`are available. If a color isn't specified, it defaults to white.
```code
USERNAME=<color>
TIME=<color>
DIR=<color>
```

Restart the shell after making changes.

### Informative shell prompt
The shell prompt contains useful information about the current user, the current time and the working directory.

### Low-power shell mode
The shell automatically sleeps after 1 minute of idle time.
## Sustainability and Inclusivity
### Sustainability: Low-power mode shell
After 1 minute of idle time, the shell will go to sleep. This reduction helps conserve energy by minimizing power consumption, helping environmental sustainability through smart resource management.

### Inclusivity: Customizable and Informative Shell Interface
The CSEShell interface allows users to customise the appearance of the shell's interface to their individual preferences. For example, the user can individually customise the colors in each shell prompt. Each shell prompt also displays the current user, the time now and the current working directory.

