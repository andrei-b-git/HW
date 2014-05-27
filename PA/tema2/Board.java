import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class Board {

	public Square[][] Squares;
	public int rows;
	public int cols;
	public Square source;
	public Square target;
	public Dice initialDice;

	public Board(String filename) throws FileNotFoundException {
		load(filename);
	}

	private void load(String filename) throws FileNotFoundException {
		Scanner inputScanner = new Scanner(new File(filename));

		rows = inputScanner.nextInt();
		cols = inputScanner.nextInt();

		Squares = new Square[rows][cols];
		for(int i = 0; i < rows; i++)
			for(int j = 0; j < cols; j++)
				Squares[i][j] = new Square(i, j);

		int top = inputScanner.nextInt();
		int south = inputScanner.nextInt();
		int east = inputScanner.nextInt();
		int west = inputScanner.nextInt();
		int north = inputScanner.nextInt();
		int bottom = inputScanner.nextInt();

		initialDice = new Dice(west, north, east, south, top, bottom);

		int sx = inputScanner.nextInt();
		int sy = inputScanner.nextInt();
		int tx = inputScanner.nextInt();
		int ty = inputScanner.nextInt();

		source = new Square(sx, sy);
		Squares[sx][sy] = source;
		target = new Square(tx, ty);
		Squares[tx][ty] = target;

		inputScanner.close();
	}

	public Square NeedUnVisitedDistanceDeterminedWithMinimumDistanceSquare() {
		Square result = null;
		for(int i = 0; i < rows; i++)
			for(int j = 0; j < cols; j++)
				if (Squares[i][j].Visited == false)
					if (Squares[i][j].DistanceValidated() == true)
						if (result == null)
							result = Squares[i][j];
						else
							if (result.Distance > Squares[i][j].Distance)
								result = Squares[i][j];
		return result;
	}

	public Square getSquare(int x, int y) {
		if(x < rows && x >= 0 && y < cols && y >=0) {
			Square result = Squares[x][y];
			return result;
		} else {
			return null;
		}
	}

	public void CalculateDistances() {
		source.ReThinkDistance(null, initialDice);
		Square Current = source;
		while (Current != null) {
			Current.Visited = true;
/*			System.out.println(Current.getDice().getTop()+" "+
					Current.getDice().getNorth()+" "+
					Current.getDice().getBottom()+" "+
					Current.getDice().getSouth()+" "+
					Current.getDice().getWest()+" "+
					Current.getDice().getEast());*/
			if (Current.isNorth(this) != false) {
				Current.North(this).ReThinkDistance(Current, Current.getDice().MoveNorth());
//				System.out.println("N "+"("+Current.North(this).x + "," + Current.North(this).y + ") " +
//						Current.North(this).getDistance() + "," + Current.North(this).getDice().getCost());
			}
			if (Current.isWest(this) != false) {
				Current.West(this).ReThinkDistance(Current, Current.getDice().MoveWest());
//				System.out.println("W "+"("+Current.West(this).x + "," + Current.West(this).y + ") " +
//						Current.West(this).getDistance() + "," + Current.West(this).getDice().getCost());
			}
			if (Current.isEast(this) != false) {
				Current.East(this).ReThinkDistance(Current, Current.getDice().MoveEast());
//				System.out.println("E "+"("+Current.East(this).x + "," + Current.East(this).y + ") " +
//									Current.East(this).getDistance() + "," + Current.East(this).getDice().getCost());
			}
			if (Current.isSouth(this) != false) {
				Current.South(this).ReThinkDistance(Current, Current.getDice().MoveSouth());
//				System.out.println("S "+"("+Current.South(this).x + "," + Current.South(this).y + ") " +
//						Current.South(this).getDistance() + "," + Current.South(this).getDice().getCost());
			}
			Current = NeedUnVisitedDistanceDeterminedWithMinimumDistanceSquare();
//			if(Current != null)
//				System.out.println("(" + Current.x + "," + Current.y + ") " + 
//									Current.getDistance() + "," + Current.getDice().getCost());
		}
	}

	public Square CalculateNext(Square square1, Square square2) {
		if (square1 == null && square2 == null) {
			return null;
		}
		else 
			if (square1 != null && square2 != null) {
				if (square1.Distance < square2.Distance) {
					return square1;
				} else {
					return square2;
				}
			}
			else 
				if (square1 != null) {
					return square1;
				}
				else {
					return square2;
				}
	}

	public ArrayList<Square> Calculate() {
		CalculateDistances();
/*		for(int i = 0; i < rows; i++) {
			for(int j = 0; j < cols; j++)
				System.out.print(Squares[i][j].getDistance() + "," + Squares[i][j].getDice().getCost() + "\t");
			System.out.println();
		}*/
		ArrayList<Square> result = new ArrayList<Square>();
		if (target.Visited) {
			Square current = target;
			while (current != source) {
				result.add(current);
				Square next = null;
				next = CalculateNext(next, current.North(this));
				next = CalculateNext(next, current.East(this));
				next = CalculateNext(next, current.South(this));
				next = CalculateNext(next, current.West(this));
				current = next;
				if (current == null) {
					result.clear();
					break;
				}
			}
		}
		return result;
	}

	public void print() {
		for(int i = 0; i < rows; i++)
			for(int j = 0; j < cols; j++) {
				System.out.println("(" + i + "," + j + ") " + Squares[i][j].getDistance());
		}

	}
	public int result() {
		ArrayList<Square> result = Calculate();

		return result.get(0).getDistance();

/*		for(Square s : result) {
			System.out.println("(" + s.x + "," + s.y + ") " + s.getDistance());
		}*/

	}
}
