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

#include "Icarqoc.h"

#include <iostream>
#include "veins_inet/modules/application/icarnew/Icarqoc.h"

#include "inet/networklayer/ipv4/ICMPMessage.h"
#include "inet/networklayer/ipv4/IPv4Route.h"
#include "inet/networklayer/common/IPSocket.h"
#include "inet/transportlayer/contract/udp/UDPControlInfo.h"
#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/contract/INetfilter.h"

using namespace std;
Define_Module(Icarqoc);

const simsignalwrap_t Icarqoc::mobilityStateChangedSignal = simsignalwrap_t(
MIXIM_SIGNAL_MOBILITY_CHANGE_NAME);

void Icarqoc::initialize(int stage) {
    BaseModule::initialize(stage);

    if (stage == 0) {
        routingTable = inet::getModuleFromPar<inet::IRoutingTable>(
                par("routingTableModule"), this);
        jitterPar = &par("jitter");

        //Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;
        this->seqMsg = 0;
        this->lostMsgAmount = 0;
        this->tempNode = -1;

        if (FindModule<KnownGlobal*>::findGlobalModule()) {
            oKnownGlobal = FindModule<KnownGlobal*>::findGlobalModule();

            //agent name = vehicle creation order
            std::stringstream aName;
            aName << oKnownGlobal->knownVehicles.size();
            int64_t idVeicularAgent = oKnownGlobal->knownVehicles.size();

            double maxSpeed;
            if (oKnownGlobal->knownVehicles.size() % 2 == 0)
                maxSpeed = par("maxSpeedRoute1").doubleValue();
            else
                maxSpeed = par("maxSpeedRoute0").doubleValue();

            integratedScenario = par("integratedScenario").boolValue();

            VeinsInetMobility* mobility =
                    FindModule<VeinsInetMobility*>::findSubModule(
                            getParentModule());

            LocalMobility * locality = new LocalMobility(mobility, maxSpeed);

//            if (FindModule<TraCIMobility*>::findSubModule(getParentModule())) {
//                TraCIMobility * mobility = TraCIMobilityAccess().get(
//                        getParentModule());
//                traciComm = mobility->getCommandInterface();
//                traciVehicle = mobility->getVehicleCommandInterface();
//            }

            this->myData = new LocalAgent(idVeicularAgent, locality,
                    aName.str());

            this->oIcarContext = new IcarContext(this->myData,
                    this->oKnownGlobal, this);

            oMsgManager = new MessageManager(this);

            // temporario
            oVision = oIcarContext;
            oKnownGlobal->knownVehicles.push_back(this->oVision->getMyData());

            //get configuration from physic layer module
            BasePhyLayer * x = FindModule<BasePhyLayer*>::findGlobalModule();
//            oKnownGlobal->oRadiusEstimatorAgentPair->sensitivityPower_dbm = x->getAncestorPar("sensitivity").doubleValue();
            oKnownGlobal->oRadiusEstimatorAgentPair->sensitivityPower_dbm = -89;
        }

        //declared radius in meters
        this->oVision->getMyData()->setSetRadius(oKnownGlobal->setRadius);

        //schedule first to measure collect mobility
        if (oKnownGlobal->mobilityPeriod != 0) {
            cMessage *msgCollectMobility = new cMessage("collectMobility",
                    COLLECT_MOBILITY);
            scheduleLoad(oKnownGlobal->startSimulation, COLLECT_MOBILITY,
                    "firstLoad", msgCollectMobility);
        } else {
            findHost()->subscribe(mobilityStateChangedSignal, this);
        }

        // period of communication performance measurement
        perforMesurementPeriod = oKnownGlobal->perforMesurementPeriod;

        //schedule first to measure communication performance
        cMessage *msgPer = new cMessage("commperform",
                MEASURE_COMM_PERFORMANCE);
        scheduleLoad(oKnownGlobal->startSimulation, MEASURE_COMM_PERFORMANCE,
                "firstLoad", msgPer);

        //schedule first load monitor message
        this->monitorMsg = new cMessage("firstLoad", SEND_MONITOR_EVT);

        this->loadPeriodMonitor = oKnownGlobal->monitorPeriod;

        scheduleLoad(oKnownGlobal->startSimulation, SEND_MONITOR_EVT,
                "firstLoad", this->monitorMsg);

        // vehicle amount
        vehicleAmout = oKnownGlobal->vehicleAmout;

        // general type of application
        this->vfs = oKnownGlobal->oGeneralCommunicationService;

        // period to send application messages
        loadPeriodApp =
                oKnownGlobal->oGeneralCommunicationService.appPeriodLoadMsg;
        //schedule first load application messages
        if (loadPeriodApp > 0) {
            cMessage *msg = new cMessage("firstLoad", SEND_ICM_EVT);
            scheduleLoad(oKnownGlobal->startSimulation, SEND_ICM_EVT,
                    "firstLoad", msg);
        }

        //initialize trace

        dataMsgTeste << ";amoutVehicle;loadPeriodApp;"
                << msgInfoTraceTransmitting(NULL, "", "", true, 0) << std::endl;

        if (oKnownGlobal->simulationDataheaderLine)
            dataNetwork << ";vehicleAmout;loadPeriodApp;"
                    << msgInfoTraceTransmitting(NULL, "", "", true, 0)
                    << std::endl;
        else
            dataNetwork << "";

        networkProtocol = inet::getModuleFromPar<inet::INetfilter>(
                par("networkProtocolModule"), this);
// 11 = INITSTAGE_ROUTING_PROTOCOLS
    } else if (stage == 11) {
        //Initializing members that require initialized other modules goes here
        inet::IPSocket socket(gate("ipOut"));
        socket.registerProtocol(inet::IP_PROT_MANET);
        networkProtocol->registerHook(0, this);
    }

}

