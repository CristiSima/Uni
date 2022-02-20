package simulation;

import ioutils.InputData;
import elements.AnualChange;
import elements.Child;
import elements.Gift;
import common.Constants;
import enums.Category;

import org.json.simple.JSONObject;
import org.json.simple.JSONArray;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

public class Simulation {
    private int numberOfYears;
    private int curentYear;
    private int santaBudget;

    private List<Child> children;
    private Map<Category, List<Gift>> gifts;
    private List<AnualChange> annualChanges;

    public Simulation(final InputData inputData) {
        this.numberOfYears = inputData.getNumberOfYears();
        this.santaBudget = inputData.getSantaBudget();
        this.children = new ArrayList(inputData.getChildren().stream()
                                        .filter((child) -> child.getAge()
                                                        <= Constants.CHILD_AGETRESHOLD_YOUNGADULT)
                                        .toList());
        this.annualChanges = inputData.getAnnualChanges();

        this.curentYear = 0;
        this.gifts = new HashMap();

        inputData.getGifts().forEach(this::addGift);
        this.sortGifts();


        this.curentYear = 0;
    }

    /**
     * adds a new gift
     */
    private void addGift(final Gift gift) {
        if (!this.gifts.containsKey(gift.getCategory())) {
            this.gifts.put(gift.getCategory(), new ArrayList());
        }
        this.gifts.get(gift.getCategory()).add(gift);
    }


    /**
     * sorts gifts
     */
    private void sortGifts() {
        for (List<Gift> giftList: this.gifts.values()) {
            giftList.sort((g1, g2) -> Double.compare(g1.getPrice(), g2.getPrice()));
        }
    }

    /**
     * distributes gifts
     */
    public final JSONObject getGifts() {
        JSONArray result = new JSONArray();

        double sumAverageScore = 0;
        for (Child child: this.children) {
            sumAverageScore += child.getAverageNiceScore();
        }

        double budgetUnit = ((double) this.santaBudget) / sumAverageScore;

        for (Child child:this.children) {
            double assignedBudget = budgetUnit * child.getAverageNiceScore();
            JSONObject obj = new JSONObject();
            obj = child.toJSONObject();

            obj.put("assignedBudget", assignedBudget);
            JSONArray receivedGifts = new JSONArray();

            for (Category category: child.getGiftsPreferences()) {
                if (!this.gifts.containsKey(category)) {
                    continue;
                }

                for (Gift gift: this.gifts.get(category)) {
                    if (gift.getPrice() < assignedBudget) {
                        assignedBudget -= gift.getPrice();
                        receivedGifts.add(gift.toJSONObject());
                        break;
                    }
                }
            }

            obj.put("receivedGifts", receivedGifts);
            result.add(obj);
        }

        JSONObject obj = new JSONObject();
        obj.put("children", result);
        return obj;
    }

    /**
     * simulates a year pass
     */
    public final void passYear() {
        // updates years
        this.curentYear++;
        this.children.forEach((child) -> child.incAge());

        if (this.curentYear > this.numberOfYears) {
            return;
        }

        AnualChange anualChange = this.annualChanges.get(this.curentYear - 1);
        this.santaBudget = anualChange.getNewSantaBudget();

        // add gifts
        anualChange.getNewGifts().forEach(this::addGift);
        this.sortGifts();

        // add childrens
        anualChange.getNewChildren().forEach(this.children::add);

        // removes young adults
        this.children = new ArrayList(this.children.stream()
                                       .filter((child) -> child.getAge()
                                                       <= Constants.CHILD_AGETRESHOLD_YOUNGADULT)
                                       .toList());
        // update childrens
        for (AnualChange.ChildUpdate childUpdate:anualChange.getChildrenUpdates()) {
            Child child = this.getChild(childUpdate.getId());

            // check for young adults
            if (child == null) {
                continue;
            }

            // adds new niceScore
            if (childUpdate.getNiceScore() != -1) {
                child.addScore(childUpdate.getNiceScore());
            }

            // removes existin preferences that will be added
            child.getGiftsPreferences().removeAll(childUpdate.getGiftsPreferences());

            for (int i = 0, j = 0; i < childUpdate.getGiftsPreferences().size(); i++, j++) {
                // skip if a preference was already added
                if (child.getGiftsPreferences().contains(childUpdate.getGiftsPreferences()
                                                            .get(i))) {
                    j--;
                    continue;
                }

                child.getGiftsPreferences().add(j, childUpdate.getGiftsPreferences().get(i));
            }
        }
    }


    /**
     * gets a child by id
     */
    private Child getChild(final int id) {
        for (Child child:this.children) {
            if (child.getId() == id) {
                return child;
            }
        }
        return null;
    }


    /**
     * simulates all years
     */
    public final JSONObject runToCompletion() {
        JSONArray result = new JSONArray();

        while (this.curentYear <= this.numberOfYears) {
            result.add(this.getGifts());
            this.passYear();
        }

        JSONObject obj = new JSONObject();
        obj.put("annualChildren", result);
        return obj;
    }

}
