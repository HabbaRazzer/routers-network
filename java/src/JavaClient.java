import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * Client to send and receive information to and from the server.
 * This client will send the String "HelloWorld" to the server and
 * wait to receive the reversed message from the server.
 * @author Darnell Martin & Stephen Clabaugh
 *
 */
public class JavaClient 
{

	    public static void main(String[] args) throws IOException 
	    {
	        String serverAddress = "127.0.0.1";
	        Socket s = new Socket(serverAddress, 8080);
	        //Define the PrintWriter to write output to the server
	        PrintWriter output = new PrintWriter(s.getOutputStream(), true);
	        //Define the BufferedReader to get the input from the server
	        BufferedReader input = new BufferedReader(new InputStreamReader(s.getInputStream()));
	        for(int i=0; i<10; i++)
	        {
	        	output.println("HelloWorld");
	        	String answer = input.readLine();
	        	System.out.println("Data Received: "+answer);
	        }
	        s.close();
	        System.exit(0);
	    }
}