#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>


char* load_file(char* filename) {
    FILE *f;
    if((f = fopen(filename, "r")) == NULL) {
        printf ("Cannot open file.\n");
        return NULL;
    }
    int fd = fileno(f);
    struct stat st;
    stat(filename, &st);

    size_t file_size = st.st_size;

    // PROT_READ - чтение; PROT_WRITE - чтение/запись
    // MAP_PRIVATE - не записывать в файл;
    // MAP_POPULATE - предзагрузка файла ядром;
    // MAP_SHARED - деление с другими процессами
    char *region = mmap(NULL,
                        file_size,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE, // MAP_POPULATE missing !!
                        fd,
                        0);
    if (region == MAP_FAILED) {
        printf("mmap failed\n");
        close(fd);
        return NULL;
    }

    // printf("What was read:\n%d\n", region[2]);
    // write(fileno(stdout), region, file_size);
    // fileno(stdout) == 1

    // TODO: очистить mmap память
    /*
    if (munmap(region, file_size) != 0) {
        printf("munmap failed\n");
    }
     */

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

void* thread_routine(void* arg) {
    int errflag = 0;
    // detach from main process as a POSIX thread until the end
    errflag = pthread_detach(pthread_self());
    // check if pthread_detach() call was successful
    if (errflag != 0) {
        printf("Thread: caught error: %d\n", errflag);
    }

    printf("I'm a new thread!\n");

    return arg;
}

void find_diff_parallel(char* region, int* diff_count, int num_of_diff) {
    int err_flag = 0;
    pthread_t thread; // thread id

    // create and tun POSIX thread
    err_flag = pthread_create(&thread, NULL, thread_routine, NULL);
    // check if thread_create call was successful
    if (err_flag != 0) {
        printf("Main: caught error: %d\n", err_flag);
    }

    printf("I'm main thread!\n");
}

#define NUM_OF_DIFF 11

int main() {
    char* region = load_file("/Users/Ivan/TPark-SEM1/C-HW2-TP/testfile");
    print_bytes(region);

    int* diff_count = calloc(NUM_OF_DIFF, sizeof(int));
    find_diff_consecutive(region, diff_count, NUM_OF_DIFF);
    // find_diff_parallel(region, diff_count, NUM_OF_DIFF);

    for (int i = 0; i < NUM_OF_DIFF; i++) printf("Pairs with byte difference %d: %d\n", i, diff_count[i]);

    free(diff_count);
    return 0;
}