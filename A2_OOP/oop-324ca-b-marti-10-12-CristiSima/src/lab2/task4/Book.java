package lab2.task4;

public class Book {
    private String title;
    private String author;
    private int year;

    public Book(String title, String author, int year) {
        this.title = title;
        this.author = author;
        this.year = year;
    }

    public String toString() {
        String json="{\n";
        
        json+="\t\"title\":\""+this.title+"\",\n";
        json+="\t\"author\":\""+this.author+"\",\n";
        json+="\t\"year\":"+this.year+"\n";
        json+="}";

        return json;
    }
}
