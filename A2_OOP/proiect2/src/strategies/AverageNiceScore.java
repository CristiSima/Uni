package strategies;

import common.Constants;
import elements.Child;

public final class AverageNiceScore {
    private AverageNiceScore() { }

    /**
     * "visitor" entrypoint
     */
    public static double getAverageNiceScore(final Child child) {
        if (child.getAge() < Constants.CHILD_AGETRESHOLD_KID) {
            // baby
            return Constants.CHILD_KID_SCORE;
        } else if (child.getAge() < Constants.CHILD_AGETRESHOLD_TEEN) {
            // kid
            return AverageNiceScore.calculateKid(child);
        } else if (child.getAge() <= Constants.CHILD_AGETRESHOLD_YOUNGADULT) {
            // teen
            return AverageNiceScore.calculateTeen(child);
        } else {
            // young adult
            return Constants.CHILD_YOUNGADULT_SCORE;
        }
    }

    /**
     * calculates for a kid
     */
    public static double calculateKid(final Child child) {
        return child.getOldScores().stream().reduce(0d, Double::sum)
                                                / ((double) child.getOldScores().size());
    }

    /**
     * calculates for a teen
     */
    public static double calculateTeen(final Child child) {
        double sum = 0;
        for (int i = 0; i < child.getOldScores().size(); i++) {
            sum += (i + 1) * child.getOldScores().get(i);
        }
        return sum / ((double) (child.getOldScores().size()
                                * (child.getOldScores().size() + 1)) / 2);
    }
}
