//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <omnetpp.h>
#include <vector>

//=============================================================================================
#include "inet/common/INETDefs.h"
#include "inet/networklayer/contract/IRoutingTable.h"
#include "inet/networklayer/contract/ipv4/IPv4ControlInfo.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/transportlayer/udp/UDPPacket.h"

//=============================================================================================
#include "veins/base/phyLayer/BasePhyLayer.h"
#include <veins/modules/mobility/traci/TraCIScenarioManager.h>
#include "veins/base/modules/BaseModule.h"

//#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins_inet/modules/application/icarnew/CommState.h"

#include "veins_inet/modules/application/icarnew/KnownGlobal.h"
#include "veins_inet/modules/application/icarnew/LocalAgent.h"
#include "veins_inet/modules/application/icarnew/LocalMobility.h"
#include "veins_inet/modules/application/icarnew/messages/ICMessage_m.h"

#include "inet/networklayer/contract/IL3AddressType.h"

//using namespace omnetpp;

class Vision;
class Channel;

#ifndef __VEINS_ICARQOC_H_
#define __VEINS_ICARQOC_H_
#include <omnetpp.h>
#include <veins_inet/modules/application/icarnew/AgentPair.h>
#include <vector>
#include "veins/base/phyLayer/BasePhyLayer.h"
#include <veins/modules/mobility/traci/TraCIScenarioManager.h>
#include "veins/modules/mac/ieee80211p/Mac1609_4.h"
#include "veins/base/modules/BaseApplLayer.h"
#include <veins_inet/modules/application/icarnew/KnownGlobal.h>
#include "CommState.h"
#include "IcarContext.h"
#include "veins_inet/modules/application/icarnew/messages/ICMessage_m.h"
#include "veins_inet/modules/application/icarnew/messages/ICRMessage_m.h"
#include "LocalAgent.h"
#include "LocalMobility.h"
#include "veins_inet/modules/application/icarnew/messages/ICRNode.h"
#include "IcarContext.h"
#include "MessageManager.h"
#include "inet/networklayer/contract/INetfilter.h"

class IcarContext;
class IcarContext;
class AgentPair;
class ICMessage;
class MessageManager;

using namespace Veins;

/**
 * TODO - Generated class
 */

class Icarqoc: public BaseApplLayer, public inet::INetfilter::IHook {
public:
    IcarContext * oIcarContext; // vehicular Communication Context
    MessageManager * oMsgManager;
    std::stringstream dataNetwork;
    long seqMsg;  // counter vehicle messages
    KnownGlobal* oKnownGlobal;

    /**
     * @brief
     *
     * @param
     */
    virtual void sendDown(cMessage* msg);

protected:
    static const simsignalwrap_t mobilityStateChangedSignal;
    inet::INetfilter *networkProtocol = nullptr;

private:

    CommState constanceValues; // constant values
    double loadPeriodApp; //application period load
    //double loadPeriodMonitor; // monitor period load
    long lostMsgAmount;  // amount of vehicle lost messages for lost simulation

    int64_t tempNode; //to handle application messages
    long vehicleAmout; //vehicle amount in the simulation
    double perforMesurementPeriod; //period of communication performance measurement
    cMessage * monitorMsg; //monitor message event
    GeneralCommunicationService vfs;
    IcarContext * oVision; // vehicular agent vision

    LocalAgent * myData;
    TraCIScenarioManager* managerx;

    // buffer file trace netwok;

    // buffer file trace dataMsgTeste;
    std::stringstream dataMsgTeste;

    // ManagerFile * mf;
    fstream fileNetwork;

public:
    enum WaveApplMessageKinds {
        SEND_ICM_EVT = 2,
        SEND_MONITOR_EVT,
        SEND_ICAR_PROTOCOL,
        SEND_FORMATION_VEHIC,
        SEND_IMAGES_STREAMS,
        MEASURE_COMM_PERFORMANCE,
        AGENTPAIR_MONITOR_MESSAGE,
        COLLECT_MOBILITY
    };

    //////// TO GET THE MOBILITY DATA
    TraCICommandInterface* traciComm;
    TraCICommandInterface::Vehicle* traciVehicle;
    TraCICommandInterface::Lane* laneTraci;

