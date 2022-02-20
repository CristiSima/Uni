package enums;

import com.fasterxml.jackson.annotation.JsonProperty;

public enum Category {

    @JsonProperty("Board Games")
    BOARD_GAMES("Board Games"),

    @JsonProperty("Books")
    BOOKS("Books"),

    @JsonProperty("Clothes")
    CLOTHES("Clothes"),

    @JsonProperty("Sweets")
    SWEETS("Sweets"),

    @JsonProperty("Technology")
    TECHNOLOGY("Technology"),

    @JsonProperty("Toys")
    TOYS("Toys");

    private final String value;

    Category(final String value) {
        this.value = value;
    }

    /**
     * converts to string
     */
    public final String toString() {
        return this.value;
    }


    /**
     * gets a Category from String
     */
    public static Category getCategory(final String categoryName) {
        switch (categoryName) {
            case "Board Games":
                return BOARD_GAMES;
            case "Books":
                return BOOKS;
            case "Clothes":
                return CLOTHES;
            case "Sweets":
                return SWEETS;
            case "Technology":
                return TECHNOLOGY;
            case "Toys":
                return TOYS;
            default:
                return null;
        }
    }

}
