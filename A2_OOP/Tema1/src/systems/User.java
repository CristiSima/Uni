package systems;

import fileio.UserInputData;


import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
/**
 * a user of the platform
 */
public class User {
    private static Map<String, User> users;
    /**
     * gets a user by username
     */
    public static User getUser(final String username) {
        return User.users.get(username);
    }
    /**
     * gets all the users
     */
    public static List<User> getUsers() {
        return new ArrayList(User.users.values());
    }
    /**
     * registers a user
     */
    public static void registerUser(final UserInputData user) {
        if (User.users == null) {
            User.users = new HashMap();
        }
        User.users.put(user.getUsername(), new User(user));
    }
    /**
     * removes the users
     */
    public static void clearUsers() {
        User.users = null;
        Favorites.clearFavorites();
        Views.clearViews();
        Ratings.clearRatings();
    }

    private String username;
    private String subscriptionType;

    protected User(final UserInputData user) {
        this.username = user.getUsername();
        this.subscriptionType = user.getSubscriptionType();


        for (String title:user.getFavoriteMovies()) {
            Favorites.getFavorites().add(title, user.getUsername());
        }
        for (var entry:user.getHistory().entrySet()) {
            Views.getViews().addFor(entry.getKey(), user.getUsername(), entry.getValue());
        }
    }

    /**
     * gets the username of the user
     */
    public String getUsername() {
        return username;
    }
    /**
     * gets the type of subscription of the user
     */
    public String getSubscriptionType() {
        return subscriptionType;
    }
    /**
     * gets the number of views the user generated
     */
    public int getViews(final String title) {
        return Views.getViews().getVideoForUser(title, this.username);
    }
    /**
     * gets the number of videos rated by the user
     */
    public int getRatings() {
        return Ratings.getRatings().getFromUser(this.username);
    }

    /**
     * checks if the user marked a video as fovorite
     */
    public boolean checkFavorite(final String title) {
        return Favorites.getFavorites().checkFromUser(title, this.username);
    }
    /**
     * checks if the user has rated a season
     */
    public boolean checkRating(final String title, final int seasonNr) {
        return Ratings.getRatings().checkForSerial(title, seasonNr, this.username);
    }
    /**
     * checks if the user has rated a movie
     */
    public boolean checkRating(final String title) {
        return Ratings.getRatings().checkForMovie(title, this.username);
    }

    /**
     * the user marks a video as favorite
     */
    public void addFavorite(final String title) {
        Favorites.getFavorites().add(title, this.username);
    }
    /**
     * the user views a video
     */
    public void addView(final String title) {
        Views.getViews().addFor(title, this.username);
    }
    /**
     * the user rates a season
     */
    public void addRating(final String title, final int seasonNr, final double rateing) {
        Ratings.getRatings().rateSerial(title, seasonNr, this.username, rateing);
    }
    /**
     * the user rates a movie
     */
    public void addRating(final String title, final double rateing) {
        Ratings.getRatings().rateMovie(title, this.username, rateing);
    }
}
