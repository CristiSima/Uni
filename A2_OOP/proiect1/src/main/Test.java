package main;

import common.Constants;
import ioutils.InputData;
import ioutils.Writer;
import simulation.Simulation;

import org.json.simple.JSONObject;

final class Test {
    private Test() { }

    public static void runTest(final String testname) {
        InputData inputData = new InputData(Constants.TEST_PATH + testname
                                            + Constants.FILE_EXTENSION);
        Writer writer = new Writer(Constants.OUTPUT_PATH + testname + Constants.FILE_EXTENSION);

        Simulation simulation = new Simulation(inputData);
        JSONObject result = simulation.runToCompletion();

        writer.write(result);
        writer.close();
    }

    public static void main(final String[] args) {
        runTest("19");
    }
}
