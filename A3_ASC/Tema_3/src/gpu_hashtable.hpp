#ifndef _HASHCPU_
#define _HASHCPU_

#define MAX_BLOCK_SIZE 512
#define THREAD_CHUNK 16
#define EMPTY_ENTRY 0xFFFFFFFF

typedef struct {
	int key;
	int value;
} table_entry;

/**
 * Class GpuHashTable to implement functions
 */
class GpuHashTable
{
	public:
		GpuHashTable(int size);
		void reshape(int sizeReshape);

		bool insertBatch(int *keys, int* values, int numKeys);
		int* getBatch(int* key, int numItems);

		~GpuHashTable();
	
	table_entry *device_table;
	int current_size;
	int inserted_elements;
};

#endif
