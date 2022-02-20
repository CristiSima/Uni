#include <fstream>

#include <climits>
#include <cmath>
#include <stack>

using namespace std;

#define MAX INT_MAX
#define MAX_B 8

int calc_cartesioan_tree_number(int N, int A[])
{
    stack<int> st;
    int rez=0;
    for(int i=0;i<N-1;i++)
    {
        st.push(A[i]);
        rez=(rez<<1)|1;
        while(!st.empty() && st.top()>A[i+1])
        {
            rez<<=1;
            st.pop();
        }
    }
    rez=(rez<<2)|2;
    st.push(A[N-1]);
    st.pop();

    while(!st.empty())
    {
        rez<<=1;
        st.pop();
    }

    return rez;
}

class SFH_block
{
    int N;
    int **M;
public:
    bool generated;
    SFH_block()
    {
        this->M=NULL;
        this->generated=0;
    }
    SFH_block(int N, int *A)
    {
        this->generated=1;
        this->N=N;
        this->M=new int*[N];

        int A_rev[N];
        for(int i=0;i<N;i++)
            A_rev[A[i]]=i;


        for(int i=0;i<N;i++)
        {
            this->M[i]=new int[N];
            this->M[i][i]=A[i];
            for(int j=i+1;j<N;j++)
                this->M[i][j]=min(this->M[i][j-1],A[j]);
        }

        for(int i=0;i<N;i++)
            for(int j=i;j<N;j++)
                this->M[i][j]=A_rev[this->M[i][j]];
    }
    int get(int x, int y)
    {
        if(x>y)
            return -1;
        return this->M[x][y];
    }

    int extra_destructor_call=0;
    ~SFH_block()
    {
        if(generated && !extra_destructor_call--)
        {
            for(int i=0;i<this->N;i++)
                delete[] this->M[i];
            delete[] this->M;
        }
    }
};

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

#define INTERNAL_RMQ tab_rare

class SFH
{
    int *A;
    int b;
    int blocks;
    int *block_id;
    int *block_minim;
    SFH_block *solutions;
    tab_rare blocks_RMQ;
    void generate_RMQs(int k)
    {
        static bool used[MAX_B]={0};
        static int generated[MAX_B]={0};
        if(k==b)
        {
            int id=calc_cartesioan_tree_number(b,generated);
            if(!this->solutions[id].generated)
            {
                SFH_block temp(b,generated);
                this->solutions[id]=temp;
                temp.extra_destructor_call=1;
            }
            return;
        }
        for(int i=0;i<b;i++)
            if(!used[i])
            {
                used[i]=1;
                generated[k]=i;
                generate_RMQs(k+1);
                used[i]=0;
            }
    }
public:
    SFH(int N, int A[])
    {
        this->b=ceil(log2(N)/4);

        this->A=new int[N+b];
        for(int i=0;i<N;i++)
            this->A[i]=A[i];

        for(int i=0;i<b;i++)
            this->A[N+i]=MAX;

        this->blocks=N/this->b;
        if(N%this->b!=0)
            this->blocks++;

        this->block_id=new int[blocks];
        this->block_minim=new int[blocks];

        for(int i=0;i<this->blocks;i++)
            this->block_id[i]=calc_cartesioan_tree_number(this->b,this->A+(this->b*i));

        this->solutions=new SFH_block[(int)pow(4,b)];

        generate_RMQs(0);

        for(int i=0;i<this->blocks;i++)
        {
            int id=this->solutions[this->block_id[i]].get(0,b-1);
            this->block_minim[i]=A[i*b+id];
        }

        tab_rare temp(blocks,block_minim);
        this->blocks_RMQ=temp;
        temp.extra_destructor_call=1;
    }
    int get(int x, int y)
    {
        int x_s=x/this->b;
        if(x%this->b)
        {
            x_s++;
        }
        int y_s=(y+1)/this->b;
        int rez=MAX;

        if(x_s>=y_s)
        {

            if(x==y)
            {
                x_s+=(x/this->b-(x_s-1));
                x%=this->b;
                int id=this->block_id[x_s-1];
                id=this->solutions[id].get(x,x);
                return this->A[b*(x_s-1)+id];
            }

            int min_s=min(x_s,y_s);
            if(min_s)
                x-=(min_s-1)*this->b;
            if(min_s)
                y-=(min_s-1)*this->b;

            if(x>=this->b)
            {

                x-=this->b;
                y-=this->b;
                if(x_s<=y_s)
                    x_s++;

                int id=this->block_id[x_s-1];
                id=this->solutions[id].get(x,y);
                return this->A[b*(x_s-1)+id];
            }

            if(y>=this->b)
            {
                if(y_s==0)
                    y_s++;
                int id=this->block_id[y_s-1];
                id=this->solutions[id].get(x,this->b-1);
                rez=this->A[b*(x_s-1)+id];

                id=this->block_id[y_s];
                id=this->solutions[id].get(0,y-this->b);
                rez=min(rez,this->A[b*(y_s)+id]);
            }
            else
            {
                if(y_s==0)
                    y_s++;
                int id=this->block_id[y_s-1];
                id=this->solutions[id].get(x,y);
                rez=this->A[b*(y_s-1)+id];
            }
            return rez;
        }

        rez=this->blocks_RMQ.get(x_s,y_s-1);

        if(x%this->b)
        {
            // is elemente inainte
            int id=this->block_id[x_s-1];
            id=this->solutions[id].get(x%this->b,b-1);
            rez=min(rez,this->A[b*(x_s-1)+id]);
        }

        if((y+1)%this->b!=0)
        {
            // is elemente inainte
            int id=this->block_id[y_s];
            id=this->solutions[id].get(0,(y)%this->b);
            rez=min(rez,this->A[b*(y_s)+id]);
        }

        return rez;
    }
    ~SFH()
    {
        delete[] this->A;
        delete[] this->block_id;
        delete[] this->block_minim;
        delete[] this->solutions;
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

    SFH rmq(N,A);

    int M;
    int x,y;
    for(in>>M;M;M--)
    {
        in>>x>>y;
        out<<rmq.get(x-1,y-1)<<"\n";
    }

}
