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

#ifndef SRC_VEINS_MODULES_APPLICATION_ICARNEW_ICARCONTEXT_H_
#define SRC_VEINS_MODULES_APPLICATION_ICARNEW_ICARCONTEXT_H_

#include <omnetpp.h>
#include <veins_inet/modules/application/icarnew/AgentPair.h>
#include <veins_inet/modules/application/icarnew/AgentPairList.h>
#include <veins_inet/modules/application/icarnew/GeneralCommunicationService.h>
#include "messages/ICRMessage_m.h"
#include <vector>
#include "Agent.h"
#include "RemoteAgent.h"
#include "LocalAgent.h"
#include "LocalMobilityTraci.h"
#include "RemoteAgentGroup.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "IcarRouting.h"
#include "KnownGlobal.h"
#include "veins/base/phyLayer/PhyToMacControlInfo.h"
#include "veins/modules/phy/DeciderResult80211.h"
//#include "veins/modules/messages/BaseFrame1609_4_m.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins_inet/modules/application/icarnew/messages/ICRNode.h"
#include "Icarqoc.h"
#include "ICarRoute.h"


class Icarqoc;


class IcarContext {
public:
////////// STATISTIC ///////////////////////////////
    // buffer file trace Channel;
     std::stringstream QoCQoSdataAgentPair;

    // buffer file trace Channel;
     std::stringstream dataAgentPair;

     // buffer file trace Minslr;
     std::stringstream dataMinslr;

     // buffer file trace Remote Agent;
     std::stringstream dataRemoteAgent;

     // buffer file trace Remote Agent;
     std::stringstream dataLocalAgent;

     // buffer file trace Communication performance;
     std::stringstream dataCommPerformance;

////////// STATISTIC ///////////////////////////////
    // data agent local
    LocalAgent* myData;

    // PS: Data local agent are not in this list
    RemoteAgentGroup * agentGroup;

    AgentPairList * agentPairList;

    KnownGlobal * oKnownGlobal;

    // routing service default
    IcarRouting * routing;

    // general communication Service (Application - Clovis)
    GeneralCommunicationService service;

    Icarqoc * oIcarModule;
    IcarContext();
    IcarContext(LocalAgent * oMyData, KnownGlobal * pKnownGlobal, Icarqoc * pIcar);
    virtual ~IcarContext();
    void callFinish();
    void updateAgentPairsFromMsg(ICRMessage * wsm, simtime_t pMsgReceivingTime);
    double getMsgTransmitterRange(ICRMessage* m);
    RemoteAgent* updateAgentGroup(ICRNode m, simtime_t pMsgReceivingTime);
    void updateContextfromMsg(ICRMessage * wsm, simtime_t pMsgReceivingTime);
    void changeLocalMobility();
    RemoteAgentGroup* getAgentGroup();
    void setAgentGroup(RemoteAgentGroup* agentGroup);
    AgentPairList* getAgentPairList() ;
    void setAgentPairList(AgentPairList* agentPairList);
    LocalAgent* getMyData() ;
    void setMyData(LocalAgent* myData);
    void updateAgentPairs(Agent * agent, simtime_t timeStampData, int event);
    std::string textDataAgentPair(int event, AgentPair * c, bool header);
    Icarqoc* getIcarModule();
    void setIcarModule( Icarqoc* icarModule);
    KnownGlobal* getKnownGlobal() ;
    void setKnownGlobal(KnownGlobal* knownGlobal);
    IcarRouting* getRouting();
    void setRouting( IcarRouting* routing);
    GeneralCommunicationService getService();
    void setService(GeneralCommunicationService service);
    std::string infoTraceAgentPairList();

};

#endif /* SRC_VEINS_MODULES_APPLICATION_ICARNEW_ICARCONTEXT_H_ */
