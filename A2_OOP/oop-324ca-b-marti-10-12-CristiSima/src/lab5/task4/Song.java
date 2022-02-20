package lab5.task4;

public class Song {
	private String name;
	private int id;
	private String composer;

	public Song(String name, int id, String composer) {
		this.name = new String(name);
		this.id = id;
		this.composer = new String(composer);
	}

	public String getName() {
		return this.name;
	}

	public int getId() {
		return this.id;
	}

	public String getComposer() {
		return this.composer;
	}

	public String toString() {
		StringBuilder sb = new StringBuilder("{\n");
        
		sb.append("\t\"id\":").append(id).append(",\n");
		sb.append("\t\"name\":\"").append(name).append("\",\n");
		sb.append("\t\"composer\":\"").append(composer).append("\"\n");
		sb.append("}");

		return sb.toString();
	}
}
