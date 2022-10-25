/*
 * titoc9.cc
 *
 *  Created on: Oct 25, 2022
 *      Author: HaiXiang.Liu
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Tic9: public cSimpleModule
{
private:
    simtime_t timeout;
    cMessage* timeoutEvent;
    int seq;
    cMessage* message;

public:
    Tic9();
    virtual ~Tic9();

protected:
    virtual cMessage* generateNewMessage();
    virtual void sendCopyOf(cMessage* msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
};

Define_Module(Tic9);

Tic9::Tic9()
{
    timeoutEvent = message = nullptr;
}

Tic9::~Tic9()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void Tic9::initialize()
{
    // initialize variable
    seq = 0;
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");

    // generate and send initial message
    EV << "sending initial message\n";
    message = generateNewMessage();
    sendCopyOf(message);
    scheduleAt(simTime() + timeout, timeoutEvent);
}

void Tic9::handleMessage(cMessage* msg)
{
    if (msg == timeoutEvent) {
        EV << "Timeout expired, resending message and restarting timer\n";
        sendCopyOf(message);
        scheduleAt(simTime() + timeout, timeoutEvent);
    } else {
        // message arrived and ack received
        EV << "received: " << msg->getName() << "\n";
        delete msg;

        // cancel timeout event and delete the original message
        EV << "timer cancelled\n";
        cancelEvent(timeoutEvent);
        delete message;

        // ready to send new message
        message = generateNewMessage();
        sendCopyOf(message);
        scheduleAt(simTime() + timeout, timeoutEvent);
    }
}

cMessage* Tic9::generateNewMessage()
{
    char msgName[20];

    sprintf(msgName, "tic-%d", ++seq);
    cMessage* msg = new cMessage(msgName);
    return msg;
}

void Tic9::sendCopyOf(cMessage* msg)
{
    // duplicate message
    cMessage* copy = (cMessage*)msg->dup();
    send(copy, "out");
}

class Toc9 : public cSimpleModule
{
protected:
    virtual void handleMessage(cMessage* msg) override;
};

Define_Module(Toc9);

void Toc9::handleMessage(cMessage* msg)
{
    if (uniform(0, 1) < 0.1) {
        EV << "losing message " << msg << endl;
        bubble("message lost");
        delete msg;
    } else {
        EV << "msg received, sending back an ack\n";
        delete msg;
        send(new cMessage("ack"), "out");
    }
}

