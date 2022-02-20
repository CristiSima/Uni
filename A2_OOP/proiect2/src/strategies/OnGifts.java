package strategies;

import elements.Child;
import elements.Gift;
import simulation.SimulationStorage;

import org.json.simple.JSONArray;

public final class OnGifts {
    private OnGifts() { }

    /**
     * "visitor" entrypoint
     */
    public static void applyOnGifts(final Child child, final JSONArray receivedGifts) {
        switch (child.getElf()) {
        case YELLOW:
            OnGifts.onGiftsYELLOW(child, receivedGifts);
            return;
        default:
            return;
        }

    }

    /**
     * adds the cheapest gift in the most prefered category IF its in stock
     */
    public static void onGiftsYELLOW(final Child child, final JSONArray receivedGifts) {
        Gift newGift;
        if (receivedGifts.size() != 0) {
            return;
        }

        newGift = SimulationStorage.getCurrentSimulationStorage()
                    .getFisrtGiftFrom(child.getGiftsPreferences().get(0), (gift) -> true);

        if (newGift == null) {
            return;
        }

        receivedGifts.add(newGift.toJSONObject());
    }
}