void Icarqoc::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        processTimer(msg);
    }

    else //Mensagem Recebida da camada de Rede
    if (msg->arrivedOn("ipIn")) {
        if (dynamic_cast<ICMessage *>(msg) != nullptr) {
            processMsgFromLowerLayer((ICMessage *) msg);
            ICMessage * icm;
            if (icm = dynamic_cast<ICMessage*>(msg))
                onWSM(icm);
        }
    }

    else //Mensagem Recebida da camada de Transporte
    if (dynamic_cast<inet::UDPPacket *>(msg) != nullptr) {
        MsgIcar = msg;
        loadApp(msg);
    }
}

inet::INetfilter::IHook::Result Icarqoc::ensureRouteForDatagram(
        inet::INetworkDatagram *datagram) {
    const inet::L3Address& destAddr = datagram->getDestinationAddress();
    const inet::L3Address& sourceAddr = datagram->getSourceAddress();
    if (destAddr.isBroadcast() || routingTable->isLocalAddress(destAddr)
            || destAddr.isMulticast())
        return ACCEPT;
}

inet::INetfilter::IHook::Result Icarqoc::datagramForwardHook(
        inet::INetworkDatagram *datagram,
        const inet::InterfaceEntry *inputInterfaceEntry,
        const inet::InterfaceEntry *& outputInterfaceEntry,
        inet::L3Address& nextHopAddress) {

    Enter_Method("datagramForwardHook");
    const inet::L3Address& destAddr = datagram->getDestinationAddress();
    const inet::L3Address& sourceAddr = datagram->getSourceAddress();
    inet::IRoute *ipSource = routingTable->findBestMatchingRoute(sourceAddr);

    if (destAddr.isBroadcast() || routingTable->isLocalAddress(destAddr) || destAddr.isMulticast()) {
        if (routingTable->isLocalAddress(destAddr) && ipSource && ipSource->getSource() == this)
        return ACCEPT;
    }
}

void Icarqoc::processMsgFromLowerLayer(ICMessage *icMessage) {

    inet::IPv4ControlInfo *controlInfo =
            check_and_cast<inet::IPv4ControlInfo *>(
                    icMessage->removeControlInfo());
    cPacket *icPacket = icMessage->decapsulate();

    cout << "=====================RECEBIDA PELO HOST: " << getSelfIPAddress()
            << " ===============" << endl;
    cout << "HOST EMISSOR    :" << controlInfo->getSrcAddr() << endl;
    cout << "NOME MENSAGEM   :" << icMessage->getName() << endl;
    cout << "=================================================================="
            << endl << endl;

    sendUp(icPacket, controlInfo);
}

void Icarqoc::processTimer(cMessage *timer) {
}

void Icarqoc::sendUp(cPacket *icPacket, inet::IPv4ControlInfo *controlInfo) {
    controlInfo->setProtocol(inet::IP_PROT_UDP);
    controlInfo->setSrcAddr(controlInfo->getSrcAddr());
    controlInfo->setDestAddr(controlInfo->getDestAddr());
    controlInfo->setInterfaceId(controlInfo->getInterfaceId());
    controlInfo->setTimeToLive(controlInfo->getTimeToLive());
    controlInfo->setTypeOfService(controlInfo->getTypeOfService());
//    controlInfo->setHopLimit(controlInfo->getHopLimit());
    icPacket->setControlInfo(controlInfo);

    if (jitterPar->doubleValue() == 0)
        send(icPacket, "icarqOut");
    else
        sendDelayed(icPacket, jitterPar->doubleValue(), "icarqOut");
}

inet::L3Address Icarqoc::getSelfIPAddress() const {
    return routingTable->getRouterIdAsGeneric();
}

void Icarqoc::finish() {

    createVehicleTraces();
    oVision->dataCommPerformance.clear();
    oVision->dataCommPerformance << "simulation;timeStamp; localId; "
            << this->myData->getLocalCommInfo()->getLocalAgentCommPerformance()->infoTrace(
                    true, "") << std::endl;
    oVision->dataCommPerformance << oKnownGlobal->observationNameFile << ";"
            << simTime() << ";" << this->myData->getId() << ";"
            << this->myData->getLocalCommInfo()->getLocalAgentCommPerformance()->infoTrace(
                    false, "") << std::endl;
    oVision->dataMinslr
            << oVision->getAgentPairList()->infoTraceAgentPairList();
    this->oKnownGlobal->knownVehicles.at(myData->getId()) = NULL;
    this->oKnownGlobal->fileMessages << dataNetwork.str();
    this->oKnownGlobal->fileRemoteAgents << oVision->dataRemoteAgent.str();
    this->oKnownGlobal->fileLocalAgents << oVision->dataLocalAgent.str();
    this->oKnownGlobal->fileChannels << oVision->dataAgentPair.str();
    this->oKnownGlobal->fileChannelsMinslr << oVision->dataMinslr.str();
    this->oKnownGlobal->fileCommPerformance
            << oVision->dataCommPerformance.str();
    this->oKnownGlobal->fileReceivedMessages << dataMsgTeste.str();
}

