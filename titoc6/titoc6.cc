/*
 * titoc6.cc
 *
 *  Created on: Oct 9, 2022
 *      Author: HaiXiang.Liu
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc6 : public cSimpleModule // defined in titoc6.ned
{
private:
    cMessage* event;
    cMessage* tictocMsg;

public:
    Txc6();
    ~Txc6();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
};

Define_Module(Txc6);

Txc6::Txc6()
{
    event = tictocMsg = nullptr;
}

Txc6::~Txc6()
{
    cancelAndDelete(event);
    delete tictocMsg;
}

void Txc6::initialize()
{
    // use for timing
    event = new cMessage("event");

    tictocMsg = nullptr;

    if (par("sendMsgOnInit").boolValue() == true) {
        EV << "Scheduling first to 5s\n";
        tictocMsg = new cMessage("titocMsg");
        scheduleAt(5.0, event);
    }
}

void Txc6::handleMessage(cMessage* msg)
{
    // check the message is event or the message from peers
    if (msg == event) {
        EV << "wait period is over, sending back message\n";
        send(tictocMsg, "out");
        tictocMsg = nullptr; // message out
    } else {
        EV << "message arrived, starting to schedule 1s";
        tictocMsg = msg;
        scheduleAt(simTime() + 1.0, event);
    }
}
