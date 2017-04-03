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
     */
    public static void main(String[] args) throws IOException
    {
        ServerSocket listener = new ServerSocket(8080);
        System.out.println("Listening");
        try
        {
            while (true)
            {
                Socket clientSocket = listener.accept();
                System.out.println("Connected");
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

			 if(~(byte)(data[0]+data[1]+counter) != (byte) ~data[2])
			 {
				 System.exit(1);
			 }


			 System.out.println("Data:");
			 System.out.println("Source:"+(char)data[0]);
			 System.out.println("Destination:"+(char)data[1]);
			 System.out.println("Checksum:"+data[2]);
			 System.out.println("Data:"+counter);

         }
		 catch (IOException e)
		 {
			e.printStackTrace();
		 }
	}
}
