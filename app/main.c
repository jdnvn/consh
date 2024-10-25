#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <termios.h>

#define MAX_HISTORY 10
#define INPUT_SIZE 100

#define HI_COMMAND "hi"
#define EXIT_COMMAND "exit"
#define ECHO_COMMAND "echo"
#define LS_COMMAND "ls"
#define CD_COMMAND "cd"
#define CAT_COMMAND "cat"

const char* valid_commands[] = { HI_COMMAND, EXIT_COMMAND, ECHO_COMMAND, LS_COMMAND, CD_COMMAND, CAT_COMMAND };

char command_history[MAX_HISTORY][INPUT_SIZE];
int command_index = 0;
int history_count = 0;

void ls() {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
      while ((dir = readdir(d)) != NULL) {
          printf("%s\n", dir->d_name);
      }
      closedir(d);
  }
}

int cat(const char* filename) {
  FILE *fp;
  char ch;

  fp = fopen(filename, "r");

  if (fp == NULL) {
      printf("Error opening file %s\n", filename);
      return 1;
  }

  while ((ch = fgetc(fp)) != EOF) {
      printf("%c", ch);
  }

  fclose(fp);

  return 0;
}

bool is_command(const char* input, const char* command) {
  return strcmp(input, command) == 0;
}

void set_raw_mode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

void add_to_history(const char* command) {
  if (history_count < MAX_HISTORY) {
    strcpy(command_history[history_count++], command);
  } else {
    // Shift history up and add new command at the end
    for (int i = 1; i < MAX_HISTORY; i++) {
        strcpy(command_history[i - 1], command_history[i]);
    }
    strcpy(command_history[MAX_HISTORY - 1], command);
  }
  command_index = history_count;
}

int main() {
  char input[INPUT_SIZE];

  while (true) {
    printf("$ ");
    fflush(stdout);

    set_raw_mode(true);

    int index = 0;
    while (index < sizeof(input) - 1) {
      char c = getchar();
      if (c == '\n') {
        input[index] = '\0';
        putchar(c);
        fflush(stdout);
        break;
      } else if (c == '\033') {
        getchar(); // skip the '['
        switch (getchar()) {
          case 'A': // up arrow
            if (command_index > 0) {
              command_index--;
              const char* prev_command = command_history[command_index];
              // clear current input
              while (index > 0) {
                  printf("\b \b");
                  index--;
              }
              strcpy(input, prev_command);
              index = strlen(prev_command);
              printf("%s", prev_command);
              fflush(stdout);
            }
            continue;
          case 'B': // down arrow
            if (command_index <= history_count) {
              command_index++;
              const char* next_command = command_history[command_index];
              // clear current input
              while (index > 0) {
                  printf("\b \b");
                  index--;
              }
              strcpy(input, next_command);
              index = strlen(next_command);
              printf("%s", next_command);
              fflush(stdout);
            }
        }
      } else if (c == 127) { // Backspace key
        if (index > 0) {
          index--;
          printf("\b \b"); // Move cursor back, print space, move cursor back again
          fflush(stdout);
        }
      } else {
        input[index++] = c;
        putchar(c);
        fflush(stdout);
      }
    }

    // Restore terminal to normal mode
    set_raw_mode(false);

    if (index > 0) {
      add_to_history(input);
    } else {
      command_index = history_count;
      continue;
    }

    const char* command = strtok(input, " ");

    int i;
    bool is_valid = false;
    for (i = 0; i < sizeof(valid_commands) / sizeof(valid_commands[0]); i++) {
      if (strcmp(valid_commands[i], command) == 0) {
        is_valid = true;
        break;
      }
    }
    if (is_valid) {
      if (is_command(command, HI_COMMAND)) {
        printf("HI!!!!\n");
      } else if (is_command(command, EXIT_COMMAND)) {
        return 0;
      } else if (is_command(command, ECHO_COMMAND)) {
        const char* arg = strtok(NULL, " ");
        printf("%s\n", arg);
      } else if (is_command(command, LS_COMMAND)) {
        ls();
      } else if (is_command(command, CD_COMMAND)) {
        const char* dir = strtok(NULL, " ");
        chdir(dir);
      } else if (is_command(command, CAT_COMMAND)) {
        const char* filename = strtok(NULL, " ");
        cat(filename);
      }
    } else {
      printf("%s: command not found\n", input);
    }
  }

  return 0;
}
