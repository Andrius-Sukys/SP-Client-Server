Socket Programming for Windows. The Program sends a Message from Client 1 to Client 2 through Server 1 and Server 2.

To COMPILE the Program:
1. Go to the directory where the Project Files are located.
2. Compile server.c file by typing "gcc server.c -o server.exe -lws2_32" in your Terminal.
3. Compile client.c file by typing "gcc client.c -o client.exe -lws2_32" in your Terminal.

To RUN the Program (explanation):
1. The first active instance has to be Server 1 (S1). Launch server.exe by typing ".\server s1 port1". (port1 > 1024!):
   Example: .\server s1 2000
   Output:
   WSAStartup successful.

   RESULT
   Address family: 2
   Socket type: 1
   Protocol: 0
   Canonical name: (null)
   Socket address: [127.0.0.1]:2000

   Waiting for Client 1. Listening..."
   - - - - - - - - - - - - - - - - - -
2. The second active instance has to be Client 1 (C1) that Server 1 (S1) is waiting for. Launch client.exe by typing ".\client c1 port2" (port2 = port1!):
   Example: .\client c1 2000
   Output:
   WSAStartup successful.

   RESULT
   Address family: 2
   Socket type: 1
   Protocol: 0
   Canonical name: (null)
   Socket address: [127.0.0.1]:2000
    
   Connected.
    
   Enter a line to send:
   - - - - - - - - - - -
   The User is now prompted to enter a line of text that should be sent. Enter a message and press "Enter".
   Example: I am Client 1. How are you doing, Client 2?
   Output:
   Sent message: I am Client 1. How are you doing, Client 2?
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
3. S1 has established a connection with C1: "Connection with Client 1 established."
   Now S1 is waiting for another input for port that Server 2 (S2) could join on: "Enter a new port (>1024):". The User must input that port number, let's say that port3 = 3000.
   Example: 3000
   Output:
   RESULT
   Address family: 2
   Socket type: 1
   Protocol: 0
   Canonical name: (null)
   Socket address: [127.0.0.1]:3000
    
   Waiting for Server 2. Listening...
   - - - - - - - - - - - - - - - - - -
4. The third active instance has to be Server 2 (S2) that Server 1 (S1) is waiting for. Launch server.exe by typing ".\server s2 port4" (port4 = port3!):
   Example: .\server s2 3000
   Output:
   WSAStartup successful.

   Enter a new port (>1024):
   - - - - - - - - - - - - -
   Now S2 is waiting for another input for port that Client 2 (C2) could join on: "Enter a new port (>1024):". The User must input that port number, let's say that port5 = 4000.
   Example: 4000
   Output:
   RESULT
   Address family: 2
   Socket type: 1
   Protocol: 0
   Canonical name: (null)
   Socket address: [127.0.0.1]:4000
    
   Waiting for Client 2. Listening...
   - - - - - - - - - - - - - - - - - -
5. The fourth active instance has to be Client 2 (C2) that Server 2 (S2) is waiting for. Launch client.exe by typing ".\client c2 port6" (port6 = port5!):
   Example: .\client c2 4000
   Output:
   WSAStartup successful.

   RESULT
   Address family: 2
   Socket type: 1
   Protocol: 0
   Canonical name: (null)
   Socket address: [127.0.0.1]:4000
    
   Connected.
    
   Waiting for the message from Server 2.
    
   Received message: I am Client 1. How are you doing, Client 2?

Finally, Client 2 has received Client 1's message!
