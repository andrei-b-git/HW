/*
 *	Tema 2 - Algoritmi paraleli si distribuiti
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333 CA
*/
package Tema2_APD;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.PrintWriter;
import java.io.RandomAccessFile;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.TreeMap;

public class Tema2 {

	//functie ce realizeaza trunchierea la un numar de zecimale
	public static double truncate(double value, int decimals) {

		long f = (long) Math.pow(10, decimals);
		value *= f;

		return (double) (long) value / f;
	}	

	public static void main(String[] args) throws IOException, 
													InterruptedException {

		int NT = Integer.parseInt(args[0]);
		String input = args[1];
		String output = args[2];

		//citirea din fisierul de input
		BufferedReader in = new BufferedReader(new FileReader(input));
		int i, j;

		//retinerea parametrilor necesari din fisier
		String DOC = in.readLine();					//doc de verificat
		int D = Integer.parseInt(in.readLine());	//nr de octeti
		double X = Float.parseFloat(in.readLine());	//pragul de similaritate
		int ND = Integer.parseInt(in.readLine());	//nr de documente
		String[] docs = new String[ND];				//documentele
		for(i=0; i<ND; i++)
			docs[i] = in.readLine();

		//hashmap cu perechi de tipul (nume_doc, vector partial)
		HashMap< String, HashMap<String, Integer> > MAP =
							new HashMap< String, HashMap<String, Integer> >();

		//functionalitatea MAP (realizez indexarea pentru fiecare document)
		for(j=0; j<ND; j++) {

			//creez un MapWorkPool cu nThreads NT
			MapWorkPool mwp = new MapWorkPool(NT);

			//citirea fragmentelor din fisier
			RandomAccessFile raf = new RandomAccessFile(docs[j],"r");

			//cat timp nu am ajuns la sfarsitul fisierului
			while(raf.getFilePointer() != raf.length()) {

				String fragment;
				//citesc fragmente de D octeti din fisier
				if(raf.length() - raf.getFilePointer() >= D) {
					byte[] arr = new byte[D];
					raf.readFully(arr, 0, D);
					fragment = new String(arr);

					char u = fragment.charAt(D-1);
					//mai citesc un octet pana ajung la un delimitator
					while(u != ' ' && u != '\t' && u != '\n' && u != '\r' && 
							u != '\f') {
						byte[] un_oct = new byte[1];
						raf.readFully(un_oct, 0, 1);
						String in_plus = new String(un_oct);
						fragment = fragment + in_plus;
						u = in_plus.charAt(0);
					}
				} else {
					//citesc un fragment de cati octeti au mai ramas in fisier
					int d = (int)(raf.length() - raf.getFilePointer());
					byte[] arr = new byte[d];
					raf.readFully(arr, 0, d);
					fragment = new String(arr);

					char u = fragment.charAt(d-1);
					//mai citesc un octet pana ajung la un delimitator
					while(u != ' ' && u != '\t' && u != '\n' && u != '\r' && 
							u != '\f') {
						byte[] un_oct = new byte[1];
						raf.readFully(un_oct, 0, 1);
						String in_plus = new String(un_oct);
						fragment = fragment + in_plus;
						u = in_plus.charAt(0);
					}				
				}

				//pun cate un fragment in MapPartialSolution si pe urma pun MPS
				//in MapWorkPool
				mwp.putWork(new MapPartialSolution(fragment));
			}
			//creez NT MapWorkeri pt ca am NT Threaduri
			MapWorker mw[] = new MapWorker[NT];
			//fiecare MapWorker are un MapWorkPool de NT threaduri
			for (i=0; i<NT; i++) {
				mw[i] =  new MapWorker(mwp);
				mw[i].start();
			}
			//nu trece de join pana nu termina threadul de lucrat
			for (i=0; i<NT; i++) {
				mw[i].join();
			}
			//realizez combinarea rezultatelor celor NT threaduri
			MAP.put(docs[j], new HashMap<String, Integer>());
			for (i=0; i<NT; i++)
				for (String key: mw[i].hm.keySet())
					if (MAP.get(docs[j]).containsKey(key) == true)
						MAP.get(docs[j]).put(key, 
								MAP.get(docs[j]).get(key)+mw[i].hm.get(key));
					else
						MAP.get(docs[j]).put(key, mw[i].hm.get(key));

			raf.close();
		}

		//structura care retine gradele de similaritate pt fiecare doc
		HashMap<String, Double> grad = new HashMap<String, Double>();

		//functionalitatea REDUCE (calculez gradul de similaritate intre DOC si
		//restul documentelor utilizand vectorii partiali de la MAP		

		//calculul numarului de cuvinte din DOC
		float sumDOC = 0;
		for(String key: MAP.get(DOC).keySet())
			sumDOC += MAP.get(DOC).get(key);

		//pentru fiecare document
		for(String MAPkey: MAP.keySet()) {
			//daca este diferit de DOC
			if(MAPkey.equals(DOC) == false) {

				//calculul nr de cuvinte din doc MAPkey
				float sum = 0;
				for(String key: MAP.get(MAPkey).keySet())
					sum += MAP.get(MAPkey).get(key);

				//creez un ReduceWorkPool cu nThreads NT
				ReduceWorkPool rwp = new ReduceWorkPool(NT);

				//calculez frecventele de aparitie a cuvintelor comune doc
				for(String key: MAP.get(MAPkey).keySet()) {
					if(MAP.get(DOC).containsKey(key) == true) {
						float f1 = ((float)MAP.get(DOC).get(key) / sumDOC) * 100;
						float f2 = ((float)MAP.get(MAPkey).get(key) / sum) * 100;
						//pun frecventele in ReducePartialSolutions
						rwp.putWork(new ReducePartialSolution(f1, f2));
					}
				}

				//creez NT ReduceWorkeri pt ca am NT Threaduri
				ReduceWorker rw[] = new ReduceWorker[NT];
				//fiecare ReduceWorker are un ReduceWorkPool de NT threaduri
				for (i=0; i<NT; i++) {
					rw[i] =  new ReduceWorker(rwp);
					rw[i].start();
				}
				//nu trece de join pana nu termina threadul de lucrat
				for (i=0; i<NT; i++) {
					rw[i].join();
				}

				//combin gradele de similarite calculate de fiecare thread
				float sim = 0;
				for(i=0; i<NT; i++)
					sim += rw[i].sim;

				//retin gradele de similaritate si le si trunchiez
				grad.put(MAPkey, truncate((double)(sim/100),3));
			}
		}

		//ordonez gradele de similaritate si le retin intr-un TreeMap
		ValueComparator bvc =  new ValueComparator(grad);
		TreeMap<String, Double> sorted_map = new TreeMap<String, Double>(bvc);

		sorted_map.putAll(grad);

		//afisarea in fisier
		PrintWriter out = new PrintWriter(output, "UTF-8");
		out.println("Rezultate pentru: ("+DOC+")\n");
		for (Entry<String, Double> entry : sorted_map.entrySet())
			if(entry.getValue() > X)
				out.println(entry.getKey()+" ("+entry.getValue()+"%)");

		//inchiderea fisierelor
		in.close();
		out.close();
	}

}
