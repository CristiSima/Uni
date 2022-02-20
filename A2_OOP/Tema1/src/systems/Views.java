package systems;

import java.util.Map;
import java.util.HashMap;
/**
 * Handles the views of a video
 */
public final  class Views {
    private static Views views;
    /**
     * gets the views system
     */
    public static Views getViews() {
        if (Views.views == null) {
            Views.views = new Views();
        }
        return Views.views;
    }
    /**
     * clears views
     */
    public static void clearViews() {
        Views.views = null;
    }

    // Map of (Map of views for users) for each show
    private Map<String, Map<String, Integer>> shows;
    private Map<String, Integer> allViews;

    private Views() {
        this.shows = new HashMap();
        this.allViews = new HashMap();
    }
    /**
     * adds a view to a video from a user
     */
    public void addFor(final String title, final String username) {
        this.addFor(title, username, 1);
    }
    /**
     * adds newViews to a video from a user
     */
    public void addFor(final String title, final String username, final int newViews) {
        if (!this.shows.containsKey(title)) {
            this.shows.put(title, new HashMap());
            this.allViews.put(title, 0);
        }
        Map<String, Integer> users = this.shows.get(title);

        if (!users.containsKey(username)) {
            users.put(username, newViews);
        } else {
            users.put(username, users.get(username) + newViews);
        }

        this.allViews.put(title, this.allViews.get(title) + newViews);
    }
    /**
     * gets the total views of a video
     */
    public int getForVideo(final String title) {
        if (!this.allViews.containsKey(title)) {
            return 0;
        }
        return this.allViews.get(title);
    }
    /**
     * checks if a user has seen a video
     */
    public boolean checkVideoForUser(final String title, final String username) {
        return this.shows.containsKey(title)
               && this.shows.get(title).containsKey(username)
               && this.shows.get(title).get(username) != 0;
    }
    /**
     * gets the views of a user on a video
     */
    public int getVideoForUser(final String title, final String username) {
        if (this.shows.containsKey(title)
         && this.shows.get(title).containsKey(username)) {
            return this.shows.get(title).get(username);
        }
        return 0;
    }
}
