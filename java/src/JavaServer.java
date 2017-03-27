import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

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
			 //Define the PrintWriter to write output to the server
			 PrintWriter output = new PrintWriter(this.socket.getOutputStream(), true);
		     //Define the BufferedReader to get the input from the server
			 BufferedReader input = new BufferedReader(new InputStreamReader(this.socket.getInputStream()));
			 while(input != null)
			 {
				 String temp = input.readLine();
				 if(temp != null)
				 {
					 if(temp.isEmpty())
					 {
						 //System.out.println("Blank" + temp);
						 temp = input.readLine();
						 output.printf(temp+"\n");
					 }	 
					 else
					 {
						 temp = new StringBuilder(temp).reverse().toString();
						 temp = temp.replace("\n", "").replace("\r", "");
						 //System.out.println(temp);
						 output.printf(temp+"\n");
					 }
				 }
			 }
         }
		 catch (IOException e)
		 {
			e.printStackTrace();
		 } 
	}
}
