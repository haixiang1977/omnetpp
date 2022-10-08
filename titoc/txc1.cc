/*
 * txc1.cc
 *
 *  Created on: Oct 8, 2022
 *      Author: HaiXiang.Liu
 */

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * Derive the Txc1 class from cSimple Module. In the Tictoc1 network,
 * both the tic and toc modules are Txc1 objects, created by OMNet++
 * at the beginning of the simulation
 */
class Txc1 : public cSimpleModule
{
protected:
    // the following redefined virtual function holds the algorithm
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The modules needs to be registered with OMNet++
Define_Module(Txc1);

void Txc1::initialize()
{
    // initialization is called at the beginning of the simulation
    // to bootstrap the tic-toc-tic-toc process, one of the modules needs to send the first message
    // let this be 'tic' to send 1st message
    if (strcmp("tic", getName()) == 0) {
        cMessage *msg = new cMessage("tictocMsg");
        send(msg, "out");
    }
}

void Txc1::handleMessage(cMessage *msg)
{
    // the handle message is called whenever a message arrives
    // at module. here we just send to the other module, through
    // gate 'out'
    send(msg, "out");
}
