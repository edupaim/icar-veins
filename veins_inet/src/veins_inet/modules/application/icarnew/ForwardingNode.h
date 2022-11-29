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

#ifndef FORWARDINGNODE_H_
#define FORWARDINGNODE_H_
#include <omnetpp.h>
#include <veins_inet/modules/application/icarnew/AgentPair.h>
#include <iostream>

class AgentPair;

class ForwardingNode {
private:
    int64_t fowardId;
    double dataValidity;
    simtime_t timeoutValidity;
    std::string rule;
    AgentPair * agentPair;
    Agent * fowardingAgent;
public:
    ForwardingNode();
    virtual ~ForwardingNode();
    void update(std::string rule, int fowardingAgent, AgentPair * c, double periodvalidty);
    void update(std::string rule, Agent * fowardingAgent, AgentPair * c, double periodvalidty);
    double getDataValidity();
    void setDataValidity(double dataValidity);
    int64_t getFowardId();
    void setFowardId(int64_t afowardId);
    std::string getRule();
    void setRule(std::string rule);
    const simtime_t getTimeoutValidity();
    void setTimeoutValidity(simtime_t timeoutValidity);
    AgentPair* getAgentPair();
    void setAgentPair(AgentPair* agentPair);
};

#endif /* FORWARDINGNODE_H_ */
