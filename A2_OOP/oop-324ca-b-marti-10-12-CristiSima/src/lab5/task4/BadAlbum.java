package lab5.task4;

public class BadAlbum extends Album {

	public void addSong(Song song) {
		if ( song.getName().length() == 3 &&
		 song.getId() == makePalindrome(song.getId())) {
			this.songs.add(song);
		}
	}

	private int makePalindrome(int nr) {
		int res = 0;
        
		while (nr > 0) {
			res = res * 10 + nr % 10;
			nr /= 10;
		}

		return res;
	}
}
