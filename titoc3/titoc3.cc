/*
 * titoc3.cc
 *
 *  Created on: Oct 9, 2022
 *      Author: HaiXiang.Liu
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

/*
 * in this case, use a counter and delete message after 10 exchanges
 */
class Txc3 : public cSimpleModule
{
private:
    int counter;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
};

Define_Module(Txc3);

void Txc3::initialize() {
    counter = 10;
    // the WATCH() examine the variable under Tkenv
    WATCH(counter);

    if (strcmp("tic", getName()) == 0) {
        EV << "Sending initial message";
        cMessage *msg = new cMessage("tictoMsg");
        send(msg, "out");
    }
}

void Txc3::handleMessage(cMessage* msg) {
    counter --;
    if (counter == 0) {
        EV << getName() << "'s counter reached zero, deleting message";
        delete msg;
    } else {
        EV << getName() << "'s counter is " << counter << ", sending back msg.";
        send(msg, "out");
    }
}
