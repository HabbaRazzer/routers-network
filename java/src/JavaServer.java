import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
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
			 InputStream stream = socket.getInputStream();
			 byte[] data = new byte[5];
			 int count = stream.read(data);
			 
			 byte checksum = (byte) ~data[2];
			 
			 //String route = getRoute(data[1]);
			 
			 //System.out.println(route +" "+"Here");
			 
		     //Socket s = new Socket(route, 8000);

		     //PrintWriter output = new PrintWriter(s.getOutputStream(), true);

		     //output.print(data);
		     //output.flush();
			 System.out.print((char)data[0]);
			 System.out.print((char)data[1]);

		     //s.close();
			 //Define the PrintWriter to write output to the server
			 //PrintWriter output = new PrintWriter(this.socket.getOutputStream(), true);
		     //Define the BufferedReader to get the input from the server
			 //BufferedReader input = new BufferedReader(new InputStreamReader(this.socket.getInputStream()));
			 //System.out.println("Testing");
			 //while(input != null)
			 //{
				 //String temp = input.readLine();
				 //System.out.println("Blank" + temp);
				 //if(temp != null)
				 //{
					 //if(temp.isEmpty())
					 //{
					//	 System.out.println("Blank" + temp);
					 //}
					 //else
					// {
						// System.out.println(temp.);
					// }
				// }
				//		 temp = input.readLine();
				//		 output.printf(temp+"\n");
				//	 }	 
				//	 else
				//	 {
				//		 temp = new StringBuilder(temp).reverse().toString();
				////		 temp = temp.replace("\n", "").replace("\r", "");
						 //System.out.println(temp);
				//		 output.printf(temp+"\n");
				//	 }
				 //}
			// }
         }
		 catch (IOException e)
		 {
			e.printStackTrace();
		 } 
	}
}