void Icarqoc::onWSM(ICMessage * wsm) {
    //Your application has received a data message from another car or RSU
    //code for handling the message goes here, see TraciDemo11p.cc for examples
    //double vtime;

    if (wsm->getSourceId() != this->myData->getId()
            && wsm->getAntecessorId() != this->myData->getId()) {
        cObject * teste = (dynamic_cast<cPacket *>(wsm)->getControlInfo());
        //std::cout << teste->getClassName() << std::endl;
        std::cout << "TESTANDO " << std::endl;

        simtime_t msgReceivingTime = simTime();
//        this->oVision->updateVision(wsm, msgReceivingTime);

        dataMsgTeste << ";" << this->oKnownGlobal->vehicleAmout << ";"
                << this->oKnownGlobal->loadPeriodApp << ";"
                << getMsgHeaderInfoTrace(wsm, "receiving", false,
                        msgReceivingTime) << std::endl;

        if (wsm->getNextId() == this->myData->getId()
                && wsm->getDestinationId() != this->myData->getId()
                && wsm->getHopNumber() < 4) {
            this->sendForwardMessage(wsm);
//            this->scheduleLoadApp(this->loadPeriodMonitor, SEND_MONITOR_EVT, "monitorMessage", this->monitorMsg);

        }
    }
}

void Icarqoc::handleSelfMsg(cMessage* msg) {
    //BaseWaveApplLayer::handleSelfMsg(msg);
    //this method is for self messages (mostly timers)
    //it is important to call the BaseWaveApplLayer function for BSM and WSM transmission
    //SEND_ICM_EVT, // general event of IC Message
    //SEND/-*--_ICAR_PROTOCOL, //Data dissemination
    //SEND_FORMATION_VEHIC, // vehicular formation
    //SEND_IMAGES_STREAMS // images streams
    //MEASURE_COMM_PERFORMANCE // event to measure the environment communication performance
    //AGENTPAIR_MONITOR_MESSAGE = For each knowed CHANNEL when need to update communication state (communicatin, non-communicating, fault etc.)

    switch (msg->getKind()) {
    case MEASURE_COMM_PERFORMANCE: {
        // calc performance measurement
        //if (msg->getName()=="firstLoad") this->oVision->calcCommPerformVision(true);
        //else this->oVision->calcCommPerformVision(false);

        //schedule new performance measurement
        //this->scheduleLoad(this->perforMesurementPeriod, MEASURE_COMM_PERFORMANCE, "commperform", msg);
    }
        break;

    case AGENTPAIR_MONITOR_MESSAGE: {
        AgentPair * c = (AgentPair *) (msg->getContextPointer());
        int64_t s = c->getAgentS()->getId();
        int64_t d = c->getAgentD()->getId();
        //oVision->updateCommunicationState(s,d);
    }
        break;

    case COLLECT_MOBILITY: {
        //schedule new  mobility collect
        if (oKnownGlobal->mobilityPeriod == -1) {
            double tempx = (double(rand() % 100 + 1) / 10) / 2.5;
            this->scheduleLoad(tempx, COLLECT_MOBILITY, "collectMobility", msg);
            std::cout << endl << "range Mobility= " << tempx << endl;
        }

        if (oKnownGlobal->mobilityPeriod > 0)
            this->scheduleLoad(this->oKnownGlobal->mobilityPeriod,
                    COLLECT_MOBILITY, "collectMobility", msg);

        this->changeLocalMobility();
    }
        break;

    case SEND_MONITOR_EVT: {
        //send monitor message
        loadMonitor(msg);
    }
        break;

    case SEND_ICM_EVT: {
        loadApp(msg);
        this->scheduleLoad(this->loadPeriodApp, SEND_ICM_EVT, "appMessage",
                msg);
    }
        break;

    default: {
        if (msg)
            EV << "APP: Error: Got Self Message of unknown kind! Name: "
                      << msg->getName() << endl;
    }
        break;

    }
}

void Icarqoc::handleLowerMsg(cMessage* msg) {

    WaveShortMessage* icm = dynamic_cast<WaveShortMessage *>(msg);
    ASSERT(icm);

    ICRMessage * wsm = dynamic_cast<ICRMessage*>(icm);
    ASSERT(wsm);

    if (wsm != NULL) {
        this->oIcarContext->updateContextfromMsg(wsm, simTime());
        //Nao e o local adequado
        //if (verifyRuleRadiusSet(wsm)) this->oIcarContext->updateContextfromMsg(wsm, simTime());

    } else
        std::cout << " msg e null" << std::endl;
}

void Icarqoc::changeLocalMobility() {
    this->oIcarContext->changeLocalMobility();
}

/*****************************************************************************************************************************************************
 * APPLICATION LOAD IN NETWORK
 ****************************************************************************************************************************************************/

void Icarqoc::loadApp(cMessage *msg) {
    int64_t destinyNode;

    ForwardingNode * foward = new ForwardingNode();

    destinyNode = vfs.getDestination(this->oKnownGlobal->knownVehicles,
            this->myData->getId());

    //foward = oVision->getRouting()->getFowardNode(this->oVision->getAgentGroup(), this->oVision->getAgentPairList(), destinyNode,
    //      oVision->getMyData()->getId(), -1, vfs.timeToLive, Coord(0,0,0), Coord(0,0,0));

//    oMsgManager->sendIcarMessageService(foward, destinyNode, vfs.timeToLive,
//            msg->getKind());
    sendMessageApplication(foward, destinyNode, vfs.timeToLive, msg->getKind(),
            -1, -1);

    this->scheduleLoad(this->loadPeriodMonitor, SEND_MONITOR_EVT,
            "monitorMessage", this->monitorMsg);
}

void Icarqoc::loadMonitor(cMessage *msg) {
    int64_t destinyNode = -1;
    if (msg->getKind() == SEND_MONITOR_EVT) {
        ForwardingNode * foward = new ForwardingNode();

        oMsgManager->sendIcarMessageService(foward, destinyNode, vfs.timeToLive,
                msg->getKind());

        if (this->oKnownGlobal->monitorPeriod == 0) {
            this->loadPeriodMonitor = (1.5 * (double(rand() % 100) + 1) / 100)
                    + 0.5;
        }

        this->scheduleLoad(this->loadPeriodMonitor, SEND_MONITOR_EVT,
                "monitorMessage", this->monitorMsg);
    }
}

