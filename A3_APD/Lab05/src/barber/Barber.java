package barber;

public class Barber extends Thread {
    @Override
    public void run() {
        int servedClients = 0;

        do {
            // TODO
            try {
                Main.semClients.acquire();
                System.out.println("Barber acquired semClients");
                Main.semChairs.acquire();
                System.out.println("Barber acquired semChairs");
            } catch (Exception e) {}

            Main.chairs++;

            // TODO

            Main.semChairs.release();
            Main.semBarberReady.release();

            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            System.out.println("Barber served client");
            ++servedClients;

        } while (checkIfThereAnyClients());
    }

    private boolean checkIfThereAnyClients() {
        int count = 0;
        for (var client: Main.leftClients) {
            if (client == 0) {
                count++;
            }
        }

        return count != 0;
    }
}
