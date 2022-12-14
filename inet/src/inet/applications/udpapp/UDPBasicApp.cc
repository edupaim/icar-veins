//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2004,2011 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/applications/udpapp/UDPBasicApp.h"

#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/transportlayer/contract/udp/UDPControlInfo_m.h"

namespace inet {

Define_Module(UDPBasicApp);

simsignal_t UDPBasicApp::sentPkSignal = registerSignal("sentPk");
simsignal_t UDPBasicApp::rcvdPkSignal = registerSignal("rcvdPk");

UDPBasicApp::~UDPBasicApp() {
    cancelAndDelete(selfMsg);
}

void UDPBasicApp::initialize(int stage) {
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);

        udpModuleName = par("udpModuleName");
        sendRandom = par("sendRandom");
        localPort = par("localPort");
        nodesAmount = par("nodesAmount");
        destPort = par("destPort");
        startTime = par("startTime").doubleValue();
        stopTime = par("stopTime").doubleValue();
        packetName = par("packetName");
        if (stopTime >= SIMTIME_ZERO&& stopTime < startTime)
        throw cRuntimeError("Invalid startTime/stopTime parameters");
        selfMsg = new cMessage("sendTimer");

        hopCountStats.setName("hopCountStats");
        hopCountVector.setName("HopCount");
    }
}

void UDPBasicApp::finish() {
    recordScalar("packets sent", numSent);
    recordScalar("packets received", numReceived);
    hopCountStats.recordAs("hop count");
    ApplicationBase::finish();
}

void UDPBasicApp::setSocketOptions() {
    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
        socket.setTimeToLive(timeToLive);

    int typeOfService = par("typeOfService");
    if (typeOfService != -1)
        socket.setTypeOfService(typeOfService);

    const char *multicastInterface = par("multicastInterface");
    if (multicastInterface[0]) {
        IInterfaceTable *ift = getModuleFromPar<IInterfaceTable>(
                par("interfaceTableModule"), this);
        InterfaceEntry *ie = ift->getInterfaceByName(multicastInterface);
        if (!ie)
            throw cRuntimeError(
                    "Wrong multicastInterface setting: no interface named \"%s\"",
                    multicastInterface);
        socket.setMulticastOutputInterface(ie->getInterfaceId());
    }

    bool receiveBroadcast = par("receiveBroadcast");
    if (receiveBroadcast)
        socket.setBroadcast(true);

    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");
    if (joinLocalMulticastGroups) {
        MulticastGroupList mgl = getModuleFromPar<IInterfaceTable>(
                par("interfaceTableModule"), this)->collectMulticastGroups();
        socket.joinLocalMulticastGroups(mgl);
    }
}

L3Address UDPBasicApp::chooseDestAddr() {

//  Implementacao para envio aleatorio
    if (sendRandom) {
        cTopology *topo = new cTopology("topo");
        EV_INFO << "MODULE PATH: " << getFullPath() << endl;
        topo->extractByNedTypeName(
                cStringTokenizer(getNedTypeName()).asVector());
        int numOfNodes = topo->getNumNodes();
        EV_INFO << "NUM of NODES: " << numOfNodes << endl;
        bool isAlone = false;
        bool isSame = true;
        std::string moduleName = "";
        while (isSame && !isAlone) {
            int k = intrand(numOfNodes);
            std::string fullPathName =
                    topo->getNode(k)->getModule()->getFullPath();
            std::size_t from_i = fullPathName.find("node");
            std::size_t to_i = fullPathName.find(udpModuleName);
            moduleName = fullPathName.substr(from_i, to_i - from_i);
            EV_INFO << "FOUND module: " << moduleName << endl;
            isSame = getFullPath().find(moduleName) != std::string::npos;
            EV_INFO << "IS THE SAME module: " << isSame << endl;
            isAlone = numOfNodes == 1;
        }
        std::string addrNode = moduleName + "(ipv4)";
        L3Address result;
        L3AddressResolver().tryResolve(addrNode.c_str(), result);
        return result;
    }
    int k = intrand(destAddresses.size());
    if (destAddresses[k].isLinkLocal()) {    // KLUDGE for IPv6
        const char *destAddrs = par("destAddresses");
        cStringTokenizer tokenizer(destAddrs);
        const char *token = nullptr;

        for (int i = 0; i <= k; ++i)
            token = tokenizer.nextToken();
        destAddresses[k] = L3AddressResolver().resolve(token);
    }
    return destAddresses[k];
}