    double tempLoadPeriodMonitor;
    cMessage *msgCollectMobility;
    bool integratedScenario;
    double loadPeriodMonitor; // monitor period load
    virtual void initialize(int stage);
    virtual void finish();
    void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj,
            cObject* details) override;
    void scheduleLoad(double appPeriod, int AppTypeId, char * name,
            cMessage *m1);
    std::string getMsgHeaderInfoTrace(ICMessage * wsm, std::string evento,
            bool header, simtime_t pEventTime);
    void loadMonitor(cMessage *msg);
    void scheduleLoadMobility();
    bool verifyRuleRadiusSet(ICRMessage *wsm);
    void loadApp(cMessage *msg);
    void addLostMsgAmount();
    double calcDelaySendLoadApp();
    /**
     * @brief
     *
     * @param
     */
    std::string msgInfoTraceTransmitting(ICMessage * wsm, std::string event,
            std::string ruleRoute, bool header, simtime_t eventTime);

    LocalAgent* getMyData();

    void setMyData(LocalAgent* myData);
    long getLostMsgAmount();

protected:
    //=============================================================================================

    inet::IRoutingTable *routingTable = nullptr;
    cPar *jitterPar = nullptr;
    cMessage *MsgIcar = nullptr;
    int myId;
    void handleMessage(cMessage *msg) override;
    virtual int numInitStages() const override {
        return inet::NUM_INIT_STAGES;
    }
    virtual void processMsgFromLowerLayer(ICMessage *icMessage);
    virtual void processTimer(cMessage *timer);
    virtual void sendUp(cPacket *udpPacket, inet::IPv4ControlInfo *controlInfo);
    inet::L3Address getSelfIPAddress() const;

    //=============================================================================================

    virtual void handleSelfMsg(cMessage* msg);
    virtual void handlePositionUpdate(cObject* obj);

    /* Netfilter hooks */
    Result ensureRouteForDatagram(inet::INetworkDatagram *datagram);
    virtual Result datagramPreRoutingHook(inet::INetworkDatagram *datagram,
            const inet::InterfaceEntry *inputInterfaceEntry,
            const inet::InterfaceEntry *& outputInterfaceEntry,
            inet::L3Address& nextHopAddress) override {
        Enter_Method("datagramPreRoutingHook"); return ensureRouteForDatagram(datagram);}
    virtual Result datagramForwardHook(inet::INetworkDatagram *datagram, const inet::InterfaceEntry *inputInterfaceEntry, const inet::InterfaceEntry *& outputInterfaceEntry, inet::L3Address& nextHopAddress) override;
    virtual Result datagramPostRoutingHook(inet::INetworkDatagram *datagram, const inet::InterfaceEntry *inputInterfaceEntry, const inet::InterfaceEntry *& outputInterfaceEntry, inet::L3Address& nextHopAddress) override {return ACCEPT;}
    virtual Result datagramLocalInHook(inet::INetworkDatagram *datagram, const inet::InterfaceEntry *inputInterfaceEntry) override {return ACCEPT;}
    virtual Result datagramLocalOutHook(inet::INetworkDatagram *datagram, const inet::InterfaceEntry *& outputInterfaceEntry, inet::L3Address& nextHopAddress) override {Enter_Method("datagramLocalOutHook"); return ensureRouteForDatagram(datagram);}
    void delayDatagram(inet::INetworkDatagram *datagram);

    /**
     * @brief
     *
     * @param
     */
    //virtual void sendDown(cMessage* msg);
    /**
     * @brief
     *
     * @param
     */
    void handleLowerMsg(cMessage* wsm) override;

    /**
     * @brief
     *
     * @param
     */
    void onWSM(ICMessage * m);
    /**
     * @brief
     *
     * @param
     */
    //double calcDelaySendLoadApp();
    /**
     * @brief
     *
     * @param
     */
    //void loadApp(cMessage *msg);
    /**
     * @brief
     *
     * @param
     */
    ICMessage * prepareWSM(int lengthBits, AgentPair agentPair, int priority);
    /**
     * @brief
     *
     * @param
     */
    void sendMessageApplication(ForwardingNode * fowardNode,
    int64_t destination, double timetoLiveApp, unsigned int pMsgType,
    int64_t lastNodeNext, int64_t lastNextNodeRequestMsg);

    /**
     * @brief
     *
     * @param
     */
    void sendForwardMessage(ICMessage* pWsm);
    /**
     * @brief
     *
     * @param
     */
    void sendICMessage(ICMessage * wsm);
    void sendICMessage(ICMessage * wsm, std::string evento,
    std::string ruleRoute, bool header);
    /**
     * @brief
     *
     * @param
     */

    void changeLocalMobility();

    void createVehicleTraces();
    /**
     * @brief
     *
     * @param
     */
    //void sendMessageApplicationR(ForwardingNode * fowardNode, int64_t destination, double timetoLiveApp, unsigned int pMsgType, int64_t lastNodeNext, int64_t lastNextNodeRequestMsg);
    /**
     * @brief
     *
     * @param
     */
    //void sendForwardMessageR(ICRMessage* pWsm);
};
#endif
