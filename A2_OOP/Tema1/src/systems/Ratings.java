package systems;

import fileio.ActorInputData;
import fileio.MovieInputData;
import fileio.SerialInputData;

import java.util.Map;
import java.util.HashMap;

/**
 * Handles the favorite designation of a video
 */
public final class Ratings {

    private static Ratings ratings;
    /**
     * gets the ratings system
     */
    public static Ratings getRatings() {
        if (Ratings.ratings == null) {
            Ratings.ratings = new Ratings();
        }
        return Ratings.ratings;
    }
    /**
     * clears ratings
     */
    public static void clearRatings() {
        Ratings.ratings = null;
    }

    /**
     * Map of (Map of ratings from each user) for each movie
     */
    private Map<String, Map<String, Double>> movies;

    /**
     * Map of (Map of (Map of ratings from each user) for each season) for each serial
     */
    private Map<String, Map<Integer, Map<String, Double>>> serials;

    private Ratings() {
        this.movies = new HashMap();
        this.serials = new HashMap();
    }
    /**
     * tates a movie
     */
    public void rateMovie(final String title,
                            final String username, final double rating) {
        if (!this.movies.containsKey(title)) {
            this.movies.put(title, new HashMap());
        }
        Map<String, Double> movie = this.movies.get(title);

        movie.put(username, rating);

    }
    /**
     * rates a serial
     */
    public void rateSerial(final String title, final int seasonNr,
                            final String username, final double rating) {
        if (!this.serials.containsKey(title)) {
            this.serials.put(title, new HashMap());
        }
        Map<Integer, Map<String, Double>> serial = this.serials.get(title);


        if (!serial.containsKey(seasonNr)) {
            serial.put(seasonNr, new HashMap());
        }
        Map<String, Double> season = serial.get(seasonNr);

        season.put(username, rating);

    }
    /**
     * populates seasons
     * needed because of the way the rating is calculated
     */
    public void initSerial(final String title, final int seasonNr) {
        if (!this.serials.containsKey(title)) {
            this.serials.put(title, new HashMap());
        }
        Map<Integer, Map<String, Double>> serial = this.serials.get(title);


        for (int i = 1; i <= seasonNr; i++) {
            serial.put(i, new HashMap());
        }
    }
    /**
     * gets the ratings for a movie
     */
    public double getForMovie(final String title) {
        if (!this.movies.containsKey(title)) {
            return 0;
        }

        double sum = 0;
        int count = 0;

        for (double grade:this.movies.get(title).values()) {
            sum += grade;
            count++;
        }

        return sum / count;
    }
    /**
     * gets the ratings for a movie
     */
    public double getForVideo(final MovieInputData movie) {
        return this.getForMovie(movie.getTitle());
    }
    /**
     * checks if a movie has been rated by a specific user
     */
    public boolean checkForMovie(final String title, final String username) {
        return this.movies.containsKey(title)
               && this.movies.get(title).containsKey(username);
    }
    /**
     * gets the ratings for a serial
     */
    public double getForSerial(final String title) {
        if (!this.serials.containsKey(title)) {
            return 0;
        }

        double sum = 0;

        for (Map<String, Double> season:this.serials.get(title).values()) {
            double sumSeason = 0;
            int countSeason = 0;

            for (double grade:season.values()) {
                sumSeason += grade;
                countSeason++;
            }

            if (countSeason != 0) {
                sum += sumSeason / countSeason;
            }
        }

        return sum / this.serials.get(title).size();
    }
    /**
     * gets the ratings for a serial
     */
    public double getForVideo(final SerialInputData serial) {
        return this.getForSerial(serial.getTitle());
    }
    /**
     * gets the ratings for a video
     */
    public double getForVideo(final String title) {
        if (this.serials.containsKey(title)) {
            return this.getForSerial(title);
        }
        if (this.movies.containsKey(title)) {
            return this.getForMovie(title);
        }
        return 0;
    }
    /**
     * checks if a specific season of a serial has been rated by a specific user
     */
    public boolean checkForSerial(final String title, final int season, final String username) {
        return this.serials.containsKey(title)
               && this.serials.get(title).containsKey(season)
               && this.serials.get(title).get(season).containsKey(username);
    }
    /**
     * gets the number of videos rated by a user
     */
    public int getFromUser(final String username) {
        int rated = 0;
        for (Map<String, Double> movie:this.movies.values()) {
            if (movie.containsKey(username)) {
                rated++;
            }
        }
        for (Map<Integer, Map<String, Double>> serial:this.serials.values()) {
            for (Map<String, Double> season: serial.values()) {
                if (season.containsKey(username)) {
                    rated++;
                }
            }
        }
        return rated;
    }
    /**
     * gets the (indirect) ratings of an actor
     * the average of all the rated videoes the played in
     */
    public double getForActor(final ActorInputData actor) {
        double showRate;
        double sum = 0;
        int count = 0;

        for (String title:actor.getFilmography()) {
            if (this.movies.containsKey(title)) {
                showRate = this.getForMovie(title);
                if (showRate != 0) {
                    sum += showRate;
                    count++;
                }
            } else if (this.serials.containsKey(title))    {
                showRate = this.getForSerial(title);
                if (showRate != 0) {
                    sum += showRate;
                    count++;
                }
            }
        }
        if (sum == 0) {
            return 0;
        }

        return sum / count;
    }

    /**
     * gets a string representaion of all the ratings
     */
    public String toString() {
        return "Ratings{"
               + "movies=" + this.movies + ", "
               + "serials=" + this.serials
               + "}";
    }
}
