package strategies;

import elements.Child;
import enums.StrategyEnum;
import enums.Cities;

import java.util.List;
import java.util.HashMap;

public final class GiftAssignmentOrder {
    private GiftAssignmentOrder() { }

    /**
     * "visitor" entrypoint
     */
    public static List<Child> getGiftAssignmentOrder(final List<Child> children,
                                                    final StrategyEnum strategy) {
        switch (strategy) {
        case ID:
            return GiftAssignmentOrder.giftAssignmentOrderID(children);
        case NICE_SCORE_CITY:
            return GiftAssignmentOrder.giftAssignmentOrderNICESCORECITY(children);
        case NICE_SCORE:
            return GiftAssignmentOrder.giftAssignmentOrderNICESCORE(children);
        default:
            return children;
        }
    }

    /**
     * sorts based on ID
     */
    public static List<Child> giftAssignmentOrderID(final List<Child> children) {
        children.sort((child1, child2) -> child1.getId() - child2.getId());
        return children;
    }

    /**
     * sorts based on AverageNiceScore
     *                  then ID
     */
    public static List<Child> giftAssignmentOrderNICESCORE(final List<Child> children) {
        children.sort((child1, child2) -> {
            if (child1.getAverageNiceScore() != child2.getAverageNiceScore()) {
                return -Double.compare(child1.getAverageNiceScore(),
                                        child2.getAverageNiceScore());
            }
            return child1.getId() - child2.getId();
        });
        return children;
    }

    /**
     * sorts based on city avg NiceScore
     *                      then city name
     *                      then ID
     */
    public static List<Child> giftAssignmentOrderNICESCORECITY(final List<Child> children) {
        HashMap<Cities, Double> sumedScores = new HashMap();
        HashMap<Cities, Integer> sumedScoresCount = new HashMap();

        double sum;
        int count;
        for (Child child: GiftAssignmentOrder.giftAssignmentOrderID(children)) {
            if (!sumedScores.containsKey(child.getCity())) {
                sum = 0;
                count = 0;
            } else {
                sum = sumedScores.get(child.getCity());
                count = sumedScoresCount.get(child.getCity());
            }
            sum += child.getAverageNiceScore();
            count++;

            sumedScores.put(child.getCity(), sum);
            sumedScoresCount.put(child.getCity(), count);
        }

        children.sort((child1, child2) -> {
            double avgCity1 = sumedScores.get(child1.getCity())
                                / sumedScoresCount.get(child1.getCity());
            double avgCity2 = sumedScores.get(child2.getCity())
                                / sumedScoresCount.get(child2.getCity());

            if (avgCity1 != avgCity2) {
                return -Double.compare(avgCity1, avgCity2);
            }

            if (!child1.getCity().toString().equals(child2.getCity().toString())) {
                return child1.getCity().toString().compareTo(child2.getCity().toString());
            }

            return child1.getId() - child2.getId();
        });
        return children;
    }
}
