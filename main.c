#include "include/IO_manager.h"

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

#define FILENAME_OUTPUT_CONSECUTIVE "../test_data/test_results/stress_tests/main_results_consecutive"
#define FILENAME_OUTPUT_PARALLEL "../test_data/test_results/stress_tests/main_results_parallel"
#define SEQUENCE_LENGTH 1000000

// TODO: в конфиге поменять сборки и убрать запуск main

int main() {
    // подключение динамической библиотеки с параллельной реализацией
    void *library; // объект для привязки внешней библиотеки
    int (*work_parallel)(char*, size_t, const char*);

    // "./libparallel_work_lib.dylib" on mac
    // "./libparallel_work_lib.so" on linux
    library = dlopen("./libparallel_work_lib.dylib", RTLD_LAZY);
    if (!library) {
        fprintf(stderr, "library opening failed\n");
        return -1;
    }

    // загрузка функции
    void* func = dlsym(library, "work");
    work_parallel = (int (*)(char*, size_t, const char*)) func;


    // генерация данных в массив char
    char* sequence = malloc(SEQUENCE_LENGTH * sizeof(char));
    generate_random_sequence(sequence, SEQUENCE_LENGTH);

    // вызов последовательной и параллельной реализации алгоритма
    work(sequence, SEQUENCE_LENGTH, FILENAME_OUTPUT_CONSECUTIVE);
    (*work_parallel)(sequence, SEQUENCE_LENGTH, FILENAME_OUTPUT_PARALLEL);

    // закрытие динамической библиотеки и очищение сгенерированных данных
    dlclose(library);
    free(sequence);


    // сравнение результатов
    FILE* output_consecutive;
    FILE* output_parallel;
    if(
        ((output_consecutive = fopen(FILENAME_OUTPUT_CONSECUTIVE, "r")) == NULL) ||
        ((output_parallel = fopen(FILENAME_OUTPUT_PARALLEL, "r")) == NULL)
    )
    {
        printf ("Cannot open file\n");
        return -1;
    }

    size_t line_length_consecutive = 35;
    char* line_consecutive = malloc(line_length_consecutive * sizeof(char));

    size_t line_length_parallel = 35;
    char* line_parallel = malloc(line_length_parallel * sizeof(char));

    while((getline(&line_consecutive, &line_length_consecutive, output_consecutive) != -1) && (getline(&line_parallel, &line_length_parallel, output_parallel) != -1)) {
        if (strcmp(line_consecutive, line_parallel) != 0) {
            fprintf(stderr, "Consistent and parallel algorithms results are not equal\n");
            free(line_consecutive);
            free(line_parallel);
            return 0;
        }
    }
    printf("Consistent and parallel algorithms results are equal\n");

    free(line_consecutive);
    free(line_parallel);
    return 0;
}