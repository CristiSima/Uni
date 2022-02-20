#include <iostream>
#include <fstream>

#include "task.h"

#define MAX_N 100

using namespace std;

class Task_retele: Task
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

    // O(n^2 k^2)
    void formulate_oracle_question()
    {
        ofstream sat_out("sat.cnf");

        sat_out<<"p cnf\n";

        // a
        // for every spot there must be a node
        for(int k=0;k<K;k++)
        {
            for(int i=1;i<=N;i++)
                sat_out<<k*N+i<<" ";
            sat_out<<"0\n";
        }

        // b
        // for every NON-edge (v,w) there can be NO pair of spots
        for(int v=1;v<=N;v++)
            for(int w=v+1;w<=N;w++)
                if(!matrix[v][w])
                {
                    for(int i=0;i<K;i++)
                        for(int j=0;j<K;j++)
                            if(i!=j)
                                sat_out<<-(i*N+v)<<" "<<-(j*N+w)<<" 0\n";
                }

        // c.1
        // 2 diferent spots can NOT have the node
        for(int v=1;v<=N;v++)
            for(int i=0;i<K;i++)
                for(int j=i+1;j<K;j++)
                    if(i!=j)
                        sat_out<<-(i*N+v)<<" "<<-(j*N+v)<<" 0\n";

        // c.2
        // one spot can NOT have 2 nodes
        for(int v=1;v<=N;v++)
            for(int w=v+1;w<=N;w++)
                for(int i=0;i<K;i++)
                        sat_out<<-(i*N+v)<<" "<<-(i*N+w)<<" 0\n";

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

            // for all available spots
            for(int k=0;k<K;k++)
                for(int i=1;i<=N;i++)
                {
                    // marks the used node
                    sat_in>>nr;
                    if(nr>0)
                        nodes[i]=1;
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
                if(nodes[i])
                    cout<<i<<" ";

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
    Task_retele task;
    task.solve();
    return 0;
}
