package ioutils;

import org.json.simple.JSONObject;
import org.json.simple.JSONArray;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import elements.AnualChange;
import elements.Child;
import elements.Gift;

import java.util.List;
import java.util.ArrayList;
import java.io.FileReader;
import java.io.IOException;

public class InputData {
    private int numberOfYears;
    private int santaBudget;

    private List<Child> children;
    private List<Gift> gifts;
    private List<AnualChange> annualChanges;

    public InputData(final String filepath) {
        System.out.println(filepath);
        JSONObject json;
        JSONParser jsonParser = new JSONParser();
        try {
            json = (JSONObject) jsonParser
             .parse(new FileReader(filepath));
        } catch (ParseException | IOException e) {
            e.printStackTrace();
            return;
        }
        this.numberOfYears = ((Long) json.get("numberOfYears")).intValue();
        this.santaBudget = ((Long) json.get("santaBudget")).intValue();

        this.children = new ArrayList();
        for (Object element:(JSONArray) ((JSONObject) json.get("initialData")).get("children")) {
            // System.out.println(element);
            this.children.add(new Child((JSONObject) element));
        }

        this.gifts = new ArrayList();
        for (Object element:(JSONArray) ((JSONObject) json.get("initialData"))
                                                            .get("santaGiftsList")) {
            // System.out.println(element);
            this.gifts.add(new Gift((JSONObject) element));
        }

        this.annualChanges = new ArrayList<>();
        for (Object anualChange:(JSONArray) json.get("annualChanges")) {
            this.annualChanges.add(new AnualChange((JSONObject) anualChange));
        }
    }

    public final int getNumberOfYears() {
        return this.numberOfYears;
    }

    public final int getSantaBudget() {
        return this.santaBudget;
    }

    public final List<Child> getChildren() {
        return this.children;
    }

    public final List<Gift> getGifts() {
        return this.gifts;
    }

    public final List<AnualChange> getAnnualChanges() {
        return this.annualChanges;
    }

}
