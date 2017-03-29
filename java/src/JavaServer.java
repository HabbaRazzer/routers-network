import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * This is a multi-threaded server implementation that receives a String from the
 * client and then reverses the String to send it back to the client.
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
     * Run the server 
     */
	@Override
	public void run() 
	{
		 try 
		 {
			 InputStream stream = socket.getInputStream();
			 byte[] data = new byte[5];
			 int count = stream.read(data);
			 
			 ByteBuffer bb = ByteBuffer.allocate(2);
			 bb.order(ByteOrder.LITTLE_ENDIAN);
			 bb.put(data[3]);
			 bb.put(data[4]);
			 short counter = bb.getShort(0);
			 
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
