package elements;

import org.json.simple.JSONObject;
import enums.Category;

public class Gift {
    private String productName;
    private double price;
    private Category category;

    public Gift(final JSONObject json) {
        this.productName = (String) json.get("productName");
        this.price = ((Long) json.get("price")).doubleValue();
        this.category = Category.getCategory((String) json.get("category"));
    }

    /**
     * assembles a JSONObject from this
     */
    public final JSONObject toJSONObject() {
        JSONObject json = new JSONObject();

        json.put("productName", this.productName);
        json.put("price", this.price);
        json.put("category", this.category.toString());

        return json;
    }

    public final String getProductName() {
        return this.productName;
    }

    public final double getPrice() {
        return this.price;
    }

    public final Category getCategory() {
        return this.category;
    }


    /**
     * assembles a string from this
     */
    public final String toString() {
        return "Gift{"
                + "productName:\"" + this.productName + "\", "
                + "category:" + this.category + ", "
                + "price:" + this.price
                + "}";
    }
}
