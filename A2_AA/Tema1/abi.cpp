#include <fstream>
#include <climits>

using namespace std;

#define MAX INT_MAX

class abi
{
    int N;
    int *V;
    int nr;

    int __get(int x, int y, int start, int end, int index)
    {
        int mid=(start+end)>>1;
        int rez=MAX;

        if(x<=start && end<=y)
            return this->V[index];

        if(x<=mid)
            rez=__get(x,y,start,mid,index<<1);
        if(mid<y)
            rez=min(rez,__get(x,y,mid+1,end,index<<1|1));
        return rez;
    }

public:
    abi(int N, int A[])
    {
        this->N=N;
        while((N&(N-1)) !=0)
            N&=N-1;

        int offset=N<<1;
        if(this->N==N)
            offset>>=1;
        N=offset<<1;
        this->nr=N;

        this->V=new int[N];
        for(int i=0;i<this->N;i++)
            this->V[offset+i]=A[i];
        for(int i=this->N;i<offset;i++)
            this->V[offset+i]=MAX;

        for(int i=offset-1;i;i--)
            this->V[i]=min(this->V[i<<1],this->V[i<<1|1]);

        this->N=offset;
    }

    int get(int x, int y)
    {
        return __get(x, y, 1, this->N, 1);
    }

    void set(int index, int nr)
    {
        int offset=this->nr>>1;
        index+=offset;
        this->V[index]=nr;
        index>>=1;
        int new_min=min(this->V[index<<1],this->V[index<<1|1]);
        while(new_min!=this->V[index])
        {
            this->V[index]=new_min;
            index>>=1;
            if(index)
                new_min=min(this->V[index<<1],this->V[index<<1|1]);
            else
                break;
        }
    }
    ~ abi()
    {
        delete[] this->V;
    }
};

int main()
{
    ifstream in("test.in");
    ofstream out("test.out");

    int N;
    in>>N;
    int A[N+1];
    for(int i=0;i<N;i++)
        in>>A[i];
    abi rmq(N,A);

    int M;
    int x,y;
    for(in>>M;M;M--)
    {
        in>>x>>y;
        out<<rmq.get(x,y)<<"\n";
    }
}
