//
// This program is free software: you can redistribute it and/or modifygetMobilityInfo()
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

#include "RemoteAgent.h"

RemoteAgent::RemoteAgent() {
    // TODO Auto-generated constructor stub

}


RemoteAgent::RemoteAgent(simtime_t tsDataUpdating, int64_t id, simtime_t tsStart, double x, double y,  double z, double speed, double maxSpeed, double setRadius,
        std::string agentName, float acceleration, float angle) {
    this->qocInfo = new RemoteAgentQoC();
    this->setId(id);
    this->setMobilityInfo(new RemoteMobility(tsStart, x, y, z, speed, maxSpeed, acceleration, angle));
    setSetRadius(setRadius);
    this->setAgentName(agentName);
    this->setCommInfo (new RemoteAgentCommInfo());
    this->getMobilityInfo()->setAge((tsDataUpdating - tsStart).dbl());
}
void RemoteAgent::createAgent(simtime_t tsDataUpdating, int64_t id, simtime_t tsStart, double x, double y,  double z, double speed, double maxSpeed, double setRadius,
        std::string agentName, float acceleration, float angle) {
    this->qocInfo = new RemoteAgentQoC();
    this->setId(id);
    this->setMobilityInfo(new RemoteMobility(tsStart, x, y, z, speed, maxSpeed, acceleration, angle));
    setSetRadius(setRadius);
    this->setAgentName(agentName);
    this->setCommInfo (new RemoteAgentCommInfo());
    this->getMobilityInfo()->setAge((tsDataUpdating - tsStart).dbl());
}
RemoteAgent::RemoteAgent(simtime_t tsDataUpdating, ICRNode oIcrNode, double setRadius)
{
    this->createAgent(tsDataUpdating, oIcrNode.nodeId, oIcrNode.posTimeStamp,
            oIcrNode.posX, oIcrNode.posY,oIcrNode.posZ, oIcrNode.speed,
            oIcrNode.maxSpeed, setRadius, std::to_string(oIcrNode.nodeId),
            oIcrNode.aceleration, oIcrNode.angle);
}

void RemoteAgent::updateRemoteAgent(simtime_t tsDataUpdating, ICRNode oIcrNode, double setRadius) {
    updateRemoteAgent(tsDataUpdating, oIcrNode.posTimeStamp, oIcrNode.posX, oIcrNode.posY, oIcrNode.posZ, oIcrNode.speed, oIcrNode.maxSpeed, setRadius, oIcrNode.aceleration, oIcrNode.angle);
}

void RemoteAgent::updateRemoteAgent(simtime_t tsDataUpdating, simtime_t tsMobilityData, double x, double y,  double z, double speed, double maxSpeed, double radius,
        float acceleration, float angle) {
    RemoteMobility* rm = ((RemoteMobility*) getMobilityInfo());
    if (rm->getActualMove()->getStartTime()< tsMobilityData){
        rm->updateMobilityInfo(tsMobilityData, x, y, z, speed, maxSpeed, acceleration, angle);
    }
    //atualizao do raio de comunicao do agente remoto
    if (radius!=0) this->getRemoteAgentCommInfo()->setRadius(radius);
    this->getMobilityInfo()->setAge((tsDataUpdating - tsMobilityData).dbl());
}

RemoteAgent::~RemoteAgent() {
  // TODO Auto-generated destructor stub
    delete(this);
}

std::string RemoteAgent::info() {
    std::ostringstream ost;
    ost << Agent::info()  << "  Remote Mobility: " << ((RemoteMobility*) getMobilityInfo())->info()
                          << "  Remote CommInfo: " << this->getRemoteAgentCommInfo()->info();
    return ost.str();
}

std::string RemoteAgent::infoTrace(bool header=false, std::string  idagent=""){
    std::ostringstream ost;
    ost <<  Agent::infoTrace(header, idagent) << this->getRemoteAgentCommInfo()->infoTrace(header, idagent);
    return ost.str();
}

MobilityInfo* RemoteAgent::getMobilityInfo() {
    return ((MobilityInfo*) mobilityInfo);
}


RemoteAgentCommInfo* RemoteAgent::getRemoteAgentCommInfo() {
    return ((RemoteAgentCommInfo*) this->commInfo);
}
