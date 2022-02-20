package actions;

import systems.User;
import fileio.ActionInputData;
import fileio.Input;
import utils.Utils;
import org.json.simple.JSONObject;

import java.util.Map;
import java.util.HashMap;

/**
 * implementation for commands and the ActionResolver for them
 */
final class Command {
    static {
        Command.commands = new HashMap<>();
        Command.commands.put("favorite", Command::favorite);
        Command.commands.put("view", Command::view);
        Command.commands.put("rating", Command::rate);
    }

    private static Map<String, ActionRunner> commands;

    private Command() {
    }

    public static ActionRunner getAction(final ActionInputData command) {
        return Command.commands.get(command.getType());
    }

    public static JSONObject favorite(final Input input, final ActionInputData command) {
        User user = User.getUser(command.getUsername());
        String title = command.getTitle();

        if (user.getViews(title) == 0) {
            return Utils.generateResult(command.getActionId(),
                    "error -> " + title + " is not seen");
        }

        if (user.checkFavorite(title)) {
            return Utils.generateResult(command.getActionId(),
                    "error -> " + title + " is already in favourite list");
        }

        user.addFavorite(title);

        return Utils.generateResult(command.getActionId(),
                "success -> " + title + " was added as favourite");
    }

    public static JSONObject view(final Input input, final ActionInputData command) {
        User user = User.getUser(command.getUsername());
        String title = command.getTitle();

        user.addView(title);

        return Utils.generateResult(command.getActionId(),
                "success -> " + title + " was viewed with total views of "
                + user.getViews(title));
    }

    public static JSONObject rate(final Input input, final ActionInputData command) {
        User user = User.getUser(command.getUsername());
        String title = command.getTitle();

        if (user.getViews(title) == 0) {
            /* NOT VIEWED */
            return Utils.generateResult(command.getActionId(),
                    "error -> " + title + " is not seen");
        }

        if (command.getSeasonNumber() == 0) {
            /* it is a MOVIE */
            if (user.checkRating(title)) {
                /* Was rated */
                return Utils.generateResult(command.getActionId(),
                        "error -> " + title + " has been already rated");
            }

            user.addRating(title,
             command.getGrade());
        } else   {
            /* it is a SERIAL */
            if (user.checkRating(title, command.getSeasonNumber())) {
                /* Was rated */
                return Utils.generateResult(command.getActionId(),
                        "error -> " + title + " has been already rated");
            }
            user.addRating(title, command.getSeasonNumber(),
             command.getGrade());
        }

        return Utils.generateResult(command.getActionId(),
                                    "success -> " + title + " was rated with "
                                    + command.getGrade() + " by " + command.getUsername());
        }
}
