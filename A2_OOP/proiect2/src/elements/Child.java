package elements;

import org.json.simple.JSONObject;
import org.json.simple.JSONArray;
import enums.Category;
import enums.ElvesType;
import enums.Cities;
import common.Constants;
import strategies.AverageNiceScore;


import java.util.List;
import java.util.ArrayList;
import java.util.stream.Collectors;

public class Child {
    private int id;
    private String lastName;
    private String firstName;
    private int age;
    private Cities city;
    private double niceScore;
    private double niceScoreBonus;
    private List<Double> oldScores;
    private List<Category> giftsPreferences;
    private ElvesType elf;

    public Child(final JSONObject json) {
        this.id = ((Long) json.get("id")).intValue();
        this.lastName = (String) json.get("lastName");
        this.firstName = (String) json.get("firstName");
        this.age = ((Long) json.get("age")).intValue();
        this.city = Cities.getCity((String) json.get("city"));
        this.niceScore = Double.parseDouble(json.get("niceScore").toString());
        this.niceScoreBonus = Double.parseDouble(json.get("niceScoreBonus").toString());

        this.giftsPreferences = new ArrayList<>();
        for (Object category:(JSONArray) json.get("giftsPreferences")) {
            this.giftsPreferences.add(Category.getCategory((String) category));
        }

        this.elf = ElvesType.getElf((String) json.get("elf"));

        this.oldScores = new ArrayList<>();
        this.oldScores.add(this.niceScore);

        this.calcAverageNiceScore();
    }

    /**
     * assembles a JSONObject from this
     */
    public final JSONObject toJSONObject() {
        JSONObject json = new JSONObject();

        json.put("id", this.id);
        json.put("lastName", this.lastName);
        json.put("firstName", this.firstName);
        json.put("city", this.city.toString());
        json.put("age", this.age);
        json.put("giftsPreferences", this.giftsPreferences.stream()
         .map((category) -> category.toString())
         .collect(Collectors.toList()));
        json.put("averageScore", this.getAverageNiceScore());
        json.put("niceScoreHistory", new ArrayList(this.oldScores));

        return json;
    }

    /**
     * returns the score, using the bonus
     */
    public final double getAverageNiceScore() {
        double finalNiceScore = this.niceScore + this.niceScore * this.niceScoreBonus
                                                 / Constants.PROC_100;
        if (finalNiceScore > Constants.CHILD_MAX_SCORE) {
            return Constants.CHILD_MAX_SCORE;
        }
        return finalNiceScore;
    }

    /**
     * calculates (average)niceScore using past scores
     */
    public final void calcAverageNiceScore() {
        this.niceScore = AverageNiceScore.getAverageNiceScore(this);
    }

    /**
     * adds a score and recalculate niceScore
     */
    public final void addScore(final double score) {
        this.oldScores.add(score);
        this.calcAverageNiceScore();
    }

    public final int getAge() {
        return this.age;
    }

    public final ElvesType getElf() {
        return this.elf;
    }

    public final Cities getCity() {
        return this.city;
    }

    public final List<Double> getOldScores() {
        return this.oldScores;
    }

    public final int getId() {
        return this.id;
    }

    public final List<Category> getGiftsPreferences() {
        return this.giftsPreferences;
    }

    /**
     * increses age and recalculate niceScore
     */
    public final void incAge() {
        this.age++;
        this.calcAverageNiceScore();
    }

    public final void setElf(final ElvesType newElf) {
        this.elf = newElf;
    }
}
