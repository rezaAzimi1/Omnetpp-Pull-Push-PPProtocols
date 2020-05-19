#include <omnetpp.h>
#include <string.h>
#include <list>
#include <ctime>
using namespace omnetpp;
#include <iostream>
using namespace std;
class Device : public cSimpleModule
{
   /* This boolean flag shows whether the node with the same index as its number has received the
     * data packet or not.
    */
   bool flag=false;
   int numberOfMsg=0;
   /* This boolean flag shows whether the node with the same index as its number has received the
    * data packet or not.
    */
   int portMap[11][11] = {{6,6,6,6,6,6,6,6,6,6,6},
                          {6,6,0,6,1,6,6,6,6,6,6},
                          {6,0,6,2,6,6,6,6,6,1,6},
                          {6,6,0,6,2,6,6,4,3,1,6},
                          {6,0,6,1,6,6,6,2,6,6,3},
                          {6,6,6,6,6,6,0,1,6,6,2},
                          {6,6,6,6,6,2,6,1,0,6,6},
                          {6,6,6,4,2,5,0,6,3,6,1},
                          {6,6,6,0,6,6,1,2,6,6,6},
                          {6,6,1,0,6,6,6,6,6,6,6},
                          {6,6,6,6,1,0,6,2,6,6,6}};
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

Define_Module(Device);
// endedDev has number of last finished Device
long double  duplicatePacketCount = 0;
int endedDev=0;
float sumOfnumberOfMsg=0;
float EndToEndSum=0;

/* First element: current device number
 * Second element: required device number --> port number for required device
 */



void Device::initialize()
{
    this->flag=false;
    // Device01 has the final message.
    // The initial message is sent from device01 to other devices.
    if (strcmp("device01", getName()) == 0) {

        // Set the flag of device01 to true.
        this->flag=true;

        // The number in the message is the device's number.
        cMessage *msg = new cMessage("Hmsg01");

        // Send the message from all of the out ports of device01.
        int n = gateSize("out");
        for (int j = 0; j < n; j++)
        {
            cMessage *copy = msg -> dup();
            //
            const long double sysTime = time(0);
            const long double sysTimeMS = sysTime*1000;
            std::string textMsg=msg -> getName();
            textMsg+=to_string(sysTimeMS);
            copy -> setName(textMsg.c_str());
            //

            send(copy, "out", j);
        }
    }
}

void Device::handleMessage(cMessage *msg)
{
    std::string msgHeader=msg -> getName();
    std::string token = msgHeader.substr(6,msgHeader.length());
    const long double sysTime = time(0);
    const long double sysTimeMS = sysTime*1000;
    EndToEndSum=(EndToEndSum+(sysTimeMS-stod(token)));

    this->numberOfMsg++;
    /* Find the receiver device number. The two ending characters of the name
     * show the device number.
     */
    std::string strIndex(getName());
    int leftDigit = strIndex.at(6) - 48;
    int rightDigit = strIndex.at(7) - 48;
    int deviceIndex = (leftDigit * 10) + rightDigit;

    // Convert the message into a character array.
    std::string strIndexa(msg -> getName());

    // Determine the message type that the sender sends.
    char type = strIndexa.at(0);

    /* Determine the device number of the sender. The two ending characters of the message
     * show the device number.
     */
    leftDigit = strIndexa.at(4) - 48;
    rightDigit = strIndexa.at(5) - 48;
    int senderIndex = (leftDigit * 10) + rightDigit;

        // Sender says: I have the message if you don't have it, I'll send it to you.
        // H --> have message
        if(type=='H' && !this->flag) {

            // The receiver sends a get message to inform the sender to send the message.
            // G --> get message
            std::string textMsg = "Gmsg";

            // Attach the device number to the message.
            if(deviceIndex < 10) {
                textMsg += std::to_string(0);
            }
            textMsg += std::to_string(deviceIndex);

            // Send the message.
            cMessage *Gmsg = new cMessage(textMsg.c_str());
            //
            const long double sysTime = time(0);
            const long double sysTimeMS = sysTime*1000;
            std::string textMsgGmsg=Gmsg -> getName();
            textMsgGmsg+=to_string(sysTimeMS);
            Gmsg -> setName(textMsgGmsg.c_str());
            //
            send(Gmsg, "out", portMap[deviceIndex][senderIndex]);

         // Sender says: I don't have the message so send it to me.
        // G = give your message
        } else if(type=='G') {

            /* The receiver sends the final message which is a respond to the
             * get message.
             * F --> final
             */
            std::string textMsg = "Fmsg";

            // Attach the device's number to the end of the message.
            if(deviceIndex < 10) {
                textMsg += std::to_string(0);
            }
            textMsg += std::to_string(deviceIndex);

            // Send the "final message".
            cMessage *Fmsg = new cMessage(textMsg.c_str());
            //
            const long double sysTime = time(0);
            const long double sysTimeMS = sysTime*1000;
            std::string textMsgFmsg=Fmsg -> getName();
            textMsgFmsg+=to_string(sysTimeMS);
            Fmsg -> setName(textMsgFmsg.c_str());
            //
            send(Fmsg, "out", portMap[deviceIndex][senderIndex]);

        /* Sender Sends the final message and the receiver multicasts "have message"
         * to its adjacent nodes.
         * F --> Final message */
        } else if(type == 'F') {
            if (this->flag){
                        duplicatePacketCount++;
            }
            this->flag = true;
            int n = gateSize("out");

            // Have message.
            std::string textMsg = "Hmsg";

            // Attach the device number to the end of the message.
            if(deviceIndex < 10) {
                textMsg += std::to_string(0);
            }
            textMsg += std::to_string(deviceIndex);

            // Send the "have message" to the adjacent nodes.
            for(int j = 0; j < n; j++) {
                    cMessage *Hmsg = new cMessage(textMsg.c_str());
                    //
                    const long double sysTime = time(0);
                    const long double sysTimeMS = sysTime*1000;
                    std::string textMsg=Hmsg -> getName();
                    textMsg+=to_string(sysTimeMS);
                    Hmsg -> setName(textMsg.c_str());
                    //
                    send(Hmsg, "out", j);
            }
        }
}
void Device::finish(){
    endedDev++;
    sumOfnumberOfMsg+=this->numberOfMsg;
    float avgOfnumberOfMsg=sumOfnumberOfMsg/10;
    float EndToEndAvg=EndToEndSum/sumOfnumberOfMsg;
    if (endedDev == 10) {
        EV<<"duplicate Packet Count:" << duplicatePacketCount<<"\n";
        EV<<"sum of all messages in network:"<<sumOfnumberOfMsg<<"\n";
        EV<<"average of messages for each node:"<<avgOfnumberOfMsg<<"\n";
        EV<< "sum point to point time of messages:" << EndToEndSum<<"\n";
        EV<< "average point to point time of messages:" << EndToEndAvg;
    }
}
