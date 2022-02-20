package lab5.task4;

public class DangerousAlbum extends Album {

	public void addSong(Song song) {
		if (isPrime(song.getId())) {
			this.songs.add(song);
		}
	}

	private boolean isPrime(int nr) {
		if (nr % 2 == 0 || nr < 2) {
			return false;
		}
        
		for (int i = 3; i*i <= nr; i += 2) {
			if (nr % i == 0) {
				return false;
			}
		}

		return true;
	}

}
