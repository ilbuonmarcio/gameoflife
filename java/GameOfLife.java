
public class GameOfLife {
	
	public static void main(String[] args) {
		Board board = new Board(16, 32);
		
		while(true) {
			board.parse();
			board.draw();
		}
	}

}
