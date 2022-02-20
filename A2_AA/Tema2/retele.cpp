#include <iostream>
#include <fstream>

#define MAX_N 100

using namespace std;

ofstream sat_out("retele_sat.cnf");

void generate(int N, int K, int k=0, int i=1)
{
    static int V[MAX_N+1]={0};

    if(k>N-K)
    {
        for(int k=0;k<=N-K;k++)
            sat_out<<V[k]<<" ";
        sat_out<<"0\n";
        return;
    }

    for(;i<=N;i++)
    {
        V[k]=i;
        generate(N,K,k+1,i+1);
    }

}

int main() {
    int N,M,K;
    int matrix[MAX_N+1][MAX_N+1]={0};
    cin>>N>>M>>K;
    int a,b;
    for(int i=1;i<=M;i++)
    {
        cin>>a>>b;
        matrix[a][b]=matrix[b][a]=1;
    }

    for(int k=0;k<K;k++)
    {
        for(int i=1;i<=N;i++)
            sat_out<<k*N+i<<" ";
        sat_out<<"0\n";
    }

    // generate(N,K);
    for(int v=1;v<=N;v++)
        for(int w=v+1;w<=N;w++)
            if(!matrix[v][w])
            {
                for(int i=0;i<K;i++)
                    for(int j=0;j<K;j++)
                        if(i!=j)
                            sat_out<<-(i*N+v)<<" "<<-(j*N+w)<<" 0\n";
            }
    // for(int i=1;i<=N;i++)
    //     for(int k1=0;k1<K;k1++)
    //     {
    //         for(int k2=k1+1;k2<K;k2++)
    //             sat_out<<-(k1*N+i)<<" "<<-(k2*N+i)<<" 0\n";
    //     }
    return 0;
}
