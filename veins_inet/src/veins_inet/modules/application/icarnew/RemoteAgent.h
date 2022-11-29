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

#ifndef REMOTEAGENT_H_
#define REMOTEAGENT_H_
#include "RemoteMobility.h"
#include "Agent.h"
#include "math.h"
#include "RemoteAgentCommInfo.h"
#include "RemoteAgentQoC.h"
#include "veins_inet/modules/application/icarnew/messages/ICRNode.h"

class RemoteAgent : public Agent {
private:

public:
    RemoteAgent();

    RemoteAgent(simtime_t tsDataUpdating, int64_t id, simtime_t sTimePosition, double x, double y,  double z, double speed, double maxSpeed, double setRadius,
            std::string agentName, float acceleration, float angle);
        RemoteAgent(simtime_t tsDataUpdating, ICRNode oIcrNode, double setRadius);

        void createAgent(simtime_t tsDataUpdating, int64_t id, simtime_t tsStart, double x, double y,  double z,double speed, double maxSpeed, double setRadius,std::string agentName, float acceleration, float angle);

    virtual ~RemoteAgent();

    void updateRemoteAgent(simtime_t tsDataUpdating, simtime_t tsMovement, double x, double y,  double z, double speed, double maxSpeed, double setRadius,
            float acceleration, float angle);
    void updateRemoteAgent(simtime_t tsDataUpdating, ICRNode oIcrNode, double setRadius);

    std::string info();
    std::string infoTrace(bool header, std::string idagent);
    MobilityInfo* getMobilityInfo();
    RemoteAgentCommInfo* getRemoteAgentCommInfo();
};

#endif /* REMOTEAGENT_H_ */
