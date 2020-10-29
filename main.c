#include "include/IO_manager.h"

#include <dlfcn.h>
#include <stdio.h>

#define FILENAME_OUTPUT_CONSECUTIVE_BASE "../test_data/test_results/stress_tests/main_results_consecutive_"
#define FILENAME_OUTPUT_PARALLEL_BASE "../test_data/test_results/stress_tests/main_results_parallel_"
#define FILENAME_OUTPUT_BASE_SIZE 70
#define FILENAME_OUTPUT_SALT_SIZE 5
#define SEQUENCE_LENGTH 10000

int main() {
    // подключение динамической библиотеки с параллельной реализацией
    void *library; // объект для привязки внешней библиотеки
    int (*work_parallel)(char*, size_t, const char*);
    // "./libparallel_work_lib.dylib" on mac
    // "./libparallel_work_lib.so" on linux
    library = dlopen("./libparallel_work_lib.so", RTLD_LAZY);
    if (!library) {
        fprintf(stderr, "library opening failed\n");
        return -1;
    }
    void* func = dlsym(library, "work");
    work_parallel = (int (*)(char*, size_t, const char*)) func;

    // генерация случайных названий файлов и самих данных
    char* filename_consecutive = malloc((FILENAME_OUTPUT_BASE_SIZE + FILENAME_OUTPUT_SALT_SIZE) * sizeof(char));
    char* filename_parallel = malloc((FILENAME_OUTPUT_BASE_SIZE + FILENAME_OUTPUT_SALT_SIZE) * sizeof(char));
    generate_unique_filename(FILENAME_OUTPUT_CONSECUTIVE_BASE, FILENAME_OUTPUT_BASE_SIZE, FILENAME_OUTPUT_SALT_SIZE, filename_consecutive);
    generate_unique_filename(FILENAME_OUTPUT_PARALLEL_BASE, FILENAME_OUTPUT_BASE_SIZE, FILENAME_OUTPUT_SALT_SIZE, filename_parallel);
    char* sequence = malloc(SEQUENCE_LENGTH * sizeof(char));
    generate_random_sequence(sequence, SEQUENCE_LENGTH);

    // вызов последовательной и параллельной реализации алгоритма
    work(sequence, SEQUENCE_LENGTH, filename_consecutive);
    (*work_parallel)(sequence, SEQUENCE_LENGTH, filename_parallel);

    // закрытие динамической библиотеки и очищение сгенерированных данных
    dlclose(library);
    free(sequence);

    // сравнение результатов
    int res = compare_files(filename_consecutive, filename_parallel);
    free(filename_consecutive);
    free(filename_parallel);
    if (res == -1) {
        fprintf(stderr, "Error while comparing\n");
        return -1;
    }
    if (res == 1) {
        fprintf(stderr, "Consistent and parallel algorithms results are not equal\n");
        return 0;
    }
    printf("Consistent and parallel algorithms results are equal\n");
    return 0;
}