#include "mbed.h"
#include "SPI.h"
#include "MQTTEthernet.h"
#include "MQTTClient.h"

#define ECHO_SERVER_PORT   7
 
Serial pc(USBTX, USBRX);   //Enabling the Serial transmission between WIZ750SR and PC.
Serial serial(D1,D0);      // Enabling Serial transmission betwenn Wiz750SR and W7500. 
char c[100]="";

 
int main(void) {
    printf("Wait a second...\r\n");
    char* topic = "ack";                    //if we are subscribing the acknowledgement.
    MQTTEthernet ipstack = MQTTEthernet();
    
    MQTT::Client<MQTTEthernet, Countdown> client = MQTT::Client<MQTTEthernet, Countdown>(ipstack);
    
    char* hostname = "iot.eclipse.org";   //Give the IP Address of the MQTT Broker.
    int port = 1883;                  // Port number of the MQTT broker.  
    
 
    int rc = ipstack.connect(hostname, port);
    if (rc != 0)
    printf("rc from TCP connect is %d\n", rc);
    printf("Topic: %s\r\n",topic);
    
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
    data.MQTTVersion = 3;
    data.clientID.cstring = "parents";

    if ((rc = client.connect(data)) == 0)
    printf("rc from MQTT connect is %d\n", rc);
   
                              
   while (true) {
    if (serial.readable()) // if anything available/ readable in Serial port 
    {
        int i;
        c[i]=0;
        for(i=0;i<=10;i++){
        char c1=serial.getc();
        c[i] = c1;
    } 
     pc.printf("The value returned is %s ",c);
     MQTT::Message message;
     char buf[100];
     sprintf(buf, "%s", c);
     message.qos = MQTT::QOS0;
         message.retained = false;
         message.dup = false;

         message.payload = (void*)c;
         message.payloadlen = strlen(c);

         rc = client.publish("balance", message);
         pc.printf("Rc result: %c \n ",rc);
         client.yield(60);
    }
}
}
