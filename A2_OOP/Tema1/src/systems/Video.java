package systems;

import entertainment.Season;
import fileio.ShowInput;
import fileio.MovieInputData;
import fileio.SerialInputData;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
/**
 * a video on the platform
 */
public abstract class Video {
    private static Map<String, Video> videos;
    private static Map<String, Movie> movies;
    private static Map<String, Serial> serials;

    private static List<Video> videosList;
    private static List<Video> moviesList;
    private static List<Video> serialsList;
    /**
     * gets a video by title
     */
    public static Video getVideo(final String title) {
        return Video.videos.get(title);
    }
    /**
     * gets all videos
     */
    public static List<Video> getVideos() {
        return Video.videosList;
    }
    /**
     * gets all movies
     */
    public static List<Video> getMovies() {
        return Video.moviesList;
    }
    /**
     * gets all serials
     */
    public static List<Video> getSerials() {
        return Video.serialsList;
    }
    /**
     * registers a movie
     */
    public static void registerMovie(final MovieInputData show) {
        if (Video.videos == null) {
            Video.videos = new HashMap();
            Video.videosList = new ArrayList();
        }
        if (Video.movies == null) {
            Video.movies = new HashMap();
            Video.moviesList = new ArrayList();
        }
        Movie movie = new Movie(show);
        Video.videos.put(show.getTitle(), movie);
        Video.videosList.add(movie);
        Video.movies.put(show.getTitle(), movie);
        Video.moviesList.add(movie);
    }
    /**
     * registers a serial
     */
    public static void registerSerial(final SerialInputData show) {
        if (Video.videos == null) {
            Video.videosList = new ArrayList();
            Video.videos = new HashMap();
        }
        if (Video.serials == null) {
            Video.serialsList = new ArrayList();
            Video.serials = new HashMap();
        }
        Serial serial = new Serial(show);
        Video.videos.put(show.getTitle(), serial);
        Video.videosList.add(serial);
        Video.serials.put(show.getTitle(), serial);
        Video.serialsList.add(serial);
    }
    /**
     * removes the videos
     */
    public static void clearVideos() {
        Video.videos = null;
        Video.movies = null;
        Video.serials = null;
        Genres.clearGenres();
    }


    protected final String title;

    protected final int year;

    protected final List<String> genres;

    protected Video(final ShowInput show) {
        this.title = show.getTitle();
        this.year = show.getYear();
        this.genres = show.getGenres();

        Genres.getGenres().addVideo(show);
    }
    /**
     * gets the title
     */
    public String getTitle() {
        return this.title;
    }
    /**
     * gets the year it was made in
     */
    public int getYear() {
        return this.year;
    }
    /**
     * gets all genres it belongs to
     */
    public List<String> getGenres() {
        return this.genres;
    }
    /**
     * gets the duration of a video
     * depends on the type
     */
    public abstract int getDuration();
    /**
     * checks if it was marked as favorite by a specific user
     */
    public boolean checkFavoriteFrom(final String username) {
        return Favorites.getFavorites().checkFromUser(this.title, username);
    }
    /**
     * gets the number of users who marked the video as favorite
     */
    public int getFavorites() {
        return Favorites.getFavorites().getForVideo(this.title);
    }
    /**
     * gets the total views for the video
     */
    public int getViews() {
        return Views.getViews().getForVideo(this.title);
    }
    /**
     * gets the ratings for the video
     */
    public double getRatings() {
        return Ratings.getRatings().getForVideo(this.title);
    }
}

class Movie extends Video {
    private int duration;
    Movie(final MovieInputData movie) {
        super(movie);
        this.duration = movie.getDuration();
    }

    public int getDuration() {
        return this.duration;
    }
}

class Serial extends Video {
    private List<Season> seasons;
    private int seasonNr;

    Serial(final SerialInputData serial) {
        super(serial);
        this.seasons = serial.getSeasons();
        this.seasonNr = serial.getNumberSeason();
        Ratings.getRatings().initSerial(this.title, this.seasonNr);
    }

    public int getDuration() {
        int sum = 0;
        for (Season season:this.seasons) {
            sum += season.getDuration();
        }
        return sum;
    }
}
