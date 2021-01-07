#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <termios.h>
#include "cipher.h"

#define PASSWORD_MAX_SIZE 256

byte_t *
get_password(char *prompt)
{
    byte_t *password = calloc(PASSWORD_MAX_SIZE + 1, sizeof(byte_t));
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("%s", prompt);
    static struct termios oldt, newt;
    int i = 0;
    int c;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VTIME] = 0;
    newt.c_cc[VMIN] = 1;       
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((c = fgetc(stdin)) != '\n' && c != EOF && i < PASSWORD_MAX_SIZE)
    {
        if (c != 127)
        {
            password[i++] = c;
            fputc(0x2a, stdout);
        }
        else if (i > 0)
        {
            fputc (0x8, stdout);
            fputc (' ', stdout);
            fputc (0x8, stdout);
            i --;
        }
    }
    password[i] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
    setvbuf(stdout, NULL, _IOFBF, 1);
    return password;
}

byte_t *
confirmed_password()
{
    while (1)
    {
        byte_t *password = get_password("Enter a password: ");
        while (strlen(password) < 4)
        {
            printf("Password is too short. Try again\n");
            password = get_password("Enter a password: ");
        }
        
        byte_t *repeated = get_password("Confirm password: ");

        if (strcmp(password, repeated) == 0 && strlen(password) > 3)
        {
            free(repeated);
            return password;
        }
        else
        {
            free(password);
            free(repeated);
            printf("Passwords do not match. Try again...\n");
        }
    }
}

void
encrypt_file(byte_t **subkeys, int rounds, char *filename, char *outfilename)
{
    FILE *input  = (strcmp(filename, "-") != 0) ? fopen(filename, "rb") : stdin;
    if (!input)
    {
        fprintf(stderr, "%s() error: Can't open %s\n", __func__, filename);
        return;
    }
    FILE *output = (strcmp(outfilename, "-") != 0) ? fopen(outfilename, "wb") : stdout;
    if (!output)
    {
        if (input != stdin)
            fclose(input);
        fprintf(stderr, "%s() error: Can't open %s\n", __func__, outfilename);
        return;
    }
    byte_t block[BLOCK_SIZE];
    int length;
    while (length = fread(block, sizeof(byte_t), BLOCK_SIZE, input))
    {
        if (length < BLOCK_SIZE)
            memset(&block[length], BLOCK_SIZE - length, BLOCK_SIZE - length);
        encrypt_block(block, subkeys, rounds);
        fwrite(block, sizeof(byte_t), BLOCK_SIZE, output);
        
    }
    fclose(input);
    fclose(output);
}

int
unpad(byte_t *block)
{
    int i;
    byte_t pad = block[BLOCK_SIZE - 1];
    if (pad >= BLOCK_SIZE)
        return 0;
    
    for (i = BLOCK_SIZE - pad; i < BLOCK_SIZE ; i ++)
    {
        if (block[i] != pad)
            return 0;
    }
    return pad;
}

void
decrypt_file(byte_t **subkeys, int rounds, char *filename, char *outfilename)
{
    FILE *input  = (strcmp(filename, "-") != 0) ? fopen(filename, "rb") : stdin;
    if (!input)
    {
        fprintf(stderr, "%s() error: Can't open %s\n", __func__, filename);
        return;
    }
    FILE *output = (strcmp(outfilename, "-") != 0) ? fopen(outfilename, "wb") : stdout;
    if (!output)
    {
        if (input != stdin)
            fclose(input);
        fprintf(stderr, "%s() error: Can't open %s\n", __func__, outfilename);
        return;
    }
    byte_t block[BLOCK_SIZE];
    int length;
    while (length = fread(block, sizeof(byte_t), BLOCK_SIZE, input))
    {
        decrypt_block(block, subkeys, rounds);
        int pad = unpad(block);
        fwrite(block, sizeof(byte_t), BLOCK_SIZE - pad, output);
    }
    fclose(input);
    fclose(output);
}

void
help(char *me)
{
    printf("%s - LvCipher demonstration program\n\n", me);
    printf("Usage: %s [option(s)] <input_file>\n\n", me);
    printf("Options:\n");
    printf(
            "-h         Show this help message and exit.\n"
            "-v         Show version and exit\n"
            "-e         Encrypt file\n"
            "-d         Decrypt file\n"
            "-o         Save output to file\n"
    );
}

void
version()
{
    printf("version 0.1 (BETA)\n");
}

int
main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s [options] <file>\n", argv[0]);
        return 1;
    }
    
    int opt, a = 0;
    char *output_file = NULL;
    while ((opt = getopt(argc, argv, "hvedo:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                help(argv[0]);
                exit(0);
            case 'v':
                version();
                exit(0);
            case 'e':
                a = 0;
                break;
            case 'd':
                a = 1;
                break;
            case 'o':
                output_file = optarg;
                break;
        }
    }
    if (optind >= argc)
    {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }
    
    printf("\n============= LvCipher Demonstration =============\n\n");
    char *input_file = argv[optind];
    if (!output_file)
        output_file = "-";
    int rounds;
    byte_t *password = a ? get_password("Password: ") : confirmed_password();
    printf("Generating subkeys...\n");
    byte_t **subkeys = expand_key(password, &rounds);
    printf("Destroying original key...\n");
    destroy_key(password);
    printf("%s with %d rounds..\n", (a ? "Decrypting" : "Encrypting"), rounds);
        
    if (a == 0)
        encrypt_file(subkeys, rounds, input_file, output_file);
    else
        decrypt_file(subkeys, rounds, input_file, output_file);
    printf("Destroying subkeys...\n");
    destroy_subkeys(subkeys, rounds);
    printf("Done.\n");
    return 0;
}