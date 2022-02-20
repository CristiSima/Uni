package actions;

import fileio.ActionInputData;

/**
 * Interface for functions that return the ActionRunner
 */
public interface ActionResolver {
    /**
     * the actual function
     */
    ActionRunner run(ActionInputData action);
}
