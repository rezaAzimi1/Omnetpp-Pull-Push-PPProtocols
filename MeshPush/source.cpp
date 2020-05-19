#include <windows.h>

#include <string.h>
#include <list>
#include <omnetpp.h>
#include <pthread.h>
#include <ctime>
using namespace omnetpp;
#include <iostream>
using namespace std;

class Device : public cSimpleModule
{
  bool flag;
  int numberOfMsg=0;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};
long double duplicatePacketCount = 0;
long double startTimer,avgTimer;
// endedDev has number of last finished Device
int endedDev=0;
float sumOfnumberOfMsg=0;
Define_Module(Device);
std::string delimiter = "G";
/* This array shows whether a device of that number as the same as the index
 * has received the packet or not. */

float EndToEndSum=0;
void Device::initialize()
{
        this->flag=false;
        //from device2 to device6
        if (strcmp("device01", getName()) == 0) {
            //Receiver node is device6
                this->flag = 1;

                cMessage *msg = new cMessage("MSG");
                int n = gateSize("out");

                for (int j = 0; j < n; j++)
                {
                    cMessage *copy = msg->dup();
                    //
                    simtime_t simt = simTime()*1000;
                    std::string simtStr=simt.str();
                    std::string textMsg="MSG";
                    textMsg+=simtStr;
                    copy -> setName(textMsg.c_str());
                    //
                    send(copy, "out", j);
                }
        }
}
void Device::handleMessage(cMessage *msg)
{
    std::string msgHeader=msg -> getName();
    std::string token = msgHeader.substr(msgHeader.find(delimiter)+1,msgHeader.length());
    simtime_t simtt = simTime()*1000;
    std::string sysTimeMS=simtt.str();
    EndToEndSum=(EndToEndSum+(stoi(sysTimeMS)-stoi(token)));

    this->numberOfMsg++;
    /* If the receiver does not have the message then sets its flag to true
     * and sends it out from all of its out ports.
     */
    if(!this->flag) {

        this->flag=true;
        //msg -> setName("M2");
        int n = gateSize("out");
        for(int j = 0; j < n; j++) {
                cMessage *copy = msg->dup();
                //
                simtime_t simt = simTime()*1000;
                std::string simtStr=simt.str();
                std::string textMsg="MSG";
                textMsg+=simtStr;
                copy -> setName(textMsg.c_str());
                //
                send(copy, "out", j);
        }

    // Otherwise the number of duplicate packets sent must get incremented.
    } else {

        duplicatePacketCount++;

    }
}
void Device::finish(){
    endedDev++;
    sumOfnumberOfMsg+=this->numberOfMsg;
    float avgOfnumberOfMsg=sumOfnumberOfMsg/10;
    float EndToEndAvg=EndToEndSum/sumOfnumberOfMsg;
    if (endedDev==10) {
        EV<<"duplicate Packet Count:" << duplicatePacketCount<<"\n";
        EV<<"sum of all messages in network:"<<sumOfnumberOfMsg<<"\n";
        EV<<"average of messages for each node:"<<avgOfnumberOfMsg<<"\n";
        EV<< "sum point to point time of messages:" << EndToEndSum<<"\n";
        EV<< "average point to point time of messages:" << EndToEndAvg;

    }
}
