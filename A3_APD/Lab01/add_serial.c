#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
    schelet pentru exercitiul 5
*/
#define NUM_THREADS 8

int* arr;
int array_size;

struct seq_add_args
{
    int *numv;
    size_t numc;
};

void seq_add(struct seq_add_args *args)
{
    for(int i=0;i<args->numc;i++)
        args->numv[i]+=100;
    free(args);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);

    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

    //for (int i = 0; i < array_size; i++) {
     //   printf("%d", arr[i]);
     //   if (i != array_size - 1) {
       //     printf(" ");
      //  } else {
      //      printf("\n");
     //   }
    //}

    // TODO: aceasta operatie va fi paralelizata
  	//for (int i = 0; i < array_size; i++) {
    //    arr[i] += 100;
    //}
    pthread_t threads[NUM_THREADS];

    for(int i=0;i<NUM_THREADS;i++)
    {
        struct seq_add_args *args=malloc(sizeof(struct seq_add_args));
        args->numv=arr+(int) (i*(double) array_size/NUM_THREADS);
        args->numc=array_size/NUM_THREADS;
        if(i+1==NUM_THREADS)
            args->numc=array_size-(int)(i*(double) array_size/NUM_THREADS);
        pthread_create(threads+i, NULL, seq_add, args);
    }
    
    int status;
    for(int i=0;i<NUM_THREADS;i++)
        pthread_join(threads[i], &status);

    //for (int i = 0; i < array_size; i++) {
      //  printf("%d", arr[i]);
      //  if (i != array_size - 1) {
      //      printf(" ");
       // } else {
      //      printf("\n");
     //   }
    //}
    free(arr);
  	pthread_exit(NULL);
}
