package systems;

import fileio.ShowInput;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

/**
 * Handles the genres of a video
 */
public final class Genres {
    private static Genres genres;
    /**
     * gets the genre system
     */
    public static Genres getGenres() {
        if (Genres.genres == null) {
            Genres.genres = new Genres();
        }
        return Genres.genres;
    }
    /**
     * clears genres
     */
    public static void clearGenres() {
        Genres.genres = null;
    }

    // Map of (Lists of genres) for each video
    private Map<String, List<String>> showBased;
    // Map of (Lists of genres) for each video
    private Map<String, List<String>> genreBased;

    private Genres() {
        this.showBased = new HashMap();
        this.genreBased = new HashMap();
    }
    /**
     * adds a show to the database
     */
    public void addVideo(final ShowInput show) {
        String title = show.getTitle();
        this.showBased.put(title, new ArrayList(show.getGenres()));

        for (String genre: show.getGenres()) {
            if (!this.genreBased.containsKey(genre)) {
                this.genreBased.put(genre, new ArrayList());
            }
            this.genreBased.get(genre).add(title);
        }
    }
    /**
     * gets the ratings for all genres
     * the rating of a genre is the average of all rated videos within it
     */
    public Map<String, Integer> getGenresRatings() {
        Map<String, Integer> result = new HashMap();
        Views views = Views.getViews();

        for (var entry:this.genreBased.entrySet()) {
            int viewCount = 0;

            for (String title:entry.getValue()) {
                viewCount += views.getForVideo(title);
            }

            result.put(entry.getKey(), viewCount);
        }

        return result;
    }
    /**
     * gets all videos in a genre
     */
    public List<String> getForGenre(final String genre) {
        if (this.genreBased.containsKey(genre)) {
            return this.genreBased.get(genre);
        }
        return new ArrayList();
    }
}
