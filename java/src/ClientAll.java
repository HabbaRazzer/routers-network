//import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
//import java.io.InputStreamReader;
//import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class ClientAll
{

	public static void main(String[] args) throws UnknownHostException, IOException, InterruptedException
	{
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

	        destination = randomizeClient();
	        output.writeByte(source);
	        output.writeByte(destination);
	        output.writeByte(checksum);
	        output.writeShort(data);
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


