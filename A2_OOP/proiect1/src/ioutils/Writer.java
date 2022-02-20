package ioutils;

import org.json.simple.JSONObject;

import java.io.FileWriter;
import java.io.IOException;

public class Writer {
    private FileWriter file;

    public Writer(final String path) {
        try {
            this.file = new FileWriter(path);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * writes a JSONObject to file
     */
    public final void write(final JSONObject json) {
        try {
            this.file.write(json.toJSONString());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * closes the file
     */
    public final void close() {
        try {
            this.file.flush();
            this.file.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
