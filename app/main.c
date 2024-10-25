#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// void run_command(char command[]) {
//   print("running %s", command);
// }

// int strlen(char string[]) {
//   int i = 0;
//   while (string[i] != '\0') {
//     i++;
//   }
//   return i;
// }

int main() {
  // Uncomment this block to pass the first stage
  printf("$ ");
  fflush(stdout);

  // Wait for user input
  char input[100];
  fgets(input, 100, stdin);

  int len = strlen(input);
  if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = 0;
  }

  const char* valid_commands[] = {"hi"};
  bool is_valid = false;

  for (int i = 0; i < sizeof(valid_commands) / sizeof(valid_commands[0]); i++) {
    if (strcmp(valid_commands[i], input) == 0) {
      is_valid = true;
      break;
    }
  }
  if (!is_valid) {
    printf("\n%s: command not found", input);
  }

  return 0;
}
