
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <time.h>

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

void find_diff_consecutive(char* region, int* diff_count, int num_of_diff) {
    for (int i = 0; i < num_of_diff; i++) {
        int count = 0;
        int j = 1;
        while(region[j]) {
            if (abs(region[j] - region[j-1]) == i)
                count++;
            j++;
        }
        diff_count[i] = count;
    }
}

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
// TODO: определять количество потоков в зависимости от возможностей системы
#define NUM_OF_THREADS 8

void find_diff_parallel(char* region, size_t file_size, int* diff_count, int num_of_diff) {
    pthread_t threads[NUM_OF_THREADS]; // thread id
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        data_chunk* chunk = calloc(1, sizeof(data_chunk));
        // разделяем данные файла между потоками на чанки размером data_chunk_size
        chunk->size = (size_t)ceil((double)file_size / NUM_OF_THREADS);
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
    for (int i = 0; i < NUM_OF_THREADS; i++) {
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

#define NUM_OF_DIFF 11

// TODO: в тестах настроить замер времени работы через chrono в тестах
// TODO: поставить cppcheck и valgrind в clion

int main() {
    char* filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/book.txt";

    char* region = load_file(filename); // allocate mmap!
    int size = (int) get_file_size(filename);
    printf("Size of file: %d\n", size);
    // print_bytes(region);

    int* diff_count = calloc(NUM_OF_DIFF, sizeof(int));

    // find_diff_consecutive(region, diff_count, NUM_OF_DIFF);
    find_diff_parallel(region, get_file_size(filename), diff_count, NUM_OF_DIFF);

    for (int i = 0; i < NUM_OF_DIFF; i++) printf("Pairs with byte difference %d: %d\n", i, diff_count[i]);

    free(diff_count);
    // очищение mmap памяти
    if (munmap(region, get_file_size(filename)) != 0) {
        printf("munmap failed\n");
    }

    return 0;
}