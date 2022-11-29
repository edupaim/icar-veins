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

#ifndef SRC_VEINS_MODULES_APPLICATION_ICARNEW_ICARROUTING_H_
#define SRC_VEINS_MODULES_APPLICATION_ICARNEW_ICARROUTING_H_
#include "ForwardingNode.h"
#include "AgentPair.h"
#include "AgentPairList.h"
#include "RemoteAgentGroup.h"

class IcarRouting {
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

public:
    IcarRouting();
    ForwardingNode * selectForwardingNode();
    virtual ~IcarRouting();
    IcarRouting(RemoteAgentGroup* pGroupAgent, AgentPairList* pAgentPairList);
    AgentPair * verifyAgentPairContext(int64_t pSource, int64_t pDestination, string rule, double msgTTime);
    bool  verifyAgentPairContext(AgentPair * c, double msgTTime);
    AgentPair * verifyAgentPairRoute(int64_t pSource, int64_t pDestination, double msgTTime);
    ForwardingNode* getFowardNode(int64_t destinationId, int64_t sourceId, int64_t localId, double msgTimeToLive, Coord target1, Coord target2);

};

#endif /* SRC_VEINS_MODULES_APPLICATION_ICARNEW_ICARROUTING_H_ */
