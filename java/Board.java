import java.util.Random;

public class Board {
	
	private int width;
	private int height;
	private int[][] board;
	
	public Board(int width, int height) {
		this.width = width;
		this.height = height;
		
		board = new int[width][height];
		
		this.reset();
	}
	
	public void reset() {
		for(int i = 0; i < width; i++) {
			for(int j = 0; j < height; j++) {
				this.board[i][j] = new Random().nextInt(2);
			}
		}
	}
	
	public void parse() {
		int[][] tmp = new int[this.width][this.height];
		for(int i = 0; i < width; i++) {
			for(int j = 0; j < height; j++) {
				tmp[i][j] = this.board[i][j];
			}
		}
		
		
		for(int i = 0; i < width; i++) {
			for(int j = 0; j < height; j++) {
				int neighbours = 0;
				
				// Yup, that's ugly, but I don't really care, it's just Game of Life boisss ;)
				try { neighbours += tmp[i-1][j-1]; } catch (IndexOutOfBoundsException e) {}
				try { neighbours += tmp[i][j-1];   } catch (IndexOutOfBoundsException e) {}
				try { neighbours += tmp[i+1][j-1]; } catch (IndexOutOfBoundsException e) {}
				try { neighbours += tmp[i-1][j];   } catch (IndexOutOfBoundsException e) {}
				try { neighbours += tmp[i+1][j];   } catch (IndexOutOfBoundsException e) {}
				try { neighbours += tmp[i-1][j+1]; } catch (IndexOutOfBoundsException e) {}
				try { neighbours += tmp[i][j+1];   } catch (IndexOutOfBoundsException e) {}
				try { neighbours += tmp[i+1][j+1]; } catch (IndexOutOfBoundsException e) {}
				
				if(tmp[i][j] == 1 && (neighbours < 2 || neighbours > 3)) {
					this.board[i][j] = 0;
				}
				
				if(tmp[i][j] == 0 && neighbours == 3) {
					this.board[i][j] = 1;
				}
			}
		}
	}
	
	public void draw() {
		for(int i = 0; i < width; i++) {
			for(int j = 0; j < height; j++) {
				System.out.print(this.board[i][j] == 1 ? "*" : " ");
			}
			System.out.print("\n");
		}
		
		System.out.print("\n");
	}
}
