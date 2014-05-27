import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class Main2 {

	/* Fisier de intrare */
	final static String TASK_INPUT_FILE = "zar.in";

	/* Fisier de iesire */
	final static String TASK_OUTPUT_FILE = "zar.out";

	public static void main(String[] args) throws FileNotFoundException, IOException {
		Board b = new Board(TASK_INPUT_FILE);

		int result = b.result();

		File f = new File(TASK_OUTPUT_FILE);
		f.createNewFile();
		PrintWriter out = new PrintWriter(new FileWriter(TASK_OUTPUT_FILE));
		out.print(result);
		out.close();
	}
}
