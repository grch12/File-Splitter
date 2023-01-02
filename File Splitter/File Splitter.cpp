#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long flength(FILE* fp) {
    unsigned long n = 0;
    fpos_t position;
    fgetpos(fp, &position);
    fseek(fp, 0, SEEK_END);
    n = (unsigned long) ftell(fp);
    fsetpos(fp, &position);
    return n;
}

void checkfp(FILE* fp) {
    if (fp == NULL) {
        perror("Error reading file");
    }
}


char int2str(int num) {
    char str[20] = "";
    sprintf_s(str, "%d", num);
    return *str;
}

int main(int argc, char *argv[]) {
    // Check parameters
    if (argc != 3) {
        printf("2 parameters required, but %d given.\n", argc - 1);
        exit(0);
    }

    FILE* file;
    fopen_s(&file, argv[1], "rb");

    if (file == NULL) {
        perror("Error reading file");
        return 0;
    }
    checkfp(file);

    unsigned long length = flength(file);
    unsigned int amount = (unsigned int) atoi(argv[2]);
    unsigned long each_fsize = length / amount;

    if (amount > length) {
        printf("Amount too big.\n");
        exit(0);
    }

    // Calculate size
    unsigned long n = length - each_fsize * amount;

    // Use For-Loop to write files
    for (unsigned int i = 0; i < amount; i++) {
        // New filename
        char* newfn = NULL;
        newfn = (char*) malloc(strlen(argv[1] + 2));
        strcpy_s(newfn, strlen(newfn), argv[1]);
        printf("%s", newfn);
        strcat_s(newfn, sizeof(newfn), ".");
        char ic[20];
        _itoa((int) i + 1, ic, 10);
        ic[strlen(ic)] = '\0';
        strcat_s(newfn, strlen(newfn) + 1, ic);


        printf("Generating %s...  ", newfn);

        // Open new file  
        FILE* newfile;
        fopen_s(&newfile, newfn, "wb");

        // Calculate size to read
        unsigned long size_to_read = each_fsize;
        if (n > 0) {
            size_to_read++;
            n--;
        }

        // Create a memory heap to store data
        void* content = NULL;
        content = malloc(size_to_read);
        if (content == NULL) {
            perror("Error reading file");
            exit(0);
        }
        fread_s(content, size_to_read, 1, size_to_read, file);
        if (content == NULL || newfile == NULL) {
            perror("Error reading file");
            exit(0);
        }
        fwrite(content, 1, size_to_read, newfile);

        printf("%s generated.\n", newfn);

        // Close file & free memory used
        fclose(newfile);
        free(content);
    }

    fclose(file);
    return 0;
}