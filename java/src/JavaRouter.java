import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;


public class JavaRouter implements Runnable {

	Socket socket;
	JavaRouter(Socket newSocket)
	{
		this.socket = newSocket;
	}

	/**
     * Main method to run the router
     * Creates a socket for every connection request and runs the router code on that thread
     * Sends new message to new destination
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
                new Thread(new JavaRouter(clientSocket)).start();
            }
        }
        finally
        {
            listener.close();
        }
    }
    /**
     * Router Code:
     * Reads in the bytes of the message and determines the destination using the lookup table.
     * Writes over previous destination and sends message
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

			 String route = getRoute(data[1]);


		     Socket s = new Socket(route, 8000);

		     DataOutputStream output = new DataOutputStream(s.getOutputStream());

		     output.writeByte(data[0]);
		     output.writeByte(data[1]);
		     output.writeByte(data[2]);
		     output.writeShort(counter);
		     output.flush();

		     s.close();
         }
		 catch (IOException e)
		 {
			e.printStackTrace();
		 }
	}

	//Lookup table for new destination of the message
	public String getRoute(byte destination)
	{
		if(destination == 1)
		{
			return "127.0.0.1";
		}
		else if(destination == 'b')
		{
			return "mctsomething1";
		}
		else if(destination == 'c')
		{
			return "mctsomething2";
		}
		else if(destination == 'd')
		{
			return "mctsomething2";
		}
		else
		{
			return "127.0.0.1";
		}
	}

}

