package actions;


import fileio.ActionInputData;
import fileio.Input;
import systems.Genres;
import systems.User;
import systems.Video;
import utils.Utils;
import utils.Filters;

import org.json.simple.JSONObject;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.stream.Collectors;
import java.util.Collections;

/**
 * implementation for recommendations and the ActionResolver for them
 */
public final class Recommendation {
    static {
        Recommendation.recommendations = new HashMap<>();
        Recommendation.recommendations.put("standard", Recommendation::standard);
        Recommendation.recommendations.put("best_unseen", Recommendation::bestUnseen);
        Recommendation.recommendations.put("favorite", Recommendation::favorite);
        Recommendation.recommendations.put("search", Recommendation::search);
        Recommendation.recommendations.put("popular", Recommendation::popular);
    }

    private Recommendation() {
    }

    private static Map<String, ActionRunner> recommendations;
    /**
     * gets the wanted recommendation
     */
    public static ActionRunner getAction(final ActionInputData command) {
        return Recommendation.recommendations.get(command.getType());
    }
    /**
     * standard Recommendation
     */
    public static JSONObject standard(final Input input, final ActionInputData command) {
        User user = User.getUser(command.getUsername());

        for (Video video: Video.getVideos()) {
            if (user.getViews(video.getTitle()) == 0) {
                return Utils.generateResult(command.getActionId(),
                        "StandardRecommendation result: " + video.getTitle());
            }
        }
        return Utils.generateResult(command.getActionId(),
                "StandardRecommendation cannot be applied!");
    }
    /**
     * best unseen recommendation
     */
    public static JSONObject bestUnseen(final Input input, final ActionInputData command) {
        User user = User.getUser(command.getUsername());

        String bestVideo = "";
        double bestRatings = -1;

        for (Video video: Video.getVideos()) {
            if (user.getViews(video.getTitle()) == 0
             && bestRatings < video.getRatings()) {

                bestRatings = video.getRatings();
                bestVideo = video.getTitle();
            }
        }

        if (bestRatings != -1) {
            return Utils.generateResult(command.getActionId(),
                    "BestRatedUnseenRecommendation result: " + bestVideo);

        }
        return Utils.generateResult(command.getActionId(),
                "BestRatedUnseenRecommendation cannot be applied!");
    }
    /**
     * favorite recommendation
     */
    public static JSONObject favorite(final Input input, final ActionInputData command) {
        User user = User.getUser(command.getUsername());
        if (!user.getSubscriptionType().equals("PREMIUM")) {
            return Utils.generateResult(command.getActionId(),
                    "FavoriteRecommendation cannot be applied!");
        }

        String bestVideo = "";
        int bestFavorited = 0;

        for (Video video: Video.getVideos()) {
            if (user.getViews(video.getTitle()) == 0
             && bestFavorited < video.getFavorites()) {
                bestFavorited = video.getFavorites();
                bestVideo = video.getTitle();
            }
        }

        if (bestFavorited != 0) {
            return Utils.generateResult(command.getActionId(),
                    "FavoriteRecommendation result: " + bestVideo);

        }
        return Utils.generateResult(command.getActionId(),
                "FavoriteRecommendation cannot be applied!");
    }
    /**
     * search genre recommendation
     */
    public static JSONObject search(final Input input, final ActionInputData command) {
        User user = User.getUser(command.getUsername());
        if (!user.getSubscriptionType().equals("PREMIUM")) {
            return Utils.generateResult(command.getActionId(),
                    "SearchRecommendation cannot be applied!");
        }

        List<String> result = new ArrayList();
        final ActionInputData queryParam = new ActionInputData(-1, null, "videos",
                                                                command.getGenre(),
          "asc", "ratings", null, 99999, null, null);
        result = Query.video(input, queryParam);

        result = result.stream().filter(Filters.videoNotSeen(command.getUsername()))
         .collect(Collectors.toList());

        if (result.size() != 0) {
            return Utils.generateResult(command.getActionId(),
                    "SearchRecommendation result: " + result);
        }
        return Utils.generateResult(command.getActionId(),
                "SearchRecommendation cannot be applied!");
    }
    /**
     * most popular recommendation
     */
    public static JSONObject popular(final Input input, final ActionInputData command) {
        User user = User.getUser(command.getUsername());
        if (!user.getSubscriptionType().equals("PREMIUM")) {
            return Utils.generateResult(command.getActionId(),
                    "PopularRecommendation cannot be applied!");
        }

        Genres genres = Genres.getGenres();
        List<Entry<String, Integer>> entries = new ArrayList(genres.getGenresRatings().entrySet());

        entries.sort(Entry.comparingByValue());
        Collections.reverse(entries);

        for (var entry:entries) {
            for (String title:genres.getForGenre(entry.getKey())) {
                if (user.getViews(title) == 0) {
                    return Utils.generateResult(command.getActionId(),
                            "PopularRecommendation result: " + title);
                }
            }
        }

        return Utils.generateResult(command.getActionId(),
                "PopularRecommendation cannot be applied!");
    }
}
