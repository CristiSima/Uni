package actions;

import java.util.Map;
import java.util.HashMap;
import fileio.ActionInputData;

/**
 * Root action resolver
 */
public final class Action {
    private static Map<String, ActionResolver> actions;

    private Action() {
    }

    static {
        Action.actions = new HashMap<String, ActionResolver>();
        Action.actions.put("command", Command::getAction);
        Action.actions.put("query", Query::getAction);
        Action.actions.put("recommendation", Recommendation::getAction);
    }
    /**
     * returns the ActionRunner for that action
     */
    public static ActionRunner getAction(final ActionInputData action) {
        return Action.actions.get(action.getActionType()).run(action);
    }
}
