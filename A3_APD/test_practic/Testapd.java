import java.util.concurrent.SynchronousQueue;

class Worker extends Thread
{
    int id;
    int sleep_time;

    Worker(int id, int sleep_time)
    {
        this.id=id;
        this.sleep_time=sleep_time;
    }

    @Override
    public void run() {
        try {
            Thread.sleep(sleep_time*1000);
            Testapd.queue.put(id);
        } catch(Exception e) {
            Thread.currentThread().interrupt();
            System.err.println("Thread Interrupted");
        }
    }
}


class Notifier extends Thread
{
    @Override
    public void run() {
        for(int i=1;i<Testapd.nr_threads;i++)
            try {
                System.out.println("Thread-ul "+Testapd.queue.take()+" a terminat");
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.err.println("Thread Interrupted");
            }
    }
}


public class Testapd {
    public static int nr_threads;
    public static SynchronousQueue queue;

    public static void main(String[] args) {
        nr_threads=Integer.parseInt(args[0]);
        // System.out.println(nr_threads);
        Thread[] threads = new Thread[nr_threads];
        queue= new SynchronousQueue();

        threads[0]=new Notifier();
        for(int i=1;i<nr_threads;i++)
            threads[i]=new Worker(i, Integer.parseInt(args[i]));

        for(int i=0;i<nr_threads;i++)
            threads[i].start();

        try {
            for(int i=0;i<nr_threads;i++)
                threads[i].join();
        } catch(Exception e) {
        }
    }
}
