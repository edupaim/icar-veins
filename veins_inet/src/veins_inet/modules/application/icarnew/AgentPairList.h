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

#ifndef AGENTPAIRLIST_H_
#define AGENTPAIRLIST_H_
#include <veins_inet/modules/application/icarnew/AgentPair.h>
#include <iostream>
#include <vector>

class AgentPairList;
using namespace std;

class AgentPairList{
public:
    std::vector<AgentPair*> agentPairs;
    AgentPairList();
    virtual ~AgentPairList();
    AgentPair * getAgentPair(int64_t sourceId, int64_t destinationId);
    int getAgentPairIndex(int64_t sourceId , int64_t destinationId);
    std::vector<AgentPair*> listNeighbors(int64_t agentId);
    std::vector<AgentPair*> listAgentPairsWith(int64_t agentId);
    AgentPair* getAgentPair(int64_t sourceId, int64_t destinationId, std::vector<AgentPair*> aAgentPairs);
    double calcMinDistancelr(int64_t agent);
    double calcMaxDistancelr(int64_t agent);
    double calcMaxSlr(int64_t agent);
    double calcMinSlr(int64_t agent);
    std::string infoTraceAgentPairList();
    AgentPair * updatePairListFromMsg(long numMsg, Agent* agentS, Agent* agentD, simtime_t pMsgReceivingTime, simtime_t msgSendingTime,
            RadiusEstimatorAgentPair * pRadiusEstimator, int neighbor, int event, simtime_t dataValidity, double pPeriodApp, double powerReceived_dbm, double snr);
};

#endif /* AgentPairLIST_H_ */
