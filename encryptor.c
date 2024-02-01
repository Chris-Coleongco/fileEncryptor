#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define KEY_SIZE 256
#define IV_SIZE 16
#define BUFFER_SIZE 1024

void handleErrors() {
    fprintf(stderr, "Error occurred\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    // Initialize OpenSSL

    // Generate random key and IV
    unsigned char key[KEY_SIZE / 8];
    unsigned char iv[IV_SIZE];

    // ! encrypt the key using a rsa public key and iv and put it into a txt file



    if (RAND_bytes(key, sizeof(key)) != 1 || RAND_bytes(iv, sizeof(iv)) != 1) {
        handleErrors();
    }

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


