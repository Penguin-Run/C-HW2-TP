#include "../include/IO_manager.h"

size_t get_file_size(char* filename) {
    struct stat st;
    stat(filename, &st);

    return st.st_size;
}

char* load_file(char* filename) {
    FILE *f;
    if((f = fopen(filename, "r")) == NULL) {
        printf ("Cannot open file.\n");
        return NULL;
    }
    int fd = fileno(f);
    size_t file_size = get_file_size(filename);
    char *region = mmap(NULL,
                        file_size,
                        PROT_WRITE,
                        MAP_PRIVATE,
                        fd,
                        0);
    if (region == MAP_FAILED) {
        printf("mmap failed\n");
        close(fd);
        return NULL;
    }

    close(fd);
    return region;
}

void print_bytes(char* string) {
    printf("What was read:\n");
    int i = 0;
    while (string[i]) {
        printf("%d ", string[i]);
        i++;
    }
    printf("\n");
}

// doesn't work properly
void generate_file(char* filename) {
    FILE* f;
    if((f = fopen(filename, "w")) == NULL) {
        printf ("Cannot create file.\n");
        return;
    }
    fputs("dddddddddd", f);

    int fd = fileno(f);
    close(fd);
}


