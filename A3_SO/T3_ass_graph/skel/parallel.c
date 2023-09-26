#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#include "os_graph.h"
#include "os_threadpool.h"
#include "os_list.h"

#define MAX_TASK 100
#define MAX_THREAD 4

volatile int sum = 0;
// number of nodes unprocessed and in the queue
volatile int remaining_nodes=1;
os_graph_t *graph;
pthread_mutex_t sum_lock;
pthread_mutex_t remaining_lock;
pthread_mutex_t visit_lock;
pthread_mutex_t add_lock;
os_threadpool_t *pool;
// flag vector for nodes added to the queue
int *added;

void process_wraper(void *arg);

void processNode(struct os_node_t *target)
{
    // add value
    pthread_mutex_lock(&visit_lock);
    if(graph->visited[target->nodeID])
    {
        pthread_mutex_unlock(&visit_lock);
        return;
    }
    graph->visited[target->nodeID]=1;
    pthread_mutex_unlock(&visit_lock);


    int nodeInfo=target->nodeInfo;
    pthread_mutex_lock(&sum_lock);
    sum+=nodeInfo;
    pthread_mutex_unlock(&sum_lock);

    for (int i = 0; i < target->cNeighbours; i++)
    {
        int next_id=target->neighbours[i];

        pthread_mutex_lock(&add_lock);

        // check for real
        if(added[next_id])
        {
            pthread_mutex_unlock(&add_lock);
            continue;
        }

        added[next_id]++;
        pthread_mutex_unlock(&add_lock);


        pthread_mutex_lock(&remaining_lock);
        remaining_nodes++;
        pthread_mutex_unlock(&remaining_lock);

        add_task_in_queue(pool, task_create(graph->nodes[next_id], process_wraper));
    }
}

pthread_mutex_t finished_signal_lock;
volatile int last_start=0;

void process_wraper(void *arg)
{
    processNode(arg);

    pthread_mutex_lock(&remaining_lock);
    remaining_nodes--;
    pthread_mutex_unlock(&remaining_lock);

    if(remaining_nodes > 0)
        return;

    last_start++;

    if(last_start >= graph->nCount)
    {
        // if it doesn't exist end the pool
        pthread_mutex_unlock(&finished_signal_lock);
        return;
    }

    remaining_nodes=1;
    add_task_in_queue(pool, task_create(graph->nodes[last_start], process_wraper));
}

int check_finished(os_threadpool_t *pool)
{
    pthread_mutex_lock(&finished_signal_lock);

    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./main input_file\n");
        exit(1);
    }

    FILE *input_file = fopen(argv[1], "r");

    if (input_file == NULL) {
        printf("[Error] Can't open file\n");
        return -1;
    }

    graph = create_graph_from_file(input_file);
    if (graph == NULL) {
        printf("[Error] Can't read the graph from file\n");
        return -1;
    }

    pthread_mutex_init(&sum_lock, NULL);
    pthread_mutex_init(&visit_lock, NULL);
    pthread_mutex_init(&remaining_lock, NULL);
    pthread_mutex_init(&add_lock, NULL);
    pthread_mutex_init(&finished_signal_lock, NULL);

    added = calloc(sizeof(*added), graph->nCount);

    pthread_mutex_lock(&finished_signal_lock);
    pool=threadpool_create(MAX_TASK, MAX_THREAD);

    add_task_in_queue(pool, task_create(graph->nodes[0], process_wraper));
    threadpool_stop(pool, check_finished);

    printf("%d", sum);
    fflush(stdout);
    return 0;
}
