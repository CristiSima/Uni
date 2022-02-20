package simulation;

import elements.AnualChange;
import elements.Child;
import elements.Gift;
import enums.Category;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;

public final class SimulationStorage {
    private static SimulationStorage currentSimulationStorage;

    public static SimulationStorage getCurrentSimulationStorage() {
        return SimulationStorage.currentSimulationStorage;
    }

    public SimulationStorage(final List<Child> children, final Map<Category, List<Gift>> gifts,
                            final List<AnualChange> annualChanges) {
        this.children = children;
        this.gifts = gifts;
        this.annualChanges = annualChanges;
    }

    /**
     * sets the singleton
     */
    public void setActive() {
        SimulationStorage.currentSimulationStorage = this;
    }

    /**
     * resets the singleton
     */
    public void unsetActive() {
        if (SimulationStorage.currentSimulationStorage == this) {
            SimulationStorage.currentSimulationStorage = null;
        }
    }

    /**
     * these MUSN'T be public
     * the idea is to be accessible from Simulation(or the same package)
     */
    List<Child> children;
    Map<Category, List<Gift>> gifts;
    List<AnualChange> annualChanges;

    /**
     * gets a child by id
     */
    public Child getChild(final int id) {
        for (Child child:this.children) {
            if (child.getId() == id) {
                return child;
            }
        }
        return null;
    }

    /**
     * sorts gifts
     */
    public void sortGifts() {
        for (List<Gift> giftList: this.gifts.values()) {
            giftList.sort((g1, g2) -> Double.compare(g1.getPrice(), g2.getPrice()));
        }
    }

    /**
     * adds a new gift
     */
    public void addGift(final Gift gift) {
        if (!this.gifts.containsKey(gift.getCategory())) {
            this.gifts.put(gift.getCategory(), new ArrayList());
        }
        this.gifts.get(gift.getCategory()).add(gift);
    }

    /**
     * returns the first gift from a Category that is in stock and matches tester
     */
    public Gift getGiftFrom(final Category category,
                            final Gift.GiftGiverFromCategoryTester tester) {
        if (!this.gifts.containsKey(category)) {
            return null;
        }

        for (Gift gift: this.gifts.get(category)) {
            if (tester.test(gift) && gift.getQuantity() != 0) {
                gift.decQuantity();
                return gift;
            }
        }

        return null;
    }

    /**
     * returns the first gift from a Category IF is in stock and matches tester
     */
    public Gift getFisrtGiftFrom(final Category category,
                                final Gift.GiftGiverFromCategoryTester tester) {
        if (!this.gifts.containsKey(category)) {
            return null;
        }

        Gift gift = this.gifts.get(category).get(0);

        if (tester.test(gift) && gift.getQuantity() != 0) {
            gift.decQuantity();
            return gift;
        }

        return null;
    }
}
