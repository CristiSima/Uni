package lab13.task1;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

public class Main {
    private static void readAndPrintLine() {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        // TODO: Read a line from stdin and print it to stdout
        try
        {
            System.out.println(reader.readLine());
            reader.close();
        }
        catch(IOException e)
        {

        }
        // TODO: Don't forget to close the reader (Hint: try-with-resources, try-finally)
    }

    public static void main(String[] args) {
        readAndPrintLine();
    }
}
