import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

/**
 * Creates a runnable connection for each client connected to the server.
 *
 * @author Robert Windisch and Nick Martinez
 *
 */
public class ClientsThread implements Runnable
{
	private Socket clientConnection;
	private static final int MESSAGE_LEN = 10;

	/**
	 * Creates the client connection
	 * @param connect - the socket interface
	 */
	public ClientsThread(Socket connect)
	{
		clientConnection = connect;
	}

	/**
	 * Runs the server reading and handling of incoming messages.
	 */
	@Override
	public void run()
	{
		// DataInputStream for reading bytes in
		DataInputStream dis = null;
		try
		{
			dis = new DataInputStream(clientConnection.getInputStream());
		} catch (IOException e2)
		{
			System.out.println("Error creating DataInputStream");
			e2.printStackTrace();
		}

		// DataOutputStream for writing bytes out
		DataOutputStream dos = null;
		try
		{
			dos = new DataOutputStream(clientConnection.getOutputStream());
		} catch (IOException e2)
		{
			System.out.println("Error creating DataOutputStream");
			e2.printStackTrace();
		}

		try
		{
			byte[] inMessage = new byte[MESSAGE_LEN];
			while (dis.read(inMessage, 0, MESSAGE_LEN) > 0)
			{
				dos.write(reverse(inMessage), 0, MESSAGE_LEN);
			}
		} catch (IOException e )
		{
			System.out.println("server - read()");
			e.printStackTrace();
		}

		try
		{
			clientConnection.close();
		} catch (IOException e)
		{
			System.out.println("Error closing server");
			e.printStackTrace();
		}
	}

	/**
	 * Takes a message and reverses it.
	 *
	 * @param inMessage - array of bytes to be reversed
	 * @return reversed array of bytes
	 */
	private static byte[] reverse(byte[] inMessage)
	{
		byte[] newMsg = new byte[MESSAGE_LEN];
		for( int i = 0, j = MESSAGE_LEN - 1; i < MESSAGE_LEN; ++i, --j )
		{
			newMsg[i] = inMessage[j];
		}
		return newMsg;
	}
}
