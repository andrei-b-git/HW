public class Dice {

	public int North;
	public int East;
	public int South;
	public int West;
	public int Top;
	public int Bottom;
	
	public Dice(int west, int north, int east, int south, int top, int bottom) {
		West = west;
		North = north;
		East = east;
		South = south;
		Top = top;
		Bottom = bottom;
	}

	public int getNorth() {
		return North;
	}

	public int getEast() {
		return East;
	}

	public int getSouth() {
		return South;
	}

	public int getWest() {
		return West;
	}

	public int getTop() {
		return Top;
	}

	public int getBottom() {
		return Bottom;
	}

	public int getCost() {
		return Top;
	}

	public Dice MoveNorth() {
		return new Dice(West, Top, East, Bottom, South, North);
	}

	public Dice MoveSouth() {
		return new Dice(West, Bottom, East, Top, North, South);
	}

	public Dice MoveEast() {
		return new Dice(Bottom, North, Top, South, West, East);
	}

	public Dice MoveWest() {
		return new Dice(Top, North, Bottom, South, East, West);
	}
}
