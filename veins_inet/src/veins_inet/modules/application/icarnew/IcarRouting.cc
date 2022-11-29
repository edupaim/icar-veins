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

#include <veins_inet/modules/application/icarnew/IcarRouting.h>

IcarRouting::IcarRouting(RemoteAgentGroup* pGroupAgent,
        AgentPairList* pAgentPairList) {
    // TODO Auto-generated constructor stub
    this->agentPairList = pAgentPairList;
    this->groupAgent = pGroupAgent;
}

IcarRouting::IcarRouting() {
    // TODO Auto-generated constructor stub
}

IcarRouting::~IcarRouting() {
    // TODO Auto-generated destructor stub
}

/*
 * Verify if the channel is communicating and neighbor between source and destiny
 */
AgentPair * IcarRouting::verifyAgentPairRoute(int64_t pSource, int64_t pDestination, double msgTTime)
{
    AgentPair *  pair= NULL;;

    AgentPair * c = this->agentPairList->getAgentPair(pSource, pDestination);

    c = agentPairList->getAgentPair(pSource, pDestination);
    if (c!=NULL)
      {
        bool answer=false;
        unsigned int i=0;

        while (i < c->pairRoutes.size() && !answer)
        {
            ICarRoute * wRoute =  c->pairRoutes.at(i);

            if (wRoute->qocValidityTime > simTime()) {
                    answer=verifyAgentPairContext( wRoute->routeAgentPairs.at(0), msgTTime);
                    if (answer)
                        pair = wRoute->routeAgentPairs.at(0);
            }
            i++;

        }

      }
    return pair;
}

/*
 * Verify if the channel is communicating and neighbor between source and destiny
 */
bool IcarRouting::verifyAgentPairContext(AgentPair * c,  double msgTTime)
{
    bool resp = false;
    if (c!=NULL)
    {

        //verify the communication state and if is neighbor
        if ((c->getCommnunicationState()!= CommState::noCommunicating
                && c->getCommnunicationState()!= CommState::faulty)
                && ( simTime() + c->getMessageTTime()).dbl() <= msgTimeToLive)
            {
                resp = true;
            }
     }

    return resp;
}

/*
 * Verify if the channel is communicating and neighbor between source and destiny
 */
AgentPair * IcarRouting::verifyAgentPairContext(int64_t pSource, int64_t pDestination, string rule, double msgTTime)
{
    AgentPair * resp = NULL;
    AgentPair *  c;

    if (pDestination!=-1)
        c = agentPairList->getAgentPair(pSource, pDestination);

    if (c!=NULL)
    {
        //verify the communication state and if is neighbor
            if ((c->getCommnunicationState()!= CommState::noCommunicating
                    && c->getCommnunicationState()!= CommState::faulty))
                    //&& c->isNeighbors() && ( simTime() + c->getMessageTTime()).dbl() <= msgTimeToLive)
                {
                    resp = c;
                }
        }

    return resp;
}

ForwardingNode* IcarRouting::getFowardNode(int64_t destinationId, int64_t sourceId, int64_t localId, double msgTimeToLive, Coord target1,
        Coord target2) {

        Agent * fowardingAgent;
        ForwardingNode* fw=NULL;
        AgentPair * c = verifyAgentPairContext(sourceId, destinationId, "teste", msgTimeToLive);

        if (c!=NULL){
            if (c->getAgentS()->getId()==localId ){
                fowardingAgent = c->getAgentD();
            }else fowardingAgent = c->getAgentS();

            fw = new ForwardingNode();
            fw->update("teste", fowardingAgent, c, 0);
        }

        return fw;
}