void UDPBasicApp::sendPacket() {
    std::ostringstream str;
    str << packetName << "-" << numSent;
    ApplicationPacket *payload = new ApplicationPacket(str.str().c_str());
    payload->setByteLength(par("messageLength").longValue());
    payload->setSequenceNumber(numSent);

    L3Address destAddr = chooseDestAddr();

    emit(sentPkSignal, payload);
    socket.sendTo(payload, destAddr, destPort);
    numSent++;
}

void UDPBasicApp::processStart() {
    socket.setOutputGate(gate("udpOut"));
    const char *localAddress = par("localAddress");
    socket.bind(
            *localAddress ?
                    L3AddressResolver().resolve(localAddress) : L3Address(),
            localPort);
    setSocketOptions();

    const char *destAddrs = par("destAddresses");

    cStringTokenizer tokenizer(destAddrs);
    const char *token;

    while ((token = tokenizer.nextToken()) != nullptr) {
        L3Address result;
        L3AddressResolver().tryResolve(token, result);
        if (result.isUnspecified())
            EV_ERROR << "cannot resolve destination address: " << token << endl;
        else
            destAddresses.push_back(result);
    }

    if (!destAddresses.empty()) {
        selfMsg->setKind(SEND);
        processSend();
    } else {
        if (stopTime >= SIMTIME_ZERO) {
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
    }
}

void UDPBasicApp::processSend() {
    sendPacket();
    simtime_t d = simTime() + par("sendInterval").doubleValue();
    if (stopTime < SIMTIME_ZERO|| d < stopTime) {
        selfMsg->setKind(SEND);
        scheduleAt(d, selfMsg);
    }
    else {
        selfMsg->setKind(STOP);
        scheduleAt(stopTime, selfMsg);
    }
}

void UDPBasicApp::processStop() {
    socket.close();
}

void UDPBasicApp::handleMessageWhenUp(cMessage *msg) {
    if (msg->isSelfMessage()) {
        ASSERT(msg == selfMsg);
        switch (selfMsg->getKind()) {
        case START:
            processStart();
            break;

        case SEND:
            processSend();
            break;

        case STOP:
            processStop();
            break;

        default:
            throw cRuntimeError("Invalid kind %d in self message",
                    (int) selfMsg->getKind());
        }
    } else if (msg->getKind() == UDP_I_DATA) {
        // process incoming packet
        processPacket(PK(msg));
    } else if (msg->getKind() == UDP_I_ERROR) {
        EV_WARN << "Ignoring UDP error report\n";
        delete msg;
    } else {
        throw cRuntimeError("Unrecognized message (%s)%s", msg->getClassName(),
                msg->getName());
    }
}

void UDPBasicApp::refreshDisplay() const {
    char buf[100];
    sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
    getDisplayString().setTagArg("t", 0, buf);
}

void UDPBasicApp::processPacket(cPacket *pk) {
    UDPDataIndication *ctrl = check_and_cast<UDPDataIndication *>(
            pk->getControlInfo());
    int ttl = ctrl->getTtl();
    emit(rcvdPkSignal, pk);
    EV_INFO << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk)
                   << endl;
    delete pk;
    hopCountVector.record(32 - ttl);
    hopCountStats.collect(32 - ttl);
    numReceived++;
}

bool UDPBasicApp::handleNodeStart(IDoneCallback *doneCallback) {
    simtime_t start = std::max(startTime, simTime());
    if ((stopTime < SIMTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime)) {
        selfMsg->setKind(START);
        scheduleAt(start, selfMsg);
    }
    return true;
}

bool UDPBasicApp::handleNodeShutdown(IDoneCallback *doneCallback) {
    if (selfMsg)
        cancelEvent(selfMsg);
    //TODO if(socket.isOpened()) socket.close();
    return true;
}

void UDPBasicApp::handleNodeCrash() {
    if (selfMsg)
        cancelEvent(selfMsg);
}

} // namespace inet

