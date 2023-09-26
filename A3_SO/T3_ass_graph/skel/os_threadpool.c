#include "os_threadpool.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// https://github.com/open-education-hub/operating-systems/blob/cb6dd3259f883fbb03fe11fba4651d15fceebc7f/content/chapters/compute/lecture/demo/utils/utils.h#L10
#define DIE(assertion, call_description)			\
	do {							\
		if (assertion) {				\
			fprintf(stderr, "(%s, %d): ",		\
					__FILE__, __LINE__);	\
			perror(call_description);		\
			printf("(%s, %d): ",		\
					__FILE__, __LINE__);	\
			perror(call_description);		\
			exit(EXIT_FAILURE);			\
		}						\
	} while (0)

/* === TASK === */

/* Creates a task that thread must execute */
os_task_t *task_create(void *arg, void (*f)(void *))
{
    os_task_t *task = malloc(sizeof(os_task_t));
    DIE(task == NULL, "grav ba");
    task->argument = arg;
    task->task = f;
    return task;
}

/* Add a new task to threadpool task queue */
void add_task_in_queue(os_threadpool_t *tp, os_task_t *t)
{
    struct _node *task_node = malloc(sizeof(struct _node));
    DIE(task_node == NULL, "grav ba");
    task_node->task = t;
    task_node->next = NULL;

    struct _node *it;
    pthread_mutex_lock(&tp->taskLock);
    if(tp->tasks == NULL)
    {
        tp->tasks = task_node;
    }
    else
    {
        for(it = tp->tasks; it->next; it = it->next);
        it->next = task_node;
    }
    pthread_mutex_unlock(&tp->taskLock);
}

/* Get the head of task queue from threadpool */
os_task_t *get_task(os_threadpool_t *tp)
{
    struct _node *next_node;
	while(1)
	{
		// wait for a task in queue
		while(!tp->tasks && !tp->should_stop);

		if(tp->should_stop)
			return NULL;

		pthread_mutex_lock(&tp->taskLock);

		// if ta task is still in queue, get out
		if(tp->tasks)
			break;

		// the task was consumed, keep waiting
		pthread_mutex_unlock(&tp->taskLock);
	}

    next_node = tp->tasks;
    tp->tasks = next_node->next;
    pthread_mutex_unlock(&tp->taskLock);

    os_task_t *next_task = next_node->task;
    free(next_node);

    return next_task;
}

/* === THREAD POOL === */

/* Initialize the new threadpool */
os_threadpool_t *threadpool_create(unsigned int nTasks, unsigned int nThreads)
{
    os_threadpool_t *pool = malloc(sizeof(os_threadpool_t));
    DIE(pool == NULL, "grav ba");

    pthread_mutex_init(&pool->taskLock, NULL);

    pool->should_stop = 0;
    pool->num_threads = nThreads;
    pool->threads = malloc(nThreads * sizeof(pthread_t));
    DIE(pool->threads == NULL, "grav ba");

    pool->tasks = NULL;

    for(int i = 0; i < nThreads; i++)
        pthread_create(pool->threads+i, NULL, thread_loop_function, pool);

    return pool;
}

/* Loop function for threads */
void *thread_loop_function(void *args)
{
    os_threadpool_t *pool = args;
    os_task_t *current_task;
    while (!pool->should_stop) {
        current_task = get_task(pool);
		if(!current_task)
			continue;
        current_task->task(current_task->argument);

        free(current_task);
    }
    return NULL;
}

/* Stop the thread pool once a condition is met */
void threadpool_stop(os_threadpool_t *tp, int (*processingIsDone)(os_threadpool_t *))
{
    if(processingIsDone)
        while(!processingIsDone(tp));

    tp->should_stop=1;

    for(int thread_id = 0; thread_id < tp->num_threads; thread_id++)
	{
		pthread_join(tp->threads[thread_id], NULL);

	}

	// FREE
	free(tp->threads);
	free(tp);
}
