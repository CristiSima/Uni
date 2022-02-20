#include <iostream>
#include <fstream>

#include "task.h"

#define MAX_N 100

using namespace std;

class Task_registre: Task
{
    int N, M, K;
    int matrix[MAX_N+1][MAX_N+1]={0};
    char result;
    int nodes[MAX_N+1]={0};

public:
    void read_problem_data()
    {
        cin>>N>>M>>K;

        int a,b;
        for(int i=1;i<=M;i++)
        {
            cin>>a>>b;
            matrix[a][b]=matrix[b][a]=1;
        }
    }

    // O(n^2 k)
    void formulate_oracle_question()
    {
        ofstream sat_out("sat.cnf");

        sat_out<<"p cnf\n";

        // a
        // for every node there must be a spot
        for(int i=1;i<=N;i++)
        {
            for(int k=0;k<K;k++)
                sat_out<<k*N+i<<" ";
            sat_out<<"0\n";
        }

        // b
        // for every edge the nodes can't use the same spot
        // can be reduced to m<=n^2
        for(int v=1;v<=N;v++)
            for(int w=v+1;w<=N;w++)
                if(matrix[v][w])
                    for(int i=0;i<K;i++)
                        sat_out<<-(i*N+v)<<" "<<-(i*N+w)<<" 0\n";

        // c.1
        // 2 diferent spots can NOT have the node
        for(int v=1;v<=N;v++)
            for(int i=0;i<K;i++)
                for(int j=i+1;j<K;j++)
                    if(i!=j)
                        sat_out<<-(i*N+v)<<" "<<-(j*N+v)<<" 0\n";

        sat_out.close();
    }

    void decipher_oracle_answer()
    {
        ifstream sat_in("sat.sol");
        char c;

        sat_in>>result;

        if(result=='T')
        {
            sat_in>>c>>c>>c;

            int nr;
            sat_in>>nr;
            // for all available registers
            for(int k=0;k<K;k++)
                for(int i=1;i<=N;i++)
                {
                    // see what variables use it
                    sat_in>>nr;
                    if(nr>0)
                        nodes[i]=k+1;
                }
        }
        sat_in.close();
    }

    void write_answer()
    {
        if(result=='T')
        {
            cout<<"True\n";

            for(int i=1;i<=N;i++)
                cout<<nodes[i]<<" ";

            cout<<"\n";
        }
        else
        {
            cout<<"False\n";
        }
    }

    void solve()
    {
        read_problem_data();
        formulate_oracle_question();
        ask_oracle();
        decipher_oracle_answer();
        write_answer();
    }
};

int main() {
    Task_registre task;
    task.solve();
    return 0;
}
