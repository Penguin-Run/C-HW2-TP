#include "../include/IO_manager.h"

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

size_t get_file_size(const char* filename) {
    struct stat st;
    stat(filename, &st);

    return st.st_size;
}

char* load_file(const char* filename) {
    FILE *f;
    if((f = fopen(filename, "r")) == NULL) {
        printf ("Cannot open file\n");
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
        fclose(f);
        return NULL;
    }

    fclose(f);
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

int generate_unique_filename(const char* base, size_t base_size, size_t salt_size, char* result) {
    if (result == NULL) {
        fprintf(stderr, "NULL pointer passed in generate_unique_filename()\n");
        return -1;
    }

    char* salt = malloc(salt_size * sizeof(char));
    if (generate_random_sequence(salt, salt_size) == -1) {
        fprintf(stderr, "generate_random_sequence error\n");
        return -1;
    }
    strcpy(result, base);
    strcat(result, salt);

    free(salt);
    return 0;
}

int compare_files(const char* file_1, const char* file_2) {
    FILE* output_consecutive;
    FILE* output_parallel;
    if ((output_consecutive = fopen(file_1, "r")) == NULL) {
        printf ("Cannot open file\n");
        return -1;
    }
    if ((output_parallel = fopen(file_2, "r")) == NULL) {
        printf ("Cannot open file\n");
        fclose(output_consecutive);
        return -1;
    }

    size_t line_length_consecutive = 35;
    char* line_consecutive = malloc(line_length_consecutive * sizeof(char));
    size_t line_length_parallel = 35;
    char* line_parallel = malloc(line_length_parallel * sizeof(char));

    while((getline(&line_consecutive, &line_length_consecutive, output_consecutive) != -1) && (getline(&line_parallel, &line_length_parallel, output_parallel) != -1)) {
        if (strcmp(line_consecutive, line_parallel) != 0) {
            fclose(output_consecutive);
            fclose(output_parallel);
            free(line_consecutive);
            free(line_parallel);
            return 1;
        }
    }

    fclose(output_consecutive);
    fclose(output_parallel);
    free(line_consecutive);
    free(line_parallel);
    return 0;
}


// записывает случайную последовательность симолов размера bytes байтов в массив char
int generate_random_sequence(char* sequence, int num_of_bytes) {
    if (sequence == NULL) {
        fprintf(stderr, "NULL pointer passed\n");
        return -1;
    }

    srand(time(0));
    for (int i = 0; i < num_of_bytes; i++) {
        int random_symbol = (41 + rand() % 86); // from 41 to 126
        sequence[i] = (char)random_symbol;
    }
    return 0;
}

// записывает случайную последовательность симолов размера bytes байтов в файл test_data/generated_file
// случайная последовательность повторяется
void generate_file(int num_of_bytes) {
    char* generated_file_name = "../test_data/generated_file";
    FILE* file = fopen(generated_file_name, "w");
    if (!file) {
        fprintf(stderr, "Failed to create file in generate_file()");
        return;
    }

    for (int i = 0; i < num_of_bytes; i++) {
        int random_symbol = (41 + rand() % 86); // from 41 to 126
        fputc(random_symbol, file);
    }
    fclose(file);
}

// записывает случайную последовательность симолов размера bytes байтов в файл test_data/generated_file_random
void generate_random_file(int num_of_bytes) {
    char* generated_file_name = "../test_data/generated_file_random";
    FILE* file = fopen(generated_file_name, "w");
    if (!file) {
        fprintf(stderr, "Failed to create file in generate_random_file()");
        return;
    }

    srand(time(0));
    for (int i = 0; i < num_of_bytes; i++) {
        int random_symbol = (41 + rand() % 86); // from 41 to 126
        fputc(random_symbol, file);
    }
    fclose(file);
}

void print_result(const char* filename_output, int* diff_count) {
    if (filename_output == NULL)
        filename_output = "pass/incorrect/path";
    FILE* f = fopen(filename_output, "w");
    if (!f) {
        printf("Failed to open file for write the result. Using default output stream..\n");
        f = stdout;
    }

    for (int i = 0; i < NUM_OF_DIFF_COUNT; i++)
        fprintf(f, "Pairs with byte difference %d: %d\n", i, diff_count[i]);

    fclose(f);
}

int work_from_file(const char* filename_input, const char* filename_output) {
    char* region = load_file(filename_input); // allocate mmap!
    if (region == NULL) {
        fprintf(stderr, "error while loading file and allocating memory for it with mmap\n");
        return -1;
    }

    int size = (int) get_file_size(filename_input);
    printf("Size of file: %d\n", size);

    work(region, size, filename_output);

    // очищение mmap памяти
    if (munmap(region, get_file_size(filename_input)) != 0) {
        fprintf(stderr, "munmap failed\n");
        return -1;
    }

    return 0;
}

int work(char* region, size_t file_size, const char* filename_output) {
    if (region == NULL) {
        fprintf(stderr, "NULL pointer passed\n");
        return -1;
    }
    int* diff_count = (int*) calloc(NUM_OF_DIFF_COUNT, sizeof(int));

    find_diff(region, file_size, diff_count, NUM_OF_DIFF_COUNT);

    print_result(filename_output, diff_count);

    free(diff_count);

    return 0;
}


