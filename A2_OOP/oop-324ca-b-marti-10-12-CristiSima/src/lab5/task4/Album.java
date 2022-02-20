package lab5.task4;

import java.util.ArrayList;
import java.lang.StringBuilder;

abstract class Album {
	protected ArrayList<Song> songs = new ArrayList<Song>();

	abstract void addSong(Song song);

	public void removeSong() {
	}

	public String toString() {
		StringBuilder sb = new StringBuilder("{\n");
		sb.append("\tsongs:[ \n\t");

		if (this.songs.size() > 0) {
			sb.append("\t").append(this.songs.get(0).toString().replace("\n", "\n\t\t"));
		}

		for (Song song: this.songs) {
			if (song == this.songs.get(0)) {
				continue;
			}
			sb.append(",\n\t\t");
			sb.append(song.toString().replace("\n", "\n\t\t"));
		}

        sb.append("\n");
		sb.append("\t]\n");
		sb.append("}");

		return sb.toString();
	}
}
