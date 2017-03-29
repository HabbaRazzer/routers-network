import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class JavaRouter implements Runnable {
	
	Socket socket;
	JavaRouter(Socket newSocket)
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
                new Thread(new JavaRouter(clientSocket)).start();
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
			 
			 byte checksum = (byte) ~data[2];
			 
			 String route = getRoute(data[1]);
			 
			 System.out.println("Data:");
			 System.out.print((data[0]));
			 System.out.print(data[1]);
			 System.out.print("");
			 
		     Socket s = new Socket(route, 8000);

		     PrintWriter output = new PrintWriter(s.getOutputStream(), true);

		     output.print(data);
		     output.flush();
		     

		     s.close();
         }
		 catch (IOException e)
		 {
			e.printStackTrace();
		 } 
	}
	
	public String getRoute(byte destination)
	{
		if(destination == 'a')
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

