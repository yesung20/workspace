import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class ChatServer {
    private static final int PORT = 12345;

    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Server is waiting for client connection...");

            // 클라이언트의 연결을 기다림
            Socket clientSocket = serverSocket.accept();
            System.out.println("Client connected!");

            // 메시지 보내기 위한 스레드
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
            Thread sendThread = new Thread(() -> {
                Scanner sc = new Scanner(System.in);
                String spend;
                while (true) {
                    System.out.print("server: ");
                    spend = sc.nextLine();
                    if (spend.equals("exit")) {
                        out.println("Server is closing the connection.");
                        break;
                    }
                    out.println(spend);
                }
                try {
                    clientSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            });
            sendThread.start();

            // 메시지 받기 위한 스레드
            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            Thread receiveThread = new Thread(() -> {
                String message;
                try {
                    while ((message = in.readLine()) != null) {
                        System.out.println("Received from client: " + message);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            });
            receiveThread.start();

            // 스레드가 종료될 때까지 대기
            sendThread.join();
            receiveThread.join();

        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}



                                              
                          
