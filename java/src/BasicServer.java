import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Listens for incoming connections.
 *
 * @author Robert Windisch and Nick Martinez
 *
 */
public class BasicServer extends Thread
{
	/**
	 * listens for connections
	 * if one is found sends it to thread to handle
	 * @param args - arguments
	 * @throws IOException - if there is an issue creating the ServerSocket interface
	 */
	public static void main(String[] args) throws IOException
	{
		System.out.println("JAVA Server is running");

		// create sockets
		ServerSocket server = new ServerSocket(4446);
		Socket connect;

		// listen for multiple clients
		while(true)
		{
			connect = server.accept();
			ClientsThread newClient = new ClientsThread(connect);
			Thread thread = new Thread(newClient);
			thread.start();
		}
	}
}
