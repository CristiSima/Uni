package barber;

public class Client extends Thread {
    private final int id;

    public Client(int id) {
        super();
        this.id = id;
    }

    @Override
    public void run() {
        // TODO
        try {Main.
            semChairs.acquire();
        } catch (Exception e) {}

        if (Main.chairs > 0) {
            // client occupies a seat
            Main.chairs--;

            System.out.println("Client " + id + " is waiting for haircut");
            System.out.println("Available seats: " + Main.chairs);

            // TODO
            Main.semClients.release();
            Main.semChairs.release();
            try {
                Main.semBarberReady.acquire();
            } catch (Exception e) {}

            System.out.println("Client " + id + " is served by the barber");

            Main.leftClients[id] = Main.SERVED_CLIENT;
        } else {
            // TODO
            Main.semChairs.release();
            System.out.println("Client " + id + " left unserved");
            Main.leftClients[id] = Main.UNSERVED_CLIENT;
        }
    }
}
