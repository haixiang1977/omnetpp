/*
 * tictoc10.cc
 *
 *  Created on: Nov 6, 2022
 *      Author: HaiXiang.Liu
 */


#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc10 : public cSimpleModule
{
protected:
    virtual void forwardMessage(cMessage* msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
};

Define_Module(Txc10);

void Txc10::initialize()
{
    if (getIndex() == 0) {
        // boot the process scheduling the initial message as self message
        char msgName[20];
        sprintf(msgName, "tic-%d", getIndex());
        cMessage* msg = new cMessage(msgName);
        scheduleAt(0.0, msg);
    }
}

void Txc10::handleMessage(cMessage* msg)
{
    if (getIndex() == 3) {
        // Message arrived
        EV << "Message " << msg << " arrived.\n";
        delete msg;
    } else {
        // we need to formward message
        forwardMessage(msg);
    }
}

void Txc10::forwardMessage(cMessage* msg)
{
    // pick a random gateway between 0 and gateSize("out") - 1
    int n = gateSize("out");
    int k = intuniform(0, n - 1);
    EV << "forward message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}
