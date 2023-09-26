#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS cores

#include <unistd.h>
 
long cores ;


void *f(void *arg) {
  	long id = *(long*)arg;
  	for(int i=0;i<10;i++)
        printf("Hello World din thread-ul %ld!\n", id);
  	pthread_exit(NULL);
}


void g(void *arg)
{
    sleep(1);
    puts("NEATA");
}


int main(int argc, char *argv[]) {
    
    cores=sysconf(_SC_NPROCESSORS_CONF);
	pthread_t threads[NUM_THREADS];
  	int r;
  	long id;
  	void *status;
	long ids[NUM_THREADS];

  	for (id = 0; id < 1; id++) {
		ids[id] = id; 
		r = pthread_create(&threads[id], NULL, f, &ids[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}
    pthread_create(&threads[1], NULL, g, ids+1);

  	for (id = 0; id < 2; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	pthread_exit(NULL);
}
