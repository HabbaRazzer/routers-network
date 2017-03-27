import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class ClientAll
{

	public static void main() throws UnknownHostException, IOException, InterruptedException
	{
		Thread serverThread = new Thread(new JavaServerRunner());
		serverThread.run();

		byte destination = 'A';
		byte source = 'B';
		byte checksum = 0;
		byte[] data = new byte[2];

		while(true)
		{
			Thread.sleep(2000);
			String routerAddress = "127.0.0.1";
	        Socket s = new Socket(routerAddress, 8080);
	        //Define the PrintWriter to write output to the server
	        PrintWriter output = new PrintWriter(s.getOutputStream(), true);
	        //Define the BufferedReader to get the input from the server
	        BufferedReader input = new BufferedReader(new InputStreamReader(s.getInputStream()));



	        data[1]++;

	        output.println(source + destination + checksum + data[0] + data[1]);
	        /*for(int i=0; i<10; i++)
	        {
	        	output.println("HelloWorld");
	        	String answer = input.readLine();
	        	System.out.println("Data Received: "+answer);
	        }
	        */
	        s.close();
	        System.exit(0);



		}


	}

	public byte randomizeClient()
	{
		int i = 0;
		byte[] destinations = new byte[4];

//randomize i between 0 and 3

		return destinations[i];

	}

}


