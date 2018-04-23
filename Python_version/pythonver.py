import socket                   # Import socket module
import mazeroute2 as mazeroute  # Import the predefined route
import threading                # Import threading module

class servermain:
    ##attributes
    PORT_SERVER = 6532#0x1984 #// We define a port that we are going to use.
    PORT_CLIENT = 6533#0x1985 #// We define a port that we are going to use.
    host_c = '127.0.0.1'#IP_ADDR #address of the client
    host_s = '127.0.0.1'#address of the server
    #

    #bare minimum student information needed to communicate with the server
    STUDENT_NUMBER      ="17024721" #student number
    STUDENT_FIRSTNAME   ="Robert"   #name
    STUDENT_FAMILYNAME  ="Painter"  #
    #
    
    ##methods
    #main loop for sending data to the server (thread 1)
    def mainloop(self):
        ssrv = socket.socket(   socket.AF_INET,     # Internet,UDP
                                socket.SOCK_DGRAM)  # Create a socket object
        #connect
        #scli.connect((host, PORT_CLIENT))
        #ssrv.bind((self.host_s, self.PORT_SERVER)) # Bind to the port
        

        #register student number and name to the client to buld maze serverside
        RegisterString = ("Register  "+ str(self.STUDENT_NUMBER)    +" "+
                                        str(self.STUDENT_FIRSTNAME) +" "+
                                        str(self.STUDENT_FAMILYNAME)).encode('utf-8')
        
        ssrv.sendto(RegisterString,(self.host_s, self.PORT_SERVER))         #send the registration request
        print('sent',RegisterString,'ports',(self.host_s, self.PORT_SERVER))#debug (print what was sent)

        #here we keep sending the data to the server, as no response is required we can send it all at once
        for x in range(0,len(mazeroute.mvkeyarray),2):#for each item in the array,steps set to 2
            senddata = "Option "+str(mazeroute.mvkeyarray[x])+", "+str(mazeroute.mvkeyarray[x+1])#build the option string
            print('SENT['+str(self.host_s)+']: ',end='')#print what was sent
            print(senddata)#debug
            ssrv.sendto(senddata.encode('utf-8'),(self.host_s, self.PORT_SERVER))#send to the server
        print('sent num of moves: '+str(len(mazeroute.mvkeyarray)/2))
    ##recieve method for thread 2     
    def recieveloop(self):
        scli = socket.socket(   socket.AF_INET,     # Internet,UDP
                                socket.SOCK_DGRAM)  # Create a socket object
        scli.bind((self.host_c, self.PORT_CLIENT))  #bind the socket to a port

        while (True):
            message, address = scli.recvfrom(1024)      #recieve forever
            print("RECIEVED["+str(address)+"]: ",end='')#print message recieved
            print(message)
        #scli.close()
        #ssrv.close()
            
    def initialise(self):#threading initialisiation
        t1 = threading.Thread(target = self.mainloop)#set up threads for sending/recieving data
        t2 = threading.Thread(target = self.recieveloop)

        t1.start()#start both threads
        t2.start()

        t1.join()#wait for them to finish
        t2.join()



c=servermain()#instanciate class
#c.mainloop()#do the process
c.initialise()#initialise threads and do the processes
print('DONE!')