void Icarqoc::scheduleLoad(double appPeriod, int AppTypeId, char * name,
        cMessage *m1) {
    if (m1->isScheduled())
        cancelEvent(m1);

    m1->setName(name);
    m1->setKind(AppTypeId);

    simtime_t s = ((simtime_t) (simTime().dbl() + appPeriod));

    scheduleAt(s, m1);
}

void Icarqoc::scheduleLoadMobility() {
    if (this->msgCollectMobility->isScheduled())
        cancelEvent(msgCollectMobility);

    msgCollectMobility->setName("collectMobility");
    msgCollectMobility->setKind(COLLECT_MOBILITY);

    simtime_t s = ((simtime_t) (simTime().dbl()
            + this->oKnownGlobal->mobilityPeriod));

    scheduleAt(s, msgCollectMobility);
}

void Icarqoc::handlePositionUpdate(cObject* obj) {
    //BaseWaveApplLayer::handlePositionUpdate(obj);
    //the vehicle has moved. Code that reacts to new positions goes here.
    //member variables such as currentPosition and currentSpeed are updated in the parent class
}

/*
 * Calcing delay to send application load message
 */
double Icarqoc::calcDelaySendLoadApp() {
    return ((double(rand() % 10) + 1) / 100);

}
/*
 * when the radius of a node has a value on configuration (for example radiusSet=250m)
 */
bool Icarqoc::verifyRuleRadiusSet(ICRMessage *wsm) {
    // when the radius of a node has a value on configuration (for example radiusSet=250m)
    double measureDistance = 0;
    int transmitterId =
            wsm->getRouteNodesArraySize() > 1 ?
                    wsm->getRouteNodesArraySize() - 2 : 0;

    measureDistance =
            this->oIcarContext->oKnownGlobal->calcTraciDistanceMobility(
                    wsm->getRouteNodes(transmitterId).nodeId,
                    this->oIcarContext->myData->getId());

    if (measureDistance <= this->oIcarContext->myData->getSetRadius()
            && this->oIcarContext->myData->getSetRadius() > 0)
        return true;
    else {
        if (this->oIcarContext->myData->getSetRadius() <= 0)
            return true;
        else
            return false;
    }

}

void Icarqoc::sendICMessage(ICMessage * wsm) {
    if (jitterPar->doubleValue() == 0)
        send(wsm, "ipOut");
    else
        sendDelayed(wsm, jitterPar->doubleValue(), "ipOut");
    //sendDown(wsm);
}

void Icarqoc::sendICMessage(ICMessage * wsm, std::string evento,
        std::string ruleRoute, bool header) {
    dataNetwork << ";" << this->oKnownGlobal->vehicleAmout << ";"
            << this->oKnownGlobal->loadPeriodApp << ";"
            << msgInfoTraceTransmitting(wsm, evento, ruleRoute, header,
                    wsm->getTimestamp());
    sendICMessage(wsm);
}

void Icarqoc::sendDown(cMessage* msg) {
    BaseApplLayer::sendDown(msg);
}

/*
 *  @brief send application message
 *  @param forwarding node
 *  @param  destiny node
 *  @param  message time (time to leave)
 *  @param  message type (see enum in baseapp)
 *  @param  last node forwarding node
 *  @param  number of monitoring message)
 */
void Icarqoc::receiveSignal(cComponent* source, simsignal_t signalID,
        cObject* obj, cObject* details) {
    Enter_Method_Silent();
    if (signalID == mobilityStateChangedSignal) {
        if (oKnownGlobal->mobilityPeriod == 0) this->changeLocalMobility();
    }

}

