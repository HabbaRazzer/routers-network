//import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
//import java.io.InputStreamReader;
//import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Random;

/*
 * Client will sleep for 2 seconds and then send a message to another random client,
 * the message will include the source, destination, checksum, and data for a total of 5 bytes,
 * message data will be an integer that increments by one for every message that sends.
 * Checksum is the sum of all other bytes and will be used by the routers to insure no corruption
 * @author Darnell Martin & Stephen Clabaugh
 */

public class ClientAll
{

	public static void main(String[] args) throws UnknownHostException, IOException, InterruptedException
	{

		//Creates thread of JavaServerRunner to listen for additional clients at one time.
		Thread serverThread = new Thread(new JavaServerRunner());
		serverThread.start();

		byte destination = 'A';
		byte source = 'A';
		byte checksum;
		short data = 120;

		while(true)
		{
			Thread.sleep(2000);
			String routerAddress = "127.0.0.1";
	        Socket s = new Socket(routerAddress, 8080);
	        //Define the PrintWriter to write output to the server
	        DataOutputStream output = new DataOutputStream(s.getOutputStream());

	        data++;
	        checksum = (byte)(source + destination + data);

	        //Writes data to outgoing message
	        destination = randomizeClient();
	        output.writeByte(source);
	        output.writeByte(destination);
	        output.writeByte(checksum);
	        output.writeShort(data);
	        output.flush();
	        s.close();
		}


	}

	//randomize client destination
	public static byte randomizeClient()
	{
		int i = 0;
		byte[] destinations = {'A', 'B', 'C', 'D'};

		//Random rand = new Random();
		//int value = rand.nextInt(3);

		return 'A';
		//return destinations[i];

	}

}


