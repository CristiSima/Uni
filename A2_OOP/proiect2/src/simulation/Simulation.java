package simulation;

import ioutils.InputData;
import elements.AnualChange;
import elements.Child;
import elements.Gift;
import common.Constants;
import enums.Category;
import enums.StrategyEnum;
import strategies.OnBudget;
import strategies.OnGifts;
import strategies.GiftAssignmentOrder;

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

    private StrategyEnum strategy;

    // private List<Child> children;
    // private Map<Category, List<Gift>> gifts;
    // private List<AnualChange> annualChanges;

    private SimulationStorage storage;

    public Simulation(final InputData inputData) {
        this.numberOfYears = inputData.getNumberOfYears();
        this.santaBudget = inputData.getSantaBudget();
        List<Child> children = new ArrayList(inputData.getChildren().stream()
          .filter((child) -> child.getAge()
          <= Constants.CHILD_AGETRESHOLD_YOUNGADULT)
          .toList());
        List<AnualChange> annualChanges = inputData.getAnnualChanges();

        this.curentYear = 0;
        Map<Category, List<Gift>> gifts = new HashMap();

        this.storage = new SimulationStorage(children, gifts, annualChanges);

        inputData.getGifts().forEach(this.storage::addGift);
        this.storage.sortGifts();

        this.strategy = StrategyEnum.ID;

        this.curentYear = 0;
    }

    /**
     * distributes gifts
     */
    public final JSONObject getGifts() {
        this.storage.setActive();

        JSONArray result = new JSONArray();

        double sumAverageScore = 0;
        for (Child child: GiftAssignmentOrder.giftAssignmentOrderID(this.storage.children)) {
            sumAverageScore += child.getAverageNiceScore();
        }

        double budgetUnit = ((double) this.santaBudget) / sumAverageScore;

        this.storage.children = GiftAssignmentOrder.getGiftAssignmentOrder(this.storage.children,
                                                                            this.strategy);

        for (Child child:this.storage.children) {
            double assignedBudget = budgetUnit * child.getAverageNiceScore();
            assignedBudget = OnBudget.applyOnBudget(child, assignedBudget);
            JSONObject obj = child.toJSONObject();

            obj.put("assignedBudget", assignedBudget);
            JSONArray receivedGifts = new JSONArray();

            for (Category category: child.getGiftsPreferences()) {
                final double finalAssignedBudget = assignedBudget;
                Gift newGift = this.storage.getGiftFrom(category, (gift) -> gift.getPrice()
                                                                        <= finalAssignedBudget);
                if (newGift == null) {
                    continue;
                }
                assignedBudget -= newGift.getPrice();
                receivedGifts.add(newGift.toJSONObject());

            }

            OnGifts.applyOnGifts(child, receivedGifts);

            obj.put("receivedGifts", receivedGifts);
            result.add(obj);
        }

        result.sort((child1, child2) -> ((Integer) ((JSONObject) child1).get("id"))
                                         - ((Integer) ((JSONObject) child2).get("id")));

        JSONObject obj = new JSONObject();
        obj.put("children", result);
        this.storage.unsetActive();
        return obj;
    }

    /**
     * simulates a year pass
     */
    public final void passYear() {
        this.storage.setActive();

        // updates years
        this.curentYear++;
        this.storage.children.forEach((child) -> child.incAge());

        if (this.curentYear > this.numberOfYears) {
            return;
        }

        AnualChange anualChange = this.storage.annualChanges.get(this.curentYear - 1);
        this.santaBudget = anualChange.getNewSantaBudget();
        this.strategy = anualChange.getStrategy();

        // System.out.println();

        // add gifts
        anualChange.getNewGifts().forEach(this.storage::addGift);
        this.storage.sortGifts();

        // add childrens
        anualChange.getNewChildren().forEach(this.storage.children::add);

        // removes young adults
        this.storage.children = new ArrayList(this.storage.children.stream()
                                  .filter((child) -> child.getAge()
                                                      <= Constants.CHILD_AGETRESHOLD_YOUNGADULT)
                                  .toList());
        // update childrens
        for (AnualChange.ChildUpdate childUpdate:anualChange.getChildrenUpdates()) {
            Child child = this.storage.getChild(childUpdate.getId());

            // check for young adults
            if (child == null) {
                continue;
            }

            // adds new niceScore
            if (childUpdate.getNiceScore() != -1) {
                child.addScore(childUpdate.getNiceScore());
            }

            child.setElf(childUpdate.getElf());

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
        this.storage.unsetActive();
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
