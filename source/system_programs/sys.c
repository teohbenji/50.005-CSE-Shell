#include "system_program.h"
#include <stdio.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/statvfs.h>
/*
 List all information
*/

void print_ascii_art() {
    printf("      .--..--..--..--..--..--.\n");
    printf("    .' \\  (`._   (_)     _   \\\n");
    printf("  .'    |  '._)         (_)  |\n");
    printf("  \\ _.')\\      .----..---.   /\n");
    printf("  |(_.'  |    /    .-\\-.  \\  |\n");
    printf("  \\     0|    |   ( O| O) | o|\n");
    printf("   |  _  |  .--.____.'._.-.  |\n");
    printf("   \\ (_) | o         -` .-`  |\n");
    printf("    |    \\   |`-._ _ _ _ _\\ /\n");
    printf("    \\    |   |  `. |_||_|   |\n");
    printf("    | o  |    \\_      \\     |     -.   .-.\n");
    printf("    |.-.  \\     `--..-'   O |     `.`-' .'\n");
    printf("  _.'  .' |     `-.-'      /-.__   ' .-'\n");
    printf(".' `-.` '.|='=.='=.='=.='=|._/_ `-'.\n");
    printf("`-._  `.  |________/\\_____|    `-.'\n");
    printf("   .'   ).| '=' '='\\/ '=' |\n");
    printf("   `._.`  '---------------'\n");
    printf("           //___\\   //___\\\n");
    printf("             ||       ||\n");
    printf("             ||_.-.   ||_.-.\n");
    printf("            (_.--__) (_.--__) \n");
}

void print_os_info() {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        perror("uname");
        _exit(1);
    }
    printf("Operating System: %s\n", buffer.sysname);
    printf("Node Name: %s\n", buffer.nodename);
    printf("Kernel Version: %s\n", buffer.release);
    printf("Kernel Build: %s\n", buffer.version);
    printf("Machine: %s\n", buffer.machine);
}

void print_memory_info() {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        _exit(1);
    }
    printf("Total RAM: %ld MB\n", info.totalram / 1024 / 1024);
    printf("Free RAM: %ld MB\n", info.freeram / 1024 / 1024);
}

void print_user_info() {
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        printf("User: %s\n", pw->pw_name);
    } else {
        perror("getpwuid");
        _exit(1);
    }
}

void print_cpu_info() {
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo == NULL) {
        perror("fopen");
        _exit(1);
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), cpuinfo)) {
        if (strncmp(buffer, "model name", 10) == 0) {
            printf("CPU: %s", strchr(buffer, ':') + 2);
            break;
        }
    }
    fclose(cpuinfo);
}

void print_storage_info() {
    struct statvfs stat;
    if (statvfs("/", &stat) != 0) {
        perror("statvfs");
        _exit(1);
    }
    unsigned long long total_space = stat.f_blocks * stat.f_frsize;
    unsigned long long free_space = stat.f_bfree * stat.f_frsize;
    printf("Total SSD Space: %llu GB\n", total_space / 1024 / 1024 / 1024);
    printf("Free SSD Space: %llu GB\n", free_space / 1024 / 1024 / 1024);
}

void print_uptime() {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        _exit(1);
    }
    printf("Uptime: %ld days, %ld hours, %ld minutes, %ld seconds\n",
           info.uptime / 86400,
           (info.uptime % 86400) / 3600,
           (info.uptime % 3600) / 60,
           info.uptime % 60);
}

void print_gpu_info() {
    FILE *fp;
    char path[1035];

    /* Open the command for reading. */
    fp = popen("nvidia-smi --query-gpu=name,memory.total,memory.free,utilization.gpu --format=csv,noheader,nounits", "r");
    if (fp == NULL) {
        perror("popen");
        _exit(1);
    }

    /* Read the output a line at a time - output it. */
    printf("GPU Information:\n");
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        printf("%s", path);
    }

    /* Close the command stream. */
    pclose(fp);
}

int execute(char **args)
{   
    printf("\n");
    printf("System Information:\n\n");
    print_os_info(); 
    print_memory_info();
    print_storage_info();
    print_user_info();
    print_cpu_info();
    print_uptime();
    printf("\n");
    return 0;
}




int main(int argc, char **args)
{
    return execute(args);
}