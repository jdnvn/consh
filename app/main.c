#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const char* valid_commands[] = {"hi", "exit"};

int main() {
  while (true) {
    // Uncomment this block to pass the first stage
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);

    int len = strlen(input);

    // just a newline
    if (len == 1) {
      continue;
    }

    // strip newline
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = 0;
    }

    bool is_valid = false;

    int i;
    for (i = 0; i < sizeof(valid_commands) / sizeof(valid_commands[0]); i++) {
      if (strcmp(valid_commands[i], input) == 0) {
        is_valid = true;
        break;
      }
    }
    if (is_valid) {
      if (i == 0) {
        printf("HI!!!!\n");
      } else if (i == 1) {
        return 0;
      }
    } else {
      printf("%s: command not found\n", input);
    }
  }

  return 0;
}
