#include <stdio.h>
#include <stdlib.h>

#define MAX_COMMAND 1024

int main() {
    char command[MAX_COMMAND];
    char buffer[MAX_COMMAND];

    // Construct the find command
    snprintf(command, sizeof(command), "find /home/nix/Desktop/ransom/attacker/tests2 -type f");

    // Open a pipe to the command
    FILE *pipe = popen(command, "r");
    if (pipe == NULL) {
        perror("popen failed");
        return 1;
    }

    // Read the output of the command line by line and store it in a variable
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Do something with the file path (e.g., store it in an array)
        printf("%s", buffer); // Print the file path
    }

    // Close the pipe
    pclose(pipe);

    return 0;
}
