package doubleVectorElements;

class MyThread extends Thread {
    int start, end;
    int[] v;
    public MyThread(int start, int end, int[] v)
    {
        this.start=start;
        this.end=end;
        this.v=v;
    }

    @Override
    public void run() {
        for(int i=start;i<end;i++)
            v[i]*=2;
    }
}

public class Main {

    public static void main(String[] args) {
        int N = 100000013;
        int[] v = new int[N];
        int P = 4; // the program should work for any P <= N
        Thread[] threads=new Thread[P];
        for (int i = 0; i < N; i++) {
            v[i] = i;
        }

        // Parallelize me using P threads
        // for (int i = 0; i < N; i++) {
        //     v[i] = v[i] * 2;
        // }
        for(int i=0;i<P;i++)
        {
            int start = (int)(i * (double)N / P);
            int end = Math.min(((int)((i + 1) * (double)N / P)), N);
            threads[i]=new MyThread(start, end, v);
            threads[i].start();
        }

        for(int i=0;i<P;i++)
            try {
                threads[i].join();
            } catch(Exception e) {
                e.printStackTrace();
            }


        for (int i = 0; i < N; i++) {
            if (v[i] != i * 2) {
                System.out.println("Wrong answer");
                System.exit(1);
            }
        }
        System.out.println("Correct");
    }

}