void Icarqoc::sendMessageApplication(ForwardingNode * fowardNode,
        int64_t destination, double timetoLive, unsigned int pMsgType,
        int64_t lastNodeNext, int64_t numMonitoringMsg) {
    inet::IPv4ControlInfo *controlInfo =
            check_and_cast<inet::IPv4ControlInfo *>(
                    MsgIcar->removeControlInfo());
    inet::UDPPacket *udpPacket = check_and_cast<inet::UDPPacket *>(MsgIcar);
    //preparing transmition wsm message
    ICMessage * wsm = new ICMessage();

    wsm->encapsulate(udpPacket);
    controlInfo->setProtocol(inet::IP_PROT_UDP);
    controlInfo->setSrcAddr(controlInfo->getSrcAddr());
    controlInfo->setDestAddr(controlInfo->getDestAddr());
    controlInfo->setInterfaceId(controlInfo->getInterfaceId());
    controlInfo->setMulticastLoop(controlInfo->getMulticastLoop());
    controlInfo->setTimeToLive(controlInfo->getTimeToLive());
    controlInfo->setTypeOfService(controlInfo->getTypeOfService());
    wsm->setControlInfo(controlInfo);

//erro  this->populateWSM(wsm);
    wsm->setKind(pMsgType);

    //prepareing icmessage
    wsm->setMsgType(pMsgType);
    wsm->setNumMsg(seqMsg++);
    wsm->setName("data");

    // data transmitter
    wsm->setAntecessoPosTimeStamp(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getStartTime());
    wsm->setAntecessorAngle(
            this->oVision->getMyData()->getMobilityInfo()->getAngle());
    wsm->setAntecessorAceleration(
            this->oVision->getMyData()->getMobilityInfo()->getAcceleration());
    wsm->setAntecessorMaxSpeed(
            this->oVision->getMyData()->getMobilityInfo()->getMaxSpeed());
    wsm->setAntecessorMsgTimeStamp(simTime());
    //wsm->setAntecessorRadius(this->oVision->getMyData()->getSetRadius());
    wsm->setAntecessorSpeed(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getSpeed());
    wsm->setAntecessorX(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getCurrentPosition().x);
    wsm->setAntecessorY(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getCurrentPosition().y);
    wsm->setAntecessorZ(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getCurrentPosition().z);

    // data source
    wsm->setSourceMaxSpeed(
            this->oVision->getMyData()->getMobilityInfo()->getMaxSpeed());
    wsm->setSourceAceleration(
            this->oVision->getMyData()->getMobilityInfo()->getAcceleration());
    wsm->setSourceAngle(
            this->oVision->getMyData()->getMobilityInfo()->getAngle());
    wsm->setSourceMsgTimeStamp(simTime());
    wsm->setSourcePosTimeStamp(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getStartTime());
    //wsm->setSourceRadius(this->oVision->getMyData()->getSetRadius());
    wsm->setSourceSpeed(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getSpeed());
    wsm->setSourceX(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getCurrentPosition().x);
    wsm->setSourceY(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getCurrentPosition().y);
    wsm->setSourceZ(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getCurrentPosition().z);
    wsm->setSourceId(this->oVision->getMyData()->getId());
    wsm->setAntecessorId(this->oVision->getMyData()->getId());

    // data destination
    wsm->setDestinationId(destination);
    RemoteAgent * ra = this->oVision->getAgentGroup()->getRemoteAgent(
            destination);
    if (ra != NULL)
        if (ra->getId() != -1) {
            wsm->setDestinationMaxSpeed(ra->getMobilityInfo()->getMaxSpeed());
            wsm->setDestinationAngle(ra->getMobilityInfo()->getAngle());
            wsm->setDestinationAceleration(
                    ra->getMobilityInfo()->getAcceleration());
            wsm->setDestinationPosTimeStamp(
                    ra->getMobilityInfo()->getActualMove()->getStartTime());
            //wsm->setDestinationRadius(ra->getSetRadius());
            wsm->setDestinationSpeed(
                    ra->getMobilityInfo()->getActualMove()->getSpeed());
            wsm->setDestinationX(
                    ra->getMobilityInfo()->getActualMove()->getCurrentPosition().x);
            wsm->setDestinationY(
                    ra->getMobilityInfo()->getActualMove()->getCurrentPosition().y);
            wsm->setDestinationZ(
                    ra->getMobilityInfo()->getActualMove()->getCurrentPosition().z);
        }

    //wsm->setMsgLifeTime(timetoLive);
    wsm->setMsgLifeTime(simTime() + timetoLive);

    wsm->setTimestamp(simTime());
    wsm->setHopNumber(1);

    // next data
    std::string ruleRouting = "";
    if (fowardNode != NULL) {
        if (fowardNode->getFowardId() != -1) {
            wsm->setNextId(fowardNode->getFowardId());
            wsm->setValidityDataTimeStamp(fowardNode->getTimeoutValidity());
            wsm->setAnteNextValidityTimeStamp(fowardNode->getTimeoutValidity());
            ruleRouting = fowardNode->getRule();

            //std::cout << std::endl << ruleRouting << "  node forward - " << fowardNode->getFowardId() << "   local - " << this->myData->getId()  << "   source- " << wsm->getSourceId() << "   destino- " << destination << endl;

            RemoteAgent * rm = this->oVision->getAgentGroup()->getRemoteAgent(
                    fowardNode->getFowardId());

            if (rm != NULL) {
                //wsm->setNextRadius(this->oVision->getAgentGroup()->getRemoteAgent(fowardNode->getFowardId())->getSetRadius());
                //std::cout << std::endl << "  aqui - ";
                wsm->setNextSpeed(
                        this->oVision->getAgentGroup()->getRemoteAgent(
                                fowardNode->getFowardId())->getMobilityInfo()->getActualMove()->getSpeed());
                wsm->setNextMaxSpeed(
                        this->oVision->getAgentGroup()->getRemoteAgent(
                                fowardNode->getFowardId())->getMobilityInfo()->getMaxSpeed());
                wsm->setNextAceleration(
                        this->oVision->getAgentGroup()->getRemoteAgent(
                                fowardNode->getFowardId())->getMobilityInfo()->getAcceleration());
                wsm->setNextAngle(
                        this->oVision->getAgentGroup()->getRemoteAgent(
                                fowardNode->getFowardId())->getMobilityInfo()->getAngle());
                wsm->setNextPosTimeStamp(
                        this->oVision->getAgentGroup()->getRemoteAgent(
                                fowardNode->getFowardId())->getMobilityInfo()->getActualMove()->getStartTime());
                wsm->setNextX(
                        this->oVision->getAgentGroup()->getRemoteAgent(
                                fowardNode->getFowardId())->getMobilityInfo()->getActualMove()->getCurrentPosition().x);
                wsm->setNextY(
                        this->oVision->getAgentGroup()->getRemoteAgent(
                                fowardNode->getFowardId())->getMobilityInfo()->getActualMove()->getCurrentPosition().y);
                wsm->setNextZ(
                        this->oVision->getAgentGroup()->getRemoteAgent(
                                fowardNode->getFowardId())->getMobilityInfo()->getActualMove()->getCurrentPosition().z);
            }
        } else {
            wsm->setNextId(-1);
            ruleRouting = "-1";
        }

    } else {
        wsm->setNextId(-1);
        wsm->setValidityDataTimeStamp(-1);
        wsm->setAnteNextValidityTimeStamp(-1);
        ruleRouting = "-1";
    }

    //wsm->addByteLength(((string)wsm->getWsmData()).size());
    //wsm->setWsmLength(((string)wsm->getWsmData()).size());

    sendICMessage(wsm, "transmitting", ruleRouting, false);
}

