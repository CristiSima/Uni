package systems;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
/**
 * Handles the favorite designation of a video
 */
public final class Favorites {
    private static Favorites favorites;
    /**
     * gets the favorites system
     */
    public static Favorites getFavorites() {
        if (Favorites.favorites == null) {
            Favorites.favorites = new Favorites();
        }
        return Favorites.favorites;
    }
    /**
     * clears favorites
     */
    public static void clearFavorites() {
        Favorites.favorites = null;
    }

    // Map of (Lists of users who favor a shows) for each video
    private Map<String, List<String>> shows;

    private Favorites() {
        this.shows = new HashMap();
    }
    /**
     * marks a video for a user
     */
    public void add(final String title, final String username) {
        if (!this.shows.containsKey(title)) {
            this.shows.put(title, new ArrayList());
        }
        List<String> users = this.shows.get(title);

        users.add(username);
    }
    /**
     * how many users marked a video
     */
    public int getForVideo(final String title) {
        if (!this.shows.containsKey(title)) {
            return 0;
        }
        return this.shows.get(title).size();
    }
    /**
     * checks if a user marked a video
     */
    public boolean checkFromUser(final String title, final String username) {
        return this.shows.containsKey(title)
               && this.shows.get(title).contains(username);
    }
}
