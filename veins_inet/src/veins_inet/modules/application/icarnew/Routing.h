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

#ifndef ROUTING_H_
#define ROUTING_H_

#include <veins_inet/modules/application/icarnew/AgentPair.h>
#include <veins_inet/modules/application/icarnew/AgentPairList.h>
#include <iostream>
#include "Agent.h"
#include "CommState.h"
#include "RemoteAgentGroup.h"
#include "ForwardingNode.h"

class Routing {
private:
    AgentPairList * agentPairList;
    int64_t destinationId;
    int64_t sourceId;


    double msgTimeToLive;
    Coord target1;
    Coord target2;
    double validityTime;
    RemoteAgentGroup * groupAgent;
    double userMessageLifeTime;
    double expectedConfidence;
    ICarRoute * searchRoute(AgentPair * c);
    ICarRoute * getRoute(int s, int d);
    ICarRoute * getFowardNode(int s, int d);
    ForwardingNode * searchNode();
    AgentPair * verifyAgentPairc(int64_t pSource, int64_t pDestination, string rule, double msgTTime);
    ForwardingNode * verifyAgentPair(int64_t pSource, int64_t pDestination, string rule, double msgTTime);

    ForwardingNode * verifyAgentPairInverse(int64_t pDestination, int64_t pSource, string rule, double msgTTime);
    ForwardingNode * agentGeoNear(RemoteAgent *destination, string rule);
    ForwardingNode * verifyAgentPairAneib(int64_t agentId, string rule, double msgTTime);
    ForwardingNode * verifyConditionForwardAgent(AgentPair * c, string rule, double msgTTime);
    ForwardingNode * verifyAgentPairNotNeighbor(int64_t pSource,  int64_t pDestination, string rule, double msgTTime);
    ForwardingNode * verifyConditionForwardNotNeighbor(AgentPair * c, string rule, double msgTTime);

public:
    Routing();

    virtual ~Routing();
    ForwardingNode * rule4();

    ForwardingNode * getFowardNode( RemoteAgentGroup * groupAgent, AgentPairList * agentPairList, int64_t destinationId, int64_t sourceId,  int64_t pAntecessorMsgId, double msgTimeToLive, Coord target1, Coord target2);

    ForwardingNode * rule2();
    ForwardingNode * rule3();
    ForwardingNode * oldRule();


};

#endif /* ROUTING_H_ */
