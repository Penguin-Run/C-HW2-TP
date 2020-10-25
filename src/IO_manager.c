#include "../include/IO_manager.h"

size_t get_file_size(const char* filename) {
    struct stat st;
    stat(filename, &st);

    return st.st_size;
}

char* load_file(const char* filename) {
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

// записывает случайную последовательность симолов размера bytes байтов в файл test_data/generated_file
void generate_file(int num_of_bytes) {
    char* generated_file_name = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/generated_file";
    FILE* file = fopen(generated_file_name, "w");

    for (int i = 0; i < num_of_bytes; i++) {
        int random_symbol = (41 + rand() % 86); // from 41 to 126
        fputc(random_symbol, file);
    }
    fclose(file);
}

void print_result(const char* filename_output, int* diff_count) {
    FILE* f = fopen(filename_output, "w");
    if (!f) {
        printf("Failed to open file for write the result. Using default output stream..\n");
        f = stdout;
    }

    for (int i = 0; i < NUM_OF_DIFF_COUNT; i++)
        fprintf(f, "Pairs with byte difference %d: %d\n", i, diff_count[i]);

    fclose(f);
}

void work_from_file(const char* filename_input, const char* filename_output) {
    char* region = load_file(filename_input); // allocate mmap!
    int size = (int) get_file_size(filename_input);
    printf("Size of file: %d\n", size);
    // print_bytes(region);

    int* diff_count = (int*) calloc(NUM_OF_DIFF_COUNT, sizeof(int));

    find_diff(region, get_file_size(filename_input), diff_count, NUM_OF_DIFF_COUNT);

    print_result(filename_output, diff_count);

    free(diff_count);
    // очищение mmap памяти
    if (munmap(region, get_file_size(filename_input)) != 0) {
        printf("munmap failed\n");
    }
}


