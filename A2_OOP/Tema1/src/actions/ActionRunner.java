package actions;

import fileio.Input;
import fileio.ActionInputData;

import org.json.simple.JSONObject;

/**
 * interface for functions that do the specified action
 */
public interface ActionRunner {
    /**
     * the actual function
     */
    JSONObject run(Input input, ActionInputData action);
}
