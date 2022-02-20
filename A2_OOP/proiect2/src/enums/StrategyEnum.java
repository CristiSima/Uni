package enums;

import com.fasterxml.jackson.annotation.JsonProperty;

public enum StrategyEnum {
    @JsonProperty("niceScoreCity")
    NICE_SCORE_CITY("niceScoreCity"),

    @JsonProperty("id")
    ID("id"),

    @JsonProperty("niceScore")
    NICE_SCORE("niceScore");

    private String value;

    StrategyEnum(final String value) {
        this.value = value;
    }


    /**
     * gets a StrategyEnum from String
     */
    public static StrategyEnum getStrategyEnum(final String strategy) {
        switch (strategy) {
        case "niceScoreCity":
            return NICE_SCORE_CITY;
        case "id":
            return ID;
        case "niceScore":
            return NICE_SCORE;
        default:
            return null;
        }
    }
}
