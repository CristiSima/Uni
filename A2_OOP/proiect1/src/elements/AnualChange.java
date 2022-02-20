package elements;

import org.json.simple.JSONObject;
import org.json.simple.JSONArray;
import enums.Category;

import java.util.List;
import java.util.ArrayList;

public class AnualChange {
    private int newSantaBudget;
    private List<Gift> newGifts;
    private List<Child> newChildren;
    private List<ChildUpdate> childrenUpdates;

    public AnualChange(final JSONObject json) {
        this.newSantaBudget = ((Long) json.get("newSantaBudget")).intValue();

        // add gifts
        this.newGifts = new ArrayList();
        for (Object gift:(JSONArray) json.get("newGifts")) {
            this.newGifts.add(new Gift((JSONObject) gift));
        }

        // add children
        this.newChildren = new ArrayList();
        for (Object child:(JSONArray) json.get("newChildren")) {
            this.newChildren.add(new Child((JSONObject) child));
        }

        // add childrenUpdates
        this.childrenUpdates = new ArrayList();
        for (Object childUpdate:(JSONArray) json.get("childrenUpdates")) {
            this.childrenUpdates.add(new ChildUpdate((JSONObject) childUpdate));
        }
    }

    public final int getNewSantaBudget() {
        return this.newSantaBudget;
    }

    public final List<Gift> getNewGifts() {
        return this.newGifts;
    }

    public final List<Child> getNewChildren() {
        return this.newChildren;
    }

    public final List<ChildUpdate> getChildrenUpdates() {
        return this.childrenUpdates;
    }


    public static class ChildUpdate {
        private int id;
        private double niceScore;
        private List<Category> giftsPreferences;

        public ChildUpdate(final JSONObject json) {
            this.id = ((Long) json.get("id")).intValue();

            // checks if the niceScore is updated
            if (json.get("niceScore") == null) {
                this.niceScore = -1;
            } else {
                this.niceScore = Double.parseDouble(json.get("niceScore").toString());
            }

            this.giftsPreferences = new ArrayList<>();
            for (Object category:(JSONArray) json.get("giftsPreferences")) {
                this.giftsPreferences.add(Category.getCategory((String) category));
            }
        }

        public final int getId() {
            return this.id;
        }

        public final double getNiceScore() {
            return this.niceScore;
        }

        public final List<Category> getGiftsPreferences() {
            return this.giftsPreferences;
        }

    }
}
