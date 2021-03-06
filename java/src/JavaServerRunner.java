import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class JavaServerRunner implements Runnable{

	/**
	 * Class that creates a thread which runs javaServer in order to accept multiple
	 * connections at once.
	 * @author Darnell Martin & Stephen Clabaugh
	 */
	@Override
	public void run()
    {
        ServerSocket listener;
		try {

			listener = new ServerSocket(8000);
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
		catch (IOException e)
		{
			e.printStackTrace();
		}

    }



}
