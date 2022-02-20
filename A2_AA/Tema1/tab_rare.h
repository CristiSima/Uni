// #include <iostream>
// #include <climits>
//
#define NO_TAB_RARE_MAIN 1

class tab_rare
{
    int N;
    int power;
    int **M;
public:
    tab_rare();
    tab_rare(int N, int A[]);
    int get(int x, int y);
    ~tab_rare();
};
