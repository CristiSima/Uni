package enums;

import com.fasterxml.jackson.annotation.JsonProperty;

public enum Cities {

    @JsonProperty("Bucuresti")
    BUCURESTI("Bucuresti"),

    @JsonProperty("Constanta")
    CONSTANTA("Constanta"),

    @JsonProperty("Buzau")
    BUZAU("Buzau"),

    @JsonProperty("Timisoara")
    TIMISOARA("Timisoara"),

    @JsonProperty("Cluj-Napoca")
    CLUJ("Cluj-Napoca"),

    @JsonProperty("Iasi")
    IASI("Iasi"),

    @JsonProperty("Craiova")
    CRAIOVA("Craiova"),

    @JsonProperty("Brasov")
    BRASOV("Brasov"),

    @JsonProperty("Braila")
    BRAILA("Braila"),

    @JsonProperty("Oradea")
    ORADEA("Oradea");

    private String value;

    Cities(final String value) {
        this.value = value;
    }

    /**
     * converts to string
     */
    public final String toString() {
        return this.value;
    }


    /**
     * gets a Cities from String
     */
    public static Cities getCity(final String type) {
        switch (type) {
        case "Bucuresti":
            return BUCURESTI;
        case "Constanta":
            return CONSTANTA;
        case "Buzau":
            return BUZAU;
        case "Timisoara":
            return TIMISOARA;
        case "Cluj-Napoca":
            return CLUJ;
        case "Iasi":
            return IASI;
        case "Craiova":
            return CRAIOVA;
        case "Brasov":
            return BRASOV;
        case "Braila":
            return BRAILA;
        case "Oradea":
            return ORADEA;
        default:
            return null;
        }
    }
}