void Icarqoc::sendForwardMessage(ICMessage* pWsm) {

    ForwardingNode * fowardNode; // =  oVision->getRouting()->getFowardNode(oVision->getAgentGroup(), oVision->getAgentPairList(),
    //pWsm->getDestinationId(), oVision->getMyData()->getId(), pWsm->getAntecessorId(), pWsm->getMsgLifeTime().dbl(), Coord(0,0,0), Coord(0,0,0));

    ICMessage * wsm = new ICMessage();

//    this->populateWSM(wsm);
    wsm->setPsid(0);
    wsm->setChannelNumber(static_cast<int>(Channels::CCH));
    wsm->addBitLength(100);
    wsm->setUserPriority(7);
    wsm->setRecipientAddress(-1);

    wsm->setKind(pWsm->getKind());
    wsm->setName(pWsm->getName());
    wsm->setNumMsg(pWsm->getNumMsg());
    wsm->setAntecessoPosTimeStamp(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getStartTime());
    wsm->setAntecessorMaxSpeed(
            this->oVision->getMyData()->getMobilityInfo()->getMaxSpeed());
    wsm->setAntecessorMsgTimeStamp(simTime());

    //wsm->setAntecessorRadius(this->oVision->getMyData()->getSetRadius());
    wsm->setAntecessorSpeed(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getSpeed());
    wsm->setAntecessorAceleration(
            this->oVision->getMyData()->getMobilityInfo()->getAcceleration());
    wsm->setAntecessorAngle(
            this->oVision->getMyData()->getMobilityInfo()->getAngle());
    wsm->setAntecessorX(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getStartPos().x);
    wsm->setAntecessorY(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getStartPos().y);
    wsm->setAntecessorZ(
            this->oVision->getMyData()->getMobilityInfo()->getActualMove()->getStartPos().z);
    wsm->setAntecessorId(this->oVision->getMyData()->getId());
    wsm->setHopNumber(pWsm->getHopNumber() + 1);

    // forwarding data
    std::string ruleRouting = "";
    if (fowardNode != NULL) {
        if (fowardNode->getFowardId() != -1) {

            wsm->setNextId(fowardNode->getFowardId());
            if (fowardNode->getTimeoutValidity()
                    < pWsm->getValidityDataTimeStamp()
                    || pWsm->getValidityDataTimeStamp() == -1)
                wsm->setValidityDataTimeStamp(fowardNode->getTimeoutValidity());

            wsm->setAnteNextValidityTimeStamp(fowardNode->getTimeoutValidity());
            wsm->setNextId(fowardNode->getFowardId());

            ruleRouting = fowardNode->getRule();
            //wsm->setNextRadius(this->oVision->getAgentGroup()->getRemoteAgent(fowardNode->getFowardId())->getSetRadius());
            wsm->setNextMaxSpeed(
                    this->oVision->getAgentGroup()->getRemoteAgent(
                            fowardNode->getFowardId())->getMobilityInfo()->getMaxSpeed());
            wsm->setNextSpeed(
                    this->oVision->getAgentGroup()->getRemoteAgent(
                            fowardNode->getFowardId())->getMobilityInfo()->getActualMove()->getSpeed());
            wsm->setNextAngle(
                    this->oVision->getAgentGroup()->getRemoteAgent(
                            fowardNode->getFowardId())->getMobilityInfo()->getAngle());
            wsm->setNextAceleration(
                    this->oVision->getAgentGroup()->getRemoteAgent(
                            fowardNode->getFowardId())->getMobilityInfo()->getAcceleration());
            wsm->setNextPosTimeStamp(
                    this->oVision->getAgentGroup()->getRemoteAgent(
                            fowardNode->getFowardId())->getMobilityInfo()->getActualMove()->getStartTime());
            wsm->setNextX(
                    this->oVision->getAgentGroup()->getRemoteAgent(
                            fowardNode->getFowardId())->getMobilityInfo()->getActualMove()->getStartPos().x);
            wsm->setNextY(
                    this->oVision->getAgentGroup()->getRemoteAgent(
                            fowardNode->getFowardId())->getMobilityInfo()->getActualMove()->getStartPos().y);
            wsm->setNextZ(
                    this->oVision->getAgentGroup()->getRemoteAgent(
                            fowardNode->getFowardId())->getMobilityInfo()->getActualMove()->getStartPos().z);
        } else {
            wsm->setNextId(-1); //melhorar esta condicional -- est repetitiva
            wsm->setAnteNextValidityTimeStamp(-1);
        }
    } else {
        wsm->setNextId(-1); // melhorar esta condicional -- est repetitiva
        wsm->setAnteNextValidityTimeStamp(-1);
        ruleRouting = "-1";
    }

    // destination data
    RemoteAgent * rd = this->oVision->getAgentGroup()->getRemoteAgent(
            pWsm->getDestinationId());
    wsm->setDestinationId(pWsm->getDestinationId());

    if (rd != NULL)
        if (rd->getId() != -1
                && wsm->getDestinationPosTimeStamp()
                        < rd->getMobilityInfo()->getActualMove()->getStartTime()) {
            wsm->setDestinationMaxSpeed(rd->getMobilityInfo()->getMaxSpeed());
            wsm->setDestinationPosTimeStamp(
                    rd->getMobilityInfo()->getActualMove()->getStartTime());
            wsm->setDestinationSpeed(
                    rd->getMobilityInfo()->getActualMove()->getSpeed());
            wsm->setDestinationAceleration(
                    rd->getMobilityInfo()->getAcceleration());
            wsm->setDestinationAngle(rd->getMobilityInfo()->getAngle());
            wsm->setDestinationX(
                    rd->getMobilityInfo()->getActualMove()->getStartPos().x);
            wsm->setDestinationY(
                    rd->getMobilityInfo()->getActualMove()->getStartPos().y);
            wsm->setDestinationZ(
                    rd->getMobilityInfo()->getActualMove()->getStartPos().z);
        } else {
            wsm->setDestinationMaxSpeed(pWsm->getDestinationMaxSpeed());
            wsm->setDestinationPosTimeStamp(pWsm->getDestinationPosTimeStamp());
            wsm->setDestinationSpeed(pWsm->getDestinationSpeed());
            wsm->setDestinationAceleration(pWsm->getDestinationAceleration());
            wsm->setDestinationAngle(pWsm->getDestinationAngle());
            wsm->setDestinationX(pWsm->getDestinationX());
            wsm->setDestinationY(pWsm->getDestinationY());
            wsm->setDestinationZ(pWsm->getDestinationZ());
        }

    // source data
    RemoteAgent * rs = this->oVision->getAgentGroup()->getRemoteAgent(
            pWsm->getSourceId());
    wsm->setSourceId(pWsm->getSourceId());
    if (rs != NULL)
        if (rs->getId() != -1
                && pWsm->getSourcePosTimeStamp()
                        < rs->getMobilityInfo()->getActualMove()->getStartTime()) {
            wsm->setSourceMaxSpeed(rs->getMobilityInfo()->getMaxSpeed());
            wsm->setSourcePosTimeStamp(
                    rs->getMobilityInfo()->getActualMove()->getStartTime());
            wsm->setSourceSpeed(
                    rs->getMobilityInfo()->getActualMove()->getSpeed());
            wsm->setSourceAngle(rs->getMobilityInfo()->getAngle());
            wsm->setSourceAceleration(rs->getMobilityInfo()->getAcceleration());
            wsm->setSourceX(
                    rs->getMobilityInfo()->getActualMove()->getStartPos().x);
            wsm->setSourceY(
                    rs->getMobilityInfo()->getActualMove()->getStartPos().y);
            wsm->setSourceZ(
                    rs->getMobilityInfo()->getActualMove()->getStartPos().z);
        } else {
            wsm->setSourceMaxSpeed(pWsm->getSourceMaxSpeed());
            wsm->setSourcePosTimeStamp(pWsm->getSourcePosTimeStamp());
            wsm->setSourceSpeed(pWsm->getSourceSpeed());
            wsm->setSourceAngle(pWsm->getSourceAngle());
            wsm->setSourceAceleration(pWsm->getSourceAceleration());
            wsm->setSourceX(pWsm->getSourceX());
            wsm->setSourceY(pWsm->getSourceY());
            wsm->setSourceZ(pWsm->getSourceZ());
        }

    this->myData->getLocalCommInfo()->getLocalAgentCommPerformance()->addCorrectMsgForwardedMsg();

    sendICMessage(wsm, "forwarding", ruleRouting, false);
}

