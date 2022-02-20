#include <fstream>
#include <climits>

using namespace std;

#define MAX INT_MAX

class tab_rare
{
    int N;
    int power;
    int **M;
public:
    tab_rare()
    {
        this->N=0;
        this->power=0;
        this->M=NULL;
    }
    tab_rare(int N, int A[])
    {
        // tab_rare *p=this;
        this->N=N;
        while((N&(N-1)) !=0)
            N&=N-1;
        N<<=1;
        int power=0;
        while(N!=1)
        {
            N>>=1;
            power++;
        }
        this->power=power;
        // M il contine pe A
        this->M=new int*[power+1];
        M[power]=NULL;
        M[power-1]=NULL;

        this->M[0]=new int[this->N];
        for(int i=0;i<this->N;i++)
            this->M[0][i]=A[i];
        for(int i=0;(1<<(i+1))-1<this->N;i++)
        {
            this->M[i+1]=new int[this->N-(1<<(i))+1];

            for(int j=0;j+(1<<(i+1))<=this->N;j++)
                this->M[i+1][j]=min(this->M[i][j],this->M[i][j+(1<<(i))]);
        }
    }
    int get(int x, int y)
    {
        if(y>=N)
            y=N-1;
        int dif=y-x+1;

        if(!dif)
            return this->M[0][x];

        while((dif&(dif-1)) !=0)
            dif&=dif-1;

        int power=0;
        while(dif!=1)
        {
            dif>>=1;
            power++;
        }

        if((1<<power)==y-x+1)
            return this->M[power][x];
        else
            return min(this->M[power][x],this->M[power][y-(1<<power)+1]);
    }
    int extra_destructor_call=0;
    ~tab_rare()
    {
        if(!extra_destructor_call--)
        {
            for(int i=0;i<=this->power;i++)
                if(this->M[i])
                    delete[] this->M[i];
            delete[] this->M;
        }
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
    tab_rare rmq(N,A);


    int M;
    int x,y;
    for(in>>M;M;M--)
    {
        in>>x>>y;
        out<<rmq.get(x-1,y-1)<<"\n";
    }
}
