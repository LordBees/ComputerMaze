#!/usr/bin/python           # This is client.py file

import socket               # Import socket module
import mazeroute2 as mazeroute



#scli = socket.socket()         # Create a socket object
#ssrv = socket.socket()         # Create a socket object

##setup



##code



##end code


#host = socket.gethostname() # Get local machine name
#port = 12345                # Reserve a port for your service.
#host=
#port
class servermain:
    #attr
    PORT_SERVER = 6532#0x1984 #// We define a port that we are going to use.
    PORT_CLIENT = 6533#0x1985 #// We define a port that we are going to use.
    host_c = '127.0.0.1'#IP_ADDR
    host_s = '127.0.0.1'
    #
    STUDENT_NUMBER      ="17024721"
    STUDENT_FIRSTNAME   ="Robert"
    STUDENT_FAMILYNAME  ="Painter"
    #
    
    ##meth
    def mainloop(self):
        #
        scli = socket.socket(   socket.AF_INET, # Internet
                                socket.SOCK_DGRAM)         # Create a socket object
        ssrv = socket.socket(   socket.AF_INET, # Internet
                                socket.SOCK_DGRAM)         # Create a socket object
        #connect
        #scli.connect((host, PORT_CLIENT))
        #ssrv.bind((self.host_s, self.PORT_SERVER))        # Bind to the port
        scli.bind((self.host_c, self.PORT_CLIENT))
        #ssrv.listen(5)                 # Now wait for client connection.
        ##
        RegisterString = ("Register  "+str(self.STUDENT_NUMBER)+" "+str(self.STUDENT_FIRSTNAME)+" "+str(self.STUDENT_FAMILYNAME)).encode('utf-8')
        ssrv.sendto(RegisterString,(self.host_s, self.PORT_SERVER))
        print('sent',RegisterString,'ports',(self.host_s, self.PORT_SERVER))
        #scli.connect((self.hostcli, self.PORT_CLIENT))#now binds
        ##setup server
        
        #while (True):#(input() != 'xit'):
            #contents, addr = scli.recvfrom(1024)#scli.accept()     # Establish connection with client.
            #print ('Got connection from', addr)
            #print (contents)#(scli.recv(1024)) 
            ##
        #f = open("mazeroute.py")
        #xdat = f.readlines()
        #f.close()
        
        #for x in range(0,len(xdat),2):
        #    pass
        
        for x in range(0,len(mazeroute.mvkeyarray),2):
            senddata = "Option "+str(mazeroute.mvkeyarray[x])+", "+str(mazeroute.mvkeyarray[x+1])
            print(senddata)
            ssrv.sendto(senddata.encode('utf-8'),(self.host_s, self.PORT_SERVER))

        #while(True)
            
        
        #scli.close()
        #ssrv.close()
           



    #host = socket.gethostname() # Get local machine name
    #port = 12345                # Reserve a port for your service.






#while True:
#   c, addr = s.accept()     # Establish connection with client.
#   print 'Got connection from', addr
#   c.send('Thank you for connecting')
#   c.close()                # Close the connection
c=servermain()
c.mainloop()
