package actions;


import systems.User;
import systems.Video;
import fileio.ActionInputData;
import fileio.ActorInputData;
import fileio.Input;
import utils.Utils;
import utils.Comparators;
import utils.Filters;

import org.json.simple.JSONObject;

import java.util.List;
import java.util.ArrayList;
import java.util.stream.Collectors;

/**
 * implementation for queries and the ActionResolver for them
 */
final class Query {

    public static ActionRunner getAction(final ActionInputData command) {
        return Query::query;
    }

    private Query() {
    }

    public static JSONObject query(final Input input, final ActionInputData command) {
        List<String> result;

        switch (command.getObjectType()) {
        case "movies":
        case "shows":
            result = video(input, command);
            break;
        case "users":
            result = users(input, command);
            break;
        case "actors":
            result = actors(input, command);
            break;
        default:
            result = new ArrayList();
            result.add("NOT IMPLEMENTED");
        }
        return Utils.generateResult(command.getActionId(),
                "Query result: " + result);
    }
    public static List<String> video(final Input input, final ActionInputData command) {
        List<Video> result = new ArrayList();

        int targetYear = -1;
        if (command.getFilters().get(0).get(0) != null) {
            targetYear = Integer.parseInt(command.getFilters().get(0).get(0));
        }

        switch (command.getObjectType()) {
        case "movies":
            result = Video.getMovies();

            break;
        case "shows":
            result = Video.getSerials();
            break;
        case "videos":
            result = Video.getVideos();
            break;
        default:
        }
        result = result.stream()
         .filter(Filters.videoYear(targetYear))
         .filter(Filters.videoHasGenre(command.getFilters().get(1).get(0)))
         .collect(Collectors.toList());

        switch (command.getCriteria()) {
        case "ratings":
            if (command.getActionId() != -1) {
                result = result.stream().filter(Comparators.ForVideo.RatingsComparator::filter)
                 .collect(Collectors.toList());
            }
            result.sort(new Comparators.ForVideo.RatingsComparator(command.getSortType()));
            break;
        case "favorite":
            result = result.stream().filter(Comparators.ForVideo.FavoriteComparator::filter)
             .collect(Collectors.toList());
            result.sort(new Comparators.ForVideo.FavoriteComparator(command.getSortType()));
            break;
        case "most_viewed":
            result = result.stream().filter(Comparators.ForVideo.ViewComparator::filter)
             .collect(Collectors.toList());
            result.sort(new Comparators.ForVideo.ViewComparator(command.getSortType()));
            break;
        case "longest":
            result = result.stream().filter(Comparators.ForVideo.DurationComparator::filter)
             .collect(Collectors.toList());
            result.sort(new Comparators.ForVideo.DurationComparator(command.getSortType()));
            break;
        default:
        }

        result = result.subList(0, Math.min(result.size(), command.getNumber()));

        List<String> output = new ArrayList();
        for (Video serial:result) {
            output.add(serial.getTitle());
        }

        return output;
    }

    public static List<String> users(final Input input, final ActionInputData command) {
        List<User> result = new ArrayList();

        result = User.getUsers();

        switch (command.getCriteria()) {
        case "num_ratings":
            result = result.stream().filter(Comparators.ForUser.NumRatingsComparator::filter)
             .collect(Collectors.toList());
            result.sort(new Comparators.ForUser.NumRatingsComparator(command.getSortType()));
            break;
        default:
        }

        result = result.subList(0, Math.min(result.size(), command.getNumber()));

        List<String> output = new ArrayList();
        for (User user:result) {
            output.add(user.getUsername());
        }

        return output;
    }

    public static List<String> actors(final Input input, final ActionInputData command) {
        List<ActorInputData> result = new ArrayList();

        result = input.getActors();

        switch (command.getCriteria()) {
        case "average":
            result = result.stream().filter(Comparators.ForActor.AverageRatingsComparator::filter)
             .collect(Collectors.toList());
            result.sort(new Comparators.ForActor.AverageRatingsComparator(command));
            break;
        case "awards":
            result = result.stream()
                            .filter(Comparators.ForActor.AwardsComparator.getfilter(command))
                            .collect(Collectors.toList());
            result.sort(new Comparators.ForActor.AwardsComparator(command));
            break;
        case "filter_description":
            result = result.stream()
                            .filter(Comparators.ForActor.DescriptionComparator.getfilter(command))
                            .collect(Collectors.toList());
            result.sort(new Comparators.ForActor.DescriptionComparator(command));
            break;
        default:
        }

        result = result.subList(0, Math.min(result.size(), command.getNumber()));

        List<String> output = new ArrayList();
        for (ActorInputData actor:result) {
            output.add(actor.getName());
        }

        return output;
    }
}
