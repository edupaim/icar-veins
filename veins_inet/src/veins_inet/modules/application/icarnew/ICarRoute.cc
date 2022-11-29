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

#include <veins_inet/modules/application/icarnew/ICarRoute.h>

ICarRoute::ICarRoute() {
    // TODO Auto-generated constructor stub

}

ICarRoute::ICarRoute(ICRMessage* wsm, simtime_t pMsgReceivingTime, simtime_t pQocValidityTime,
        std::vector<AgentPair *> pRouteAgents) {

    qocValidityTime = pQocValidityTime;
    qocConfidence = wsm->getConfidence();
    hopNumber = wsm->getHopNumber();
    routeAgentPairs = pRouteAgents;
}



ICarRoute::ICarRoute(simtime_t pQocValidityTime, double pQocConfidence, int pHopNumber) {
    qocValidityTime = pQocValidityTime;
    qocConfidence =pQocConfidence;
    hopNumber = pHopNumber;
}

ICarRoute::~ICarRoute() {
    // TODO Auto-generated destructor stub
}

void ICarRoute::update(){
    AgentPair *  c;
    for (int index=0; index < routeAgentPairs.size(); index++){
        //routeAgentPairs.at(index)->getQoCAgentPair()->getTimelinessAgentPair()->
    }
}

std::string ICarRoute::infoTrace(bool header) {
    std::ostringstream ost;
     if (header){
       ost     << ";hop"
               << ";qocValidityTime"
               <<  ";qocConfidence";
   } else {
       ost
           << ";" << this->hopNumber
           << ";" << this->qocValidityTime
           << ";" << this->qocConfidence;

   }
    return ost.str();
}

