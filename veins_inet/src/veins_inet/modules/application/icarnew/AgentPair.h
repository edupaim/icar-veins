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
 /*  LAddress::L3Type networkAdress in this version we consider that a car have only one network device;
 *
*/

#ifndef AGENTPAIR_H_
#define AGENTPAIR_H_

//#ifndef DBG
//#define DBG EV
//#endif
#define DBG std::cerr << "[" << simTime().raw() << "] " << getParentModule()->getFullPath() << " "

#include "messages/ChannelMonitor_m.h"
#include "messages/ICMessage_m.h"
#include <vector>
#include "CommState.h"
#include "Agent.h"
#include "RemoteAgent.h"
#include "LocalAgent.h"
#include "Route.h"
//#include <veins/modules/application/icarnew/AgentPairMobility.h>
//#include <veins/modules/application/icarnew/QoCAgentPair.h>
//#include <veins/modules/application/icarnew/RadiusEstimatorAgentPair.h>
#include "AgentPairMobility.h"
#include "QoCAgentPair.h"
#include "RadiusEstimatorAgentPair.h"
#include "ICarRoute.h"

class QoCAgentPair;
class AgentPairMobility;
class ICarRoute;

class AgentPair {
private:
    /*
     * Agente origem da mensagem ou o que transmitiu mais recentemente
     */
    Agent *agent_s;

    /*
     * Agente que recebeu ou receber� a mensagem
     */
    Agent *agent_d;

    /*
     * Agente que informou sobre este canal -- que transmitiu a mensagem
     */
    Agent *agent_ds;

    /*
     * Mobilidade do canal
     */
    AgentPairMobility* agentPairMobility;


    RadiusEstimatorAgentPair * radiusEstimator;

    double outOfRadiusRange;


    /*
     * raio estimado da origem ao destino
     */
    double calcRadius;
    /*
     * @brief txReceived_dBm power received from phy layer
    */
    double txReceived_dBm;

    /**
     * @brief Stores the signal to noise ratio of the transmission
     * */
    double snr;
    /*
    * @brief Neighbor = 0 -- nao vizinho; neighbor= 1 -- eh vizinho; neighbor = -1 condicao desconhecida
    */
    int neighbors;

    int commnunicationState;

    QoCAgentPair * qoCAgentPair;

    /*
    * @brief
    */
    double messageTTime;

    double oPeriodApplication; // periodo de carga de aplicação

    simtime_t penultTMsgSending;
    /*
    * @brief
    */
    simtime_t lastTMsgSending;
    /*
    * @brief
    */
    simtime_t lastTMsgReceiving;
    /*
    * @brief
    */
    simtime_t currentTMsgReceiving;

    /*
    * @brief
    */
    long transmittedMsgAmount;
    /*
    * @brief
    */
    simtime_t updatingTime;

    cMessage * agentPairMessage;

    long receivedMessageAmount;

    long lastMSGNumber;
    long currentMSGNumber;


public:

    char lastEvent;
    double drlRadius;
    std::vector<ICarRoute *> pairRoutes;

    AgentPair();
    virtual ~AgentPair();

    AgentPair(Agent* agentS, Agent* agentD, simtime_t msgReceivingTime, simtime_t msgSendingTime, RadiusEstimatorAgentPair * pRadiusEstimator,
            int neighbor, int event, simtime_t timeoutrouteValidity, double pPeriodApp, double powerReceived_dbm, double snr);

    void updateAgentPair(simtime_t msgReceivingTime, simtime_t msgSendingTime, int neighbor,int event,
            simtime_t timeoutdataValidity, double powerReceived_dbm, double snr );

    void updateAgentPair();

    AgentPair * updatePairFromMsg(long numMsg, Agent* agentS, Agent* agentD, simtime_t msgReceivingTime, simtime_t msgSendingTime, RadiusEstimatorAgentPair * pRadiusEstimator, int neighbor, int event, simtime_t dataValidity, double pPeriodApp, double powerReceived_dbm, double snr);

    void updateRoute(ICMessage* m);

    //void updateCommnunicationState(int event);
    void updateCommnunicationState();
    void updateNeighborhoodCondition();

    Route * getRoute(ICMessage* m);

    bool isInAgentPair(int id);

    void addReceivedMsg();

    long getReceivedMessageAmount();


    void setCalcRadius();
    /**
        * @brief
        * @param
        * @return
        */
    Agent* getAgentD();
    /**
        * @brief
        * @param
        * @return
        */
    void setAgentD(Agent* agentD);

    /**
        * @brief
        * @param
        * @return
        */
    Agent* getAgentS();
    /**
        * @brief
        * @param
        * @return
        */
    void setAgentS( Agent* agentS);


    /**
        * @brief
        * @param
        * @return
        */
    double getCalcRadius();
    double getMessageDelay();
    /**
        * @brief
        * @param
        * @return
        */
    AgentPairMobility* getAgentPairMobility();

    /**
        * @brief
        * @param
        * @return
        */
    void setAgentPairMobility(AgentPairMobility * agentPairMobility);

    /**
        * @brief
        * @param
        * @return
        */
    int getCommnunicationState();

    /**
        * @brief
        * @param
        * @return
        */
    void setCommnunicationState(int commnunicationState);

    /**
        * @brief
        * @param
        * @return
        */
    simtime_t getCurrentTMsgReceiving();

    /**
        * @brief
        * @param
        * @return
        */
    void setCurrentTMsgReceiving(simtime_t correntTMsgReceiving);

    simtime_t getLastTMsgReceiving() ;
    /**
        * @brief
        * @param
        * @return
        */

    void setLastTMsgReceiving(simtime_t lastTMsgReceiving);
    /**
        * @brief
        * @param
        * @return
        */
    double getMessageTTime();
    /**
        * @brief
        * @param
        * @return
        */
    void setMessageTTime(double messageTTime);

    /**
        * @brief
        * @param
        * @return
        */
    bool isNeighbors();

    /**
        * @brief
        * @param
        * @return
        */
    void setNeighbors(int neighbors);


    // return the string with all attributes data  of this object
    std::string info();

    int getNeighbors();
    Agent* getAgentDs();
    void setAgentDs(Agent* agentDs);
    QoCAgentPair* getQoCAgentPair();
    void setQoCAgentPair(QoCAgentPair* qoCAgentPair);

    std::string infoTrace(bool header);
    std::string infoTraceRoutes(bool header);

    double getPeriodApplication();
    void setPeriodApplication(double periodApplication);
    double getTxReceivedDBm();
    void setTxReceivedDBm(double txReceivedDBm);
    cMessage* getAgentPairMessage();
    void setAgentPairMessage(cMessage* channelMessage);
    double getOutOfRadiusRangeState();
    void setOutOfRadiusRangeState();
    simtime_t getLastTMsgSending();
    void setLastTMsgSending(simtime_t lastTMsgSending);
    void setUpdatingTime(simtime_t timeupdate);
    simtime_t getUpdatingTime();
    long getLastMsgNumber();
    void setLastMsgNumber(long lastMsgNumber);
    long getTransmittedMsgAmount();
    void setTransmittedMsgAmount(long msgNumber);
    long getCurrentMsgNumber();
    void setCurrentMsgNumber(long currentMsgNumber);
    double getSnr();
    void setSnr(double snr);
};

#endif /* CHANNEL_H_ */