void Icarqoc::createVehicleTraces() {
    // trace message file
    stringstream nameFileMessage;

    string aux = this->oKnownGlobal->getFilePreFix(this->myData->getId()).str();
    nameFileMessage.str("");

    nameFileMessage << aux << "PairAgents.csv";
    fstream fileChannels;
    fileChannels.open(nameFileMessage.str(),
            fstream::in | fstream::out | fstream::app);
    fileChannels << oVision->dataAgentPair.str();

}

LocalAgent* Icarqoc::getMyData() {
    return myData;
}

void Icarqoc::setMyData(LocalAgent* myData) {
    this->myData = myData;
}

void Icarqoc::addLostMsgAmount() {
    ++lostMsgAmount;
}

long Icarqoc::getLostMsgAmount() {
    return this->lostMsgAmount;
}

std::string Icarqoc::msgInfoTraceTransmitting(ICMessage * wsm,
        std::string evento, std::string ruleRoute, bool header,
        simtime_t pEventTime) {
    std::ostringstream ost;
    if (header) {
        ost << "event" << ";printTime" << ";timestampMSG" << ";delayMSG"
                << ";localid" << ";localname"
                //<< oVision->getMyData()->infoTrace(header)
                << ";messageNumber" << ";Name" << ";MsgType" << ";MsgLifeTime"

                << ";RuleRoute" << ";ValidityRouteTime" << ";hopNumber"
                // << ";SourceName"
                << ";SourceId"
                // << ";AntecessorName"
                << ";AntecessorId"
                //<< ";NextName"
                << ";NextId"
                // << ";DestinationName"
                << ";DestinationId"

                << ";AntecessorMsgTimeStamp" << ";AntecessorPostime"
                << ";AntecessorMaxSpeed"
                //<< ";AntecessorRadius"
                << ";AntecessorSpeed" << ";AntecessorX" << ";AntecessorY"
                << ";AntecessorZ" << ";AnteNextValidityTimeStamp"

                << ";SourceMsgTimeStamp" << ";SourcePosTimeStamp"
                << ";sensorPosAge" << ";SourceMaxSpeed" << ";SourceRadius"
                //<< ";SourceSpeed"
                << ";SourceX" << ";SourceY" << ";SourceZ"

                << ";NextPosTimeStamp" << ";NextMaxSpeed"
                //<< ";NextRadius"
                << ";NextSpeed" << ";NextX" << ";NextY" << ";NextZ"

                << ";DestinationPosTimeStamp" << ";DestinationMaxSpeed"
                //<< ";DestinationRadius"
                << ";DestinationSpeed" << ";DestinationX" << ";DestinationY"
                << ";DestinationZ"

                << ";Msgdata" << std::endl;
    } else {
        ost << evento << ";" << simTime() << ";" << wsm->getTimestamp() << ";"
                << pEventTime - wsm->getTimestamp() << ";"
                << oVision->getMyData()->getId() << ";"
                << oVision->getMyData()->getAgentName()
                //<< oVision->getMyData()->infoTrace(header)
                << ";" << wsm->getNumMsg() << ";" << wsm->getName() << ";"
                << wsm->getMsgType() << ";" << wsm->getMsgLifeTime()

                << ";" << ruleRoute << ";" << wsm->getValidityDataTimeStamp()
                << ";" << wsm->getHopNumber()
                // << ";" << wsm->getSourceName()
                << ";" << wsm->getSourceId()
                // << ";" << wsm->getAntecessorName()
                << ";" << wsm->getAntecessorId()
                // << ";" << wsm->getNextName()
                << ";" << wsm->getNextId()
                // << ";" << wsm->getDestinationName()
                << ";" << wsm->getDestinationId()

                << ";" << wsm->getAntecessorMsgTimeStamp() << ";"
                << wsm->getAntecessoPosTimeStamp()

                << ";" << wsm->getAntecessorMaxSpeed()

                << ";" << wsm->getAntecessorSpeed() << ";"
                << wsm->getAntecessorX() << ";" << wsm->getAntecessorY() << ";"
                << wsm->getAntecessorZ() << ";"
                << wsm->getAnteNextValidityTimeStamp()
                // idade da mobilidade do agente antecessor  no evento de tranmissao no agente transmissor - o tempo do sensor
                << ";" << wsm->getTimestamp() - wsm->getAntecessoPosTimeStamp()

                << ";" << wsm->getSourceMsgTimeStamp() << ";"
                << wsm->getSourcePosTimeStamp() << ";"
                << wsm->getSourceMaxSpeed()
                //<< ";" << wsm->getSourceRadius()
                << ";" << wsm->getSourceSpeed() << ";" << wsm->getSourceX()
                << ";" << wsm->getSourceY() << ";" << wsm->getSourceZ()

                // idade da mobilidade do agente origem no evento de tranmissao do agente origem
                << ";"
                << wsm->getSourceMsgTimeStamp() - wsm->getSourcePosTimeStamp()
                // idade da mobilidade do agente origem no evento de tranmissao do agente antecessor
                << ";" << wsm->getTimestamp() - wsm->getSourcePosTimeStamp()
                // iadade da mobilidade do agente de origem na recepcao da msg no agente local
                << ";" << pEventTime - wsm->getSourcePosTimeStamp()

                << ";" << wsm->getNextPosTimeStamp() << ";"
                << wsm->getNextMaxSpeed()
                // << ";" << wsm->getNextRadius()
                << ";" << wsm->getNextSpeed() << ";" << wsm->getNextX() << ";"
                << wsm->getNextY() << ";" << wsm->getNextZ()

                // idade da mobilidade do agente next no evento de tranmissao do agente origem
                << ";"
                << wsm->getSourceMsgTimeStamp() - wsm->getNextPosTimeStamp()
                // idade da mobilidade do agente next no evento de tranmissao do agente antecessor
                << ";" << wsm->getTimestamp() - wsm->getNextPosTimeStamp()

                << ";" << wsm->getDestinationPosTimeStamp() << ";"
                << wsm->getDestinationMaxSpeed()
                //<< ";" << wsm->getDestinationRadius()
                << ";" << wsm->getDestinationSpeed() << ";"
                << wsm->getDestinationX() << ";" << wsm->getDestinationY()
                << ";" << wsm->getDestinationZ()

                // idade da mobilidade do agente next no evento de tranmissao do agente origem
                << ";"
                << wsm->getSourceMsgTimeStamp()
                        - wsm->getDestinationPosTimeStamp()
                // idade da mobilidade do agente next no evento de tranmissao do agente antecessor
                << ";"
                << wsm->getTimestamp() - wsm->getDestinationPosTimeStamp();

        //<< ";" << wsm->getWsmData() << ";";

        /*  if (evento!="receiving"){
         // iadade da mobilidade do agente de antecessor na recepcao da msg no agente local
         ost << ";" <<  pEventTime -   wsm->getAntecessoPosTimeStamp()
         // idade da mobilidade do agente next na recepcao da msg no agente local
         << ";" <<  pEventTime -  wsm->getNextPosTimeStamp()
         // idade da mobilidade do agente next na recepcao da msg no agente local
         << ";" <<  pEventTime -  wsm->getDestinationPosTimeStamp()
         }
         */
        ost << std::endl;
    }

    return ost.str();
}

