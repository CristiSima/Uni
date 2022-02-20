package strategies;

import elements.Child;
import common.Constants;

public final class OnBudget {
    private OnBudget() { }

    /**
     * "visitor" entrypoint
     */
    public static double applyOnBudget(final Child child, final double budget) {
        switch (child.getElf()) {
        case BLACK:
            return OnBudget.onBudgetBLACK(child, budget);
        case PINK:
            return OnBudget.onBudgetPINK(child, budget);
        default:
            return budget;
        }
    }

    /**
     * subtracts 30%
     */
    public static double onBudgetBLACK(final Child child, final double budget) {
        return budget - budget * Constants.PROC_30 / Constants.PROC_100;
    }

    /**
     * adds 30%
     */
    public static double onBudgetPINK(final Child child, final double budget) {
        return budget + budget * Constants.PROC_30 / Constants.PROC_100;
    }
}
