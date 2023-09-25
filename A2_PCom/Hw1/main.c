
#include "stdio.h"
#include "queue.h"
#include "skel.h"

void temp2(queue b)
{
    queue c=queue_create();
    packet *p=queue_deq(b);
    printf("%d\n",p->interface);
    queue_enq(c, p);
    printf("%d\n",p->interface);
}

void hexdump(uint8_t *data, int len)
{
	for(int i=0;i<len;i++)
		printf("%02x ", data[i]);
	puts("");
}

int main()
{
    packet m;
    m.interface=1;
    queue a=queue_create();
    printf("%d\n", sizeof(queue));

    queue_enq(a,&m);
    // printf("%llx\n",*(long long*)a );
    hexdump(a,8);
    temp2(a);
    hexdump(a,8);
    // printf("%llx\n",*(long long*)a );

}
