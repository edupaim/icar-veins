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

#ifndef SRC_VEINS_MODULES_APPLICATION_ICARNEW_ICARROUTE_H_
#define SRC_VEINS_MODULES_APPLICATION_ICARNEW_ICARROUTE_H_

#include <omnetpp.h>
#include "messages/ICRMessage_m.h"
#include <vector>
#include "AgentPair.h"
#include "RemoteAgent.h"
#include "LocalAgent.h"

class ICarRoute {
public:
    ICarRoute(ICRMessage * wsm, simtime_t pMsgReceivingTime, simtime_t pQocValidityTime, std::vector<AgentPair *> pRouteAgents);
    ICarRoute();
    virtual ~ICarRoute();
    simtime_t qocValidityTime;
    double qocConfidence;
    unsigned int hopNumber;
    std::vector<AgentPair *> routeAgentPairs;
    ICRMessage * actualWsm;
    std::string infoTrace(bool header);
    ICarRoute(simtime_t pQocValidityTime, double pQocConfidence, int phopNumber);
    void update();
};

#endif /* SRC_VEINS_MODULES_APPLICATION_ICARNEW_ICARROUTE_H_ */
