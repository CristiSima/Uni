package lab5.task4;

public class ThrillerAlbum extends Album {
	public void addSong(Song song) {
		if (song.getComposer().equals("Michael Jackson")) {
			this.songs.add(song);
		}
	}
}
