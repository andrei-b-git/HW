public class Square {

	public boolean Visited;
	public Dice Dice;
	public int Distance;
	public int x;
	public int y;

	public Square(int x, int y) {
		Visited = false;
		Dice = null;
		Distance = Integer.MAX_VALUE;
		this.x = x;
		this.y = y;
	}

	public boolean DistanceValidated() {
		return Dice != null;
	}

	public void ReThinkDistance(Square neighbour, Dice dice) {
		if (DistanceValidated()) {
			if (neighbour.getDistance() + dice.getCost() <= Distance) {
				Distance = neighbour.getDistance() + dice.getCost();
				Dice = dice;
			}
		} else {
			Dice = dice;
			if (neighbour == null) {
				Distance = Dice.getCost();
			} else {
				Distance = neighbour.getDistance() + dice.getCost();
			}
		}
	}

	public Dice getDice() {
		return Dice;
	}

	public int getDistance() {
		return Distance;
	}

	public Square North(Board board) {
		return board.getSquare(x - 1, y);
	}

	public Square East(Board board) {
		return board.getSquare(x, y + 1);
	}

	public Square South(Board board) {
		return board.getSquare(x + 1, y);
	}

	public Square West(Board board) {
		return board.getSquare(x, y - 1);
	}

	public boolean isNorth(Board board) {
		return (x - 1 < board.rows && x - 1>= 0) && 
				(y < board.cols && y >=0);
	}

	public boolean isEast(Board board) {
		return (x < board.rows && x >= 0) && 
				(y + 1 < board.cols && y + 1 >=0);
	}

	public boolean isSouth(Board board) {
		return (x + 1 < board.rows && x + 1 >= 0) && 
				(y < board.cols && y >=0);
	}

	public boolean isWest(Board board) {
		return (x < board.rows && x >= 0) && 
				(y - 1 < board.cols && y - 1 >=0);
	}
}
