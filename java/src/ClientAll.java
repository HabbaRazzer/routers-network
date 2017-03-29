import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class ClientAll
{

	public static void main(String[] args) throws UnknownHostException, IOException, InterruptedException
	{
		Thread serverThread = new Thread(new JavaServerRunner());
		serverThread.start();

		byte destination = 'a';
		byte source = 'b';
		byte checksum = 0;
		byte[] data = new byte[2];

		while(true)
		{
			Thread.sleep(10000);
			String routerAddress = "127.0.0.1";
	        Socket s = new Socket(routerAddress, 8080);
	        //Define the PrintWriter to write output to the server
	        PrintWriter output = new PrintWriter(s.getOutputStream(), true);
	        //Define the BufferedReader to get the input from the server
	        BufferedReader input = new BufferedReader(new InputStreamReader(s.getInputStream()));

	        checksum = (byte) ('b'+'b'+data[0]+data[1]);

	        data[1]++;
	        
	        destination = randomizeClient();
	        output.print(source + destination + checksum + data[0] + data[1]);
	        output.flush();

	        s.close();
		}


	}

	public static byte randomizeClient()
	{
		//int i = 0;
		//byte[] destinations = new byte[4];

		//randomize i between 0 and 3
		return 'A';
		//return destinations[i];

	}

}


