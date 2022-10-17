/*
 * titoc8.cc
 *
 *  Created on: Oct 17, 2022
 *      Author: HaiXiang.Liu
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Tic8 : public cSimpleModule
{
private:
    simtime_t timeout;
    cMessage *timeoutEvent;

public:
    Tic8();
    virtual ~Tic8();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
};

Define_Module(Tic8);

Tic8::Tic8()
{
    timeoutEvent = nullptr;
}

Tic8::~Tic8()
{
    cancelAndDelete(timeoutEvent);
}

void Tic8::initialize()
{
    EV << "Tic8::initialize()\n";
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");
    cMessage* msg = new cMessage("titocMsg");
    send(msg, "out");
    // start timeout event 1 sec now
    scheduleAt(simTime() + timeout, timeoutEvent);
}

void Tic8::handleMessage(cMessage* msg)
{
    if (msg == timeoutEvent) {
        EV << "Timeout expired, re-send the packet\n";
        cMessage* newmsg = new cMessage("titocMsg");
        send(newmsg, "out");
        scheduleAt(simTime() + timeout, timeoutEvent);
    } else {
        // we get message, cancel timer
        EV << "Cancel timer\n";
        cancelEvent(timeoutEvent);
        delete msg;

        // ready to send another one
        cMessage* newmsg = new cMessage("titocMsg");
        send(newmsg, "out");
        scheduleAt(simTime() + timeout, timeoutEvent);
    }
}

class Toc8 : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
};

Define_Module(Toc8);

void Toc8::initialize()
{
    EV << "Toc8::initialize()\n";
}

void Toc8::handleMessage(cMessage* msg)
{
    if (uniform(0, 1) < 0.1) {
        EV << "\"Losing\" message.\n";
        delete msg;
    } else {
        EV << "Sending back same message as acknowledgement.\n";
        send(msg, "out");
    }
}

