package shortestPathsFloyd_Warshall;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.BrokenBarrierException;

class MyThread extends Thread {
    int[][] graph;
    int N;
    int start, end;
    CyclicBarrier barrier;

    public MyThread(int[][] graph, int N, int start, int end, CyclicBarrier barrier)
    {
        this.graph=graph;
        this.N=N;
        this.start=start;
        this.end=end;
        this.barrier=barrier;
    }

    @Override
    public void run() {

        for (int k = 0; k < 5; k++) {
            for (int i = 0; i < 5; i++) {
                for (int j = start; j < end; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
                try {
                    barrier.await();
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                } catch (BrokenBarrierException ex) {
                    ex.printStackTrace();
                }
            }
        }
    }
}

public class Main {

    public static void main(String[] args) {
        int N = 5;
        int M = 9;
        int P = 5;
        int[][] graph = {
            {0, 1, M, M, M},
            {1, 0, 1, M, M},
            {M, 1, 0, 1, 1},
            {M, M, 1, 0, M},
            {M, M, 1, M, 0}
        };
        int[][] real_ans={
            {0, 1, 2, 3, 3},
            {1, 0, 1, 2, 2},
            {2, 1, 0, 1, 1},
            {3, 2, 1, 0, 2},
            {3, 2, 1, 2, 0}
        };
        // Object locks[]=new Object[M];
        //
        // for(int i=0;i<M;i++)
        //     locks[i]=new Object();
        CyclicBarrier barrier=new CyclicBarrier(P);

        // Parallelize me (You might want to keep the original code in order to compare)
        // for (int k = 0; k < 5; k++) {
        //     for (int i = 0; i < 5; i++) {
        //         for (int j = 0; j < 5; j++) {
        //             graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
        //         }
        //     }
        // }
        Thread[] threads=new Thread[P];
        for(int i=0;i<P;i++)
        {
            int start = (int)(i * (double)N / P);
            int end = Math.min(((int)((i + 1) * (double)N / P)), N);
            threads[i]=new MyThread(graph, N, start, end, barrier);
            threads[i].start();
        }

        for(int i=0;i<P;i++)
            try {
                threads[i].join();
            } catch(Exception e) {
                e.printStackTrace();
            }


        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                System.out.print(graph[i][j] + " ");
            }
            System.out.println();
        }

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if(graph[i][j]!=real_ans[i][j])
                {
                    System.out.println("Wrongest at "+i+j);
                    return ;
                }
            }
        }
    }
}
