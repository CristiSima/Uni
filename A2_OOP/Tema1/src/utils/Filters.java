package utils;

import java.util.function.Predicate;

import systems.Views;
import systems.Video;

public final class Filters {
    private Filters() { }
    /**
     * filters out videos from a diferent year
     */
    public static Predicate<Video> videoYear(final int targetYear) {
        return (video) -> targetYear == -1
               || video.getYear() == targetYear;
    }
    /**
     * filters out videoes that don't belong to a specific year
     */
    public static Predicate<Video> videoHasGenre(final String genre) {
        return (video) -> genre == null
               || video.getGenres().contains(genre);
    }
    /**
     * filters out videos seen by the user
     */
    public static Predicate<String> videoNotSeen(final String username) {
        return (title) -> !Views.getViews().checkVideoForUser(title, username);
    }
}
