#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <sstream>
#include <string>
#include "test_map.hpp"
#include "gpu_hashtable.hpp"
#include <cuda_runtime.h>

using namespace std;

/*
Allocate CUDA memory only through glbGpuAllocator
cudaMalloc -> glbGpuAllocator->_cudaMalloc
cudaMallocManaged -> glbGpuAllocator->_cudaMallocManaged
cudaFree -> glbGpuAllocator->_cudaFree
*/



__device__ inline int hash_key(int key)
{
	#define HASH_A     6700417	
	#define HASH_B  2147483647	

	return ((long long)key*HASH_A) % HASH_B;
}

__device__ void insert_entry(table_entry *table, int table_size, table_entry entry)
{
	int key_hash= hash_key(entry.key) % table_size;

	for(int seach_index=key_hash;;seach_index++)
	{
		if(seach_index==table_size) seach_index=0;

		if(table[seach_index].key == entry.key ||
			atomicCAS((unsigned int*)&table[seach_index].key, (unsigned int)EMPTY_ENTRY, (unsigned int)entry.key) == EMPTY_ENTRY )
		{
			table[seach_index].value=entry.value;
			return;
		}
	}
}

__global__ void kerne_migrate_table(table_entry *old_table, int old_size, table_entry *new_table, int new_size)
{
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	if(index>=old_size)
		return;

	table_entry entry=old_table[index];

	if(entry.key == EMPTY_ENTRY)
		return;

	insert_entry(new_table, new_size, entry);
}

__global__ void kerne_insert_batch(table_entry *device_table, int table_size, table_entry *entries, int num_entries)
{
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	index *= THREAD_CHUNK;

	for(int i=0; i<THREAD_CHUNK && index<num_entries; i++, index++)
		insert_entry(device_table, table_size, entries[index]);
}

__global__ void kernel_get_batch(table_entry *device_table, int table_size, table_entry *entries, int num_entries)
{
	int index = threadIdx.x + blockIdx.x * blockDim.x;

	if(index>=num_entries)
		return;

	int key = entries[index].key;
	int key_hash= hash_key(key) % table_size;

	for(int seach_index=key_hash;;seach_index++)
	{
		if(seach_index==table_size) seach_index=0;

		if(device_table[seach_index].key==key)
		{
			entries[index].value=device_table[seach_index].value;
			return;
		}
	}
}

/**
 * Function constructor GpuHashTable
 * Performs init
 * Example on using wrapper allocators _cudaMalloc and _cudaFree
 */
GpuHashTable::GpuHashTable(int size) {
	current_size=size;
	inserted_elements=0;

	glbGpuAllocator->_cudaMalloc((void**)&device_table, size * sizeof(table_entry));
	cudaMemset(device_table, 0xFF, size*sizeof(table_entry));
}

/**
 * Function desctructor GpuHashTable
 */
GpuHashTable::~GpuHashTable() {
	glbGpuAllocator->_cudaFree(device_table);
}

/**
 * Function reshape
 * Performs resize of the hashtable based on load factor
 */
void GpuHashTable::reshape(int numBucketsReshape) {
	table_entry *new_table;
	glbGpuAllocator->_cudaMalloc((void**)&new_table, numBucketsReshape * sizeof(table_entry));
	cudaMemset(new_table, 0xFF, numBucketsReshape*sizeof(table_entry));

	int blocks=current_size/MAX_BLOCK_SIZE;
	blocks+=(current_size%MAX_BLOCK_SIZE > 0) ? 1 :0;
	// call kernel
	// cudaError_t err=cudaGetLastError();
	// printf("%d %s\n", err, cudaGetErrorName(err));
	// exit(0);
	kerne_migrate_table<<<blocks, MAX_BLOCK_SIZE>>>(device_table, current_size, new_table, numBucketsReshape);
	cudaDeviceSynchronize();
	
	glbGpuAllocator->_cudaFree(device_table);
	device_table=new_table;
	current_size=numBucketsReshape;
	return;
}

/**
 * Function insertBatch
 * Inserts a batch of key:value, using GPU and wrapper allocators
 */
bool GpuHashTable::insertBatch(int *keys, int* values, int numKeys) {
	// check expand
	if(inserted_elements + numKeys >= current_size*0.9)
		reshape((inserted_elements + numKeys)* 1.75);
	
	int threads=numKeys/THREAD_CHUNK;
	threads+=(numKeys%THREAD_CHUNK > 0) ? 1 :0;

	int blocks=threads/MAX_BLOCK_SIZE;
	blocks+=(threads%MAX_BLOCK_SIZE > 0) ? 1 :0;
	
	table_entry *new_entries_d;
	glbGpuAllocator->_cudaMalloc((void**)&new_entries_d, numKeys * sizeof(table_entry));
	table_entry *new_entries_h = new table_entry[numKeys];
	for(int i=0;i<numKeys;i++)
	{
		new_entries_h[i].key=keys[i];
		new_entries_h[i].value=values[i];
	}
	cudaMemcpy(new_entries_d, new_entries_h, numKeys*sizeof(new_entries_h), cudaMemcpyHostToDevice);
	delete[] new_entries_h;

	kerne_insert_batch<<<blocks, MAX_BLOCK_SIZE>>>(device_table, current_size, new_entries_d, numKeys);
	cudaDeviceSynchronize();

	glbGpuAllocator->_cudaFree(new_entries_d);
	inserted_elements+=numKeys;
	return true;
}

/**
 * Function getBatch
 * Gets a batch of key:value, using GPU
 */
int* GpuHashTable::getBatch(int* keys, int numKeys) {
	int blocks=numKeys/MAX_BLOCK_SIZE;
	blocks+=(numKeys%MAX_BLOCK_SIZE > 0) ? 1 :0;
	
	table_entry *entries;
	glbGpuAllocator->_cudaMalloc((void**)&entries, numKeys * sizeof(table_entry));

	table_entry *new_entries_h = new table_entry[numKeys];
	for(int i=0; i<numKeys; i++)
		new_entries_h[i].key=keys[i];
	cudaError_t err=cudaGetLastError();
	cudaMemcpy(entries, new_entries_h, numKeys*sizeof(new_entries_h), cudaMemcpyHostToDevice);

	kernel_get_batch<<<blocks, MAX_BLOCK_SIZE>>>(device_table, current_size, entries, numKeys);
	cudaDeviceSynchronize();

	cudaMemcpy(new_entries_h, entries, numKeys*sizeof(new_entries_h), cudaMemcpyDeviceToHost);
	int *values=new int[numKeys];
	for(int i=0; i<numKeys; i++)
		values[i]=new_entries_h[i].value;

	int zeroes=0;
	for(int i=0; i<numKeys; i++)
		zeroes+=values[i]==0;

	delete[] new_entries_h;
	glbGpuAllocator->_cudaFree(entries);
	return values;
}
