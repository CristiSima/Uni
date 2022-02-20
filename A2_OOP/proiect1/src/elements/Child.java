package elements;

import org.json.simple.JSONObject;
import org.json.simple.JSONArray;
import enums.Category;
import common.Constants;

import java.util.List;
import java.util.ArrayList;
import java.util.stream.Collectors;

public class Child {
    private int id;
    private String lastName;
    private String firstName;
    private int age;
    private String city;
    private double niceScore;
    private List<Double> oldScores;
    private List<Category> giftsPreferences;

    public Child(final JSONObject json) {
        this.id = ((Long) json.get("id")).intValue();
        this.lastName = (String) json.get("lastName");
        this.firstName = (String) json.get("firstName");
        this.age = ((Long) json.get("age")).intValue();
        this.city = (String) json.get("city");
        this.niceScore = Double.parseDouble(json.get("niceScore").toString());

        this.giftsPreferences = new ArrayList<>();
        for (Object category:(JSONArray) json.get("giftsPreferences")) {
            this.giftsPreferences.add(Category.getCategory((String) category));
        }

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
        json.put("city", this.city);
        json.put("age", this.age);
        json.put("giftsPreferences", this.giftsPreferences.stream()
         .map((category) -> category.toString())
         .collect(Collectors.toList()));
        json.put("averageScore", this.getAverageNiceScore());
        json.put("niceScoreHistory", new ArrayList(this.oldScores));

        return json;
    }

    public final double getAverageNiceScore() {
        return this.niceScore;
    }

    /**
     * calculates (average)niceScore using past scores
     */
    public final void calcAverageNiceScore() {
        if (this.age < Constants.CHILD_AGETRESHOLD_KID) {
            // baby
            this.niceScore = Constants.CHILD_KID_SCORE;
        } else if (this.age < Constants.CHILD_AGETRESHOLD_TEEN)                        {
            // kid
            this.niceScore = this.oldScores.stream().reduce(0d, Double::sum)
                                                    / ((double) this.oldScores.size());
        } else if (this.age <= Constants.CHILD_AGETRESHOLD_YOUNGADULT)                        {
            // teen
            double sum = 0;
            for (int i = 0; i < this.oldScores.size(); i++) {
                sum += (i + 1) * this.oldScores.get(i);
            }
            this.niceScore = sum / ((double) (this.oldScores.size()
                                    * (this.oldScores.size() + 1)) / 2);
        } else         {
            // young adult
            this.niceScore = Constants.CHILD_YOUNGADULT_SCORE;
        }
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
}
