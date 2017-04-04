import java.io.DataInputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Server to listen for incoming connections to the client.
 * Confirms checksum is correct.
 * Creates socket for connection requests and prints data from the message.
 * @author Darnell Martin & Stephen Clabaugh
 *
 */
public class JavaServer implements Runnable {

	Socket socket;
	JavaServer(Socket newSocket)
	{
		this.socket = newSocket;
	}

	/**
     * Main method to run the server
	 * @param args - arguments
	 * @throws IOException - Shouldn't
     */
    public static void main(String[] args) throws IOException
    {
        ServerSocket listener = new ServerSocket(8080);
        try
        {
            while (true)
            {
                Socket clientSocket = listener.accept();
                new Thread(new JavaServer(clientSocket)).start();
            }
        }
        finally
        {
            listener.close();
        }
    }
    /**
     * Run the server and perform checksum on incoming messages.
     * Prints data from the message
     */
	@Override
	public void run()
	{
		 try
		 {
			 DataInputStream stream = new DataInputStream(socket.getInputStream());
			 byte[] data = new byte[3];
			 data[0] = stream.readByte();
			 data[1] = stream.readByte();
			 data[2] = stream.readByte();
			 short counter = stream.readShort();

			 int checksum = data[0]+data[1]+counter;

			 if(checksum+data[2] != 255)
			 {
				 System.out.println("Error: Message Checksum Failed");
				 return;
			 }

			 System.out.println("------Client-------");
			 System.out.println("Source:"+(char)data[0]);
			 System.out.println("Destination:"+(char)data[1]);
			 System.out.println("Data:"+counter);

         }
		 catch (IOException e)
		 {
			e.printStackTrace();
		 }
	}
}
