import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Random;

/**
 * Client will sleep for 2 seconds and then send a message to another random client,
 * the message will include the source, destination, checksum, and data for a total of 5 bytes,
 * message data will be an integer that increments by one for every message that sends.
 * Checksum is the sum of all other bytes and will be used by the routers to insure no corruption
 * @author Darnell Martin & Stephen Clabaugh
 */
public class ClientAll
{

	/**
	 * This is the main runner for the Java Client
	 * @param args - Arguments from the command line
	 * @throws UnknownHostException - Shouldn't
	 * @throws IOException - Shouldn't
	 * @throws InterruptedException - Shouldn't
	 */
	public static void main(String[] args) throws UnknownHostException, IOException, InterruptedException
	{

		//Creates thread of JavaServerRunner to listen for additional clients at one time.
		Thread serverThread = new Thread(new JavaServerRunner());
		serverThread.start();

		byte destination = 'D';
		byte source = 'C';
		int checksum;
		byte onesCompliment;
		short data = 120;

		while(true)
		{
			Thread.sleep(2000);
			String routerAddress = "127.0.0.1";
	        Socket s = new Socket(routerAddress, 8080);
	        //Define the DataOutputStream to write output to the server
	        DataOutputStream output = new DataOutputStream(s.getOutputStream());

	        data++;
	        checksum = source + destination + data;
	        onesCompliment = (byte)~checksum;

	        //Writes data to outgoing message
	        destination = randomizeClient();
	        output.writeByte(source);
	        output.writeByte(destination);
	        output.writeByte(onesCompliment);
	        output.writeShort(data);
	        output.flush();
	        s.close();
		}
	}

	/**
	 * Randomize Client to send to
	 * @return - Random Client
	 */
	public static byte randomizeClient()
	{
		byte[] destinations = {'A', 'B', 'C', 'D'};

		Random rand = new Random();
		int i = rand.nextInt(3);
		destinations[i] = 0;

		return 'D';
		//return destinations[i];

	}

}


