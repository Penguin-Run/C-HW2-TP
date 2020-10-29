#include "../include/work.h"

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#include <stdlib.h>
#include <zconf.h>

typedef struct data_chunk {
    char* data;
    size_t size;
    int num_of_diff;
    int* diff_count;
} data_chunk;


void* thread_routine(void* arg) {
    data_chunk* chunk = (data_chunk*) arg;

    // сравниваем только тот участок, что был выделен этому потоку
    int size = chunk->size;
    // если потоку достался конец данных - проходимся до его окончания, а не до chunk->size
    if (strlen(chunk->data) <= chunk->size) size = (int)strlen(chunk->data) - 1;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < chunk->num_of_diff; j++) {
            // сравниваем пары в чанке + сравниваем граничное значение со следующим чанком (чтобы не потерять пару)
            if (abs(chunk->data[i] - chunk->data[i+1]) == j)
                chunk->diff_count[j]++;
        }
    }

    return arg;
}

// возвращает количество ядер процессора системы
int get_max_processors_of_system() {
    return (int) (sysconf(_SC_NPROCESSORS_ONLN) / 2 - 1) * 4;
}


void find_diff(char* region, size_t file_size, int* diff_count, int num_of_diff) {
    printf( "PARALLEL running..\n");
    // ставлю количество потоков в зависимости от количества ядер процессора
    int num_of_threads = get_max_processors_of_system();
    if (num_of_threads <= 0) {
        fprintf(stderr, "error while getting info about maximum number of processes in the system\n");
        return;
    }

    pthread_t threads[num_of_threads]; // thread id
    for (int i = 0; i < num_of_threads; i++) {
        data_chunk* chunk = calloc(1, sizeof(data_chunk));
        // разделяем данные файла между потоками на чанки размером data_chunk_size
        chunk->size = (size_t)ceil((double)file_size / num_of_threads);
        // отдаем каждому потоку его секцию данных
        chunk->data = region + i * chunk->size;
        chunk->num_of_diff = num_of_diff;
        chunk->diff_count = calloc(num_of_diff, sizeof(int));

        // create and run POSIX thread
        int err_flag = 0;
        err_flag = pthread_create(&threads[i], NULL, thread_routine, chunk);
        // check if thread_create call was successful
        if (err_flag != 0) {
            printf("Main: caught error: %d\n", err_flag);
        }
    }

    // wait for threads to complete work
    for (int i = 0; i < num_of_threads; i++) {
        void* return_value;
        int err_flag = pthread_join(threads[i], &return_value);
        if (err_flag != 0) {
            printf("Main: caught error %d while joining thread\n", err_flag);
        }
        data_chunk* chunk = (data_chunk*) return_value;

        // добавляем результат вычислений потока в общий результат
        for (int j = 0; j < num_of_diff; j++) {
            diff_count[j] += chunk->diff_count[j];
        }

        // чистим память выделенную под чанк
        free(chunk->diff_count);
        free(chunk);
    }
}


