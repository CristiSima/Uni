#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <math.h>
#include "linked_list.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

void *int_2_pointer(int target)
{
	void *p=NULL;
	*(int*)&p=target;
	return p;
}

#define ll_add_end_int(list, data)  ll_add_end(list, int_2_pointer(data))

// used as a dummy for ll_free
void none(void *arg){}

struct job_info
{
	int no_mappers, no_reducers;
	linked_list **mappers_output;

	FILE *filenames;
	int no_files, opened_files;

	int thread_id;

	pthread_barrier_t *sync_mapper_reducer;

};

int check_mem(void *buff, size_t size, uint8_t target)
{
	for(size_t i=0;i<size;i++)
		if(((uint8_t*)buff)[i]!=target)
			return 0;

	return 1;
}

FILE *get_next_file(FILE *filenames, int no_files, int *opened_files)
{
	static pthread_mutex_t aquisition_mutex={0};
	if(check_mem(&aquisition_mutex, sizeof(aquisition_mutex), 0))
		pthread_mutex_init(&aquisition_mutex, NULL);

	// for initialization
	if(!filenames)
		return NULL;

	FILE *result=NULL;
	int rez=-1;
	char *line=NULL;
	size_t size=0;

	pthread_mutex_lock(&aquisition_mutex);

	if(opened_files[0]<no_files)
	{
		rez=getline(&line, &size, filenames);
		opened_files[0]++;
	}

	pthread_mutex_unlock(&aquisition_mutex);

	if(rez!=-1)
	{
		if(line[rez-1]=='\n')
			line[rez-1]=0;

		result=fopen(line, "r");
	}
	free(line);

	return result;
}


void* mapper(void *arg)
{
	struct job_info *job_info=arg;
	int n, nr, org;
	int temp;
	unsigned char can_be_power_of[1+2+job_info->no_reducers];
	unsigned char can_be_power;

	// consume all files
	for(FILE *file=get_next_file(job_info->filenames, job_info->no_files, &job_info->opened_files);
		file;
		file=get_next_file(job_info->filenames, job_info->no_files, &job_info->opened_files))
	{
		fscanf(file, "%d", &n);
		// consume all numbers in the file
		for(int i=0;i<n;i++)
		{
			fscanf(file, "%d\n", &nr);
			org=nr;

			if(nr==0)
				continue;

			// init
			int factor_power=0;
			memset(can_be_power_of, 0, 3+job_info->no_reducers);
			can_be_power=0;

			// check for power of 2
			while((nr&1)==0)
			{
				factor_power++;
				nr>>=1;
			}
			// mark powers that are still valid
			for(int power=2; power-2<job_info->no_reducers ;power++)
				if(factor_power%power==0)
					can_be_power=can_be_power_of[power]=1;

			// search for factors of nr
			for(int factor=3; can_be_power && factor*factor<=nr; factor+=2) if(nr%factor==0)
			{
				int factor_power=0;
				can_be_power=0;

				// count power of factor
				while(nr%factor==0)
				{
					factor_power++;
					nr/=factor;
				}

				// mark powers that are still valid
				for(int power=2; power-2<job_info->no_reducers ;power++)
					if(can_be_power_of[power])
					{
						temp=((factor_power%power)==0);
						can_be_power|=temp;
						can_be_power_of[power]=temp;
					}

				// check if any power is still achiveable
				if(!can_be_power)
					break;
			}

			// check if the number can be a power of any number
			if(nr==1 && can_be_power)
				// find the valid powers
				for(int power=2; power-2<job_info->no_reducers;power++)
					if(can_be_power_of[power])
						// add the number to the apropriate output list
						ll_add_end_int(job_info->mappers_output[job_info->thread_id][power-2], org);
		}
		fclose(file);
	}

	pthread_barrier_wait(job_info->sync_mapper_reducer);
	return NULL;
}

void *reducer(void *arg)
{
	struct job_info *job_info=arg;
	// wait for all mappers
	pthread_barrier_wait(job_info->sync_mapper_reducer);

	linked_list checked=ll_create();
	int found=0;
	int unique_elements=0;

	// iterate over all mappers
	for(int mapper_id=0;mapper_id<job_info->no_mappers;mapper_id++)
		// iterate over relevant mapper output
		ll_iterator_def(job_info->mappers_output[mapper_id][job_info->thread_id], mapped)
		{
			found=0;

			// check if the numer was found
			ll_iterator_def(checked, checked_iterator)
				if(checked_iterator->data==mapped->data)
				{
					found=1;
					break;
				}

			if(found)
				continue;

			// add the number
			unique_elements++;
			ll_add_end(checked, mapped->data);
		}

	ll_free(checked, none);

	// contruct file name
	char out_file_name[100];
	sprintf(out_file_name, "out%d.txt", job_info->thread_id+2);

	// output to file
	FILE *out_fd=fopen(out_file_name, "w");
	fprintf(out_fd, "%d", unique_elements);
	fclose(out_fd);

	return NULL;
}



int main(int argc, char **argv)
{
	if(argc<=3)
	{
		printf("Use: %s <M> <R> <file>\n", argv[0]);
		exit(1);
	}

	// init static lock
	get_next_file(NULL, 0, NULL);

	// populate job_info
	struct job_info info;
	info.no_mappers=atoi(argv[1]);
	info.no_reducers=atoi(argv[2]);
	info.filenames=fopen(argv[3], "r");

	info.sync_mapper_reducer=malloc(sizeof(pthread_barrier_t));
	pthread_barrier_init(info.sync_mapper_reducer, NULL, info.no_mappers+info.no_reducers);


	info.opened_files=0;
	fscanf(info.filenames, "%d\n", &info.no_files);

	// alloc mappers output
	info.mappers_output=malloc(info.no_mappers*sizeof(linked_list*));
	for(int i=0; i<info.no_mappers; i++)
	{
		info.mappers_output[i]=malloc(info.no_reducers*sizeof(linked_list));
		for(int j=0; j<info.no_reducers; j++)
			info.mappers_output[i][j]=ll_create();
	}

	pthread_t  mappers[info.no_mappers];
	pthread_t reducers[info.no_reducers];

	int maximum=max(info.no_mappers, info.no_reducers);
	struct job_info infos[maximum];
	for(int i=0;i<maximum;i++)
	{
		infos[i]=info;
		infos[i].thread_id=i;
	}

	// start threads
	for(int i=0; i<info.no_mappers;i++)
		pthread_create(mappers+i,  NULL, mapper,  infos+i);
	for(int i=0; i<info.no_reducers;i++)
		pthread_create(reducers+i, NULL, reducer, infos+i);

	void *ret;

	// wait for threads
	for(int i=0; i<info.no_mappers;i++)
		pthread_join(mappers[i], &ret);
	for(int i=0; i<info.no_reducers;i++)
		pthread_join(reducers[i], &ret);


	// free job_info
	for(int i=0; i<info.no_mappers; i++)
		for(int j=0; j<info.no_reducers; j++)
			ll_free(info.mappers_output[i][j], none);
	for(int i=0; i<info.no_mappers; i++)
		free(info.mappers_output[i]);
	free(info.mappers_output);

	fclose(info.filenames);
	return 0;
}