std::string Icarqoc::getMsgHeaderInfoTrace(ICMessage * wsm, std::string evento,
        bool header, simtime_t pEventTime) {
    std::ostringstream ost;
    if (header) {
        ost << "event" << ";eventTime" << ";receptorMsgName" << ";SourceName"
                << ";AntecessorName" << ";NextName" << ";DestinationName"
                << ";messageNumber" << ";Name" << ";MsgType" << ";timestampMSG"
                << ";antecessorMsgTimeStamp" << ";delayMSG" << ";MsgLifeTime"
                << ";hopNumber" << ";ValidityDataTimeStamp" << ";distance_rl";
    } else {
        ost << evento << ";" << pEventTime << ";" << this->myData->getId()
                << ";" << wsm->getSourceId() << ";" << wsm->getAntecessorId()
                << ";" << wsm->getNextId() << ";" << wsm->getDestinationId()
                << ";" << wsm->getNumMsg() << ";" << wsm->getName() << ";"
                << wsm->getMsgType() << ";" << wsm->getTimestamp() << ";"
                << wsm->getAntecessorMsgTimeStamp() << ";"
                << pEventTime - wsm->getTimestamp() << ";"
                << wsm->getMsgLifeTime() << ";" << wsm->getHopNumber() << ";"
                << wsm->getValidityDataTimeStamp() << ";"
                << this->oKnownGlobal->calcTraciDistanceMobility(
                        wsm->getSourceId(), this->myData->getId());

        //<< std::endl;
    }

    return ost.str();
}
