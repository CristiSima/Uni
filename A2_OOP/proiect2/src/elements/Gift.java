package elements;

import org.json.simple.JSONObject;
import enums.Category;

public class Gift {
    private String productName;
    private double price;
    private int quantity;
    private Category category;

    public Gift(final JSONObject json) {
        this.productName = (String) json.get("productName");
        this.price = ((Long) json.get("price")).doubleValue();
        this.quantity = ((Long) json.get("quantity")).intValue();
        this.category = Category.getCategory((String) json.get("category"));
    }

    /**
     * dcrements quantity
     */
    public final void decQuantity() {
        this.quantity--;
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

    public final int getQuantity() {
        return this.quantity;
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

    /**
     * interface used to provide the filter function
     */
    public interface GiftGiverFromCategoryTester {
        /**
         * filter function
         */
        boolean test(Gift gift);
    }
}
