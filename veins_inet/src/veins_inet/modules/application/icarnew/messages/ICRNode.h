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

#ifndef SRC_VEINS_MODULES_APPLICATION_ICARNEW_MESSAGES_ICRNODE_H_
#define SRC_VEINS_MODULES_APPLICATION_ICARNEW_MESSAGES_ICRNODE_H_

//#include "veins/veins.h"
#include <omnetpp.h>
#include "../Agent.h"

class ICRNode {
public:
    long  nodeId;  // source id node - 32bits
    float posX;  //source x - 32 bits
    float posY; // source y - 32 bits
    float posZ; // source z - 32 bits
    float maxSpeed;  // max speed node - 32 bits
    float angle;  // angulo do agente de origem -  32 bits
    float aceleration;  // aceleracao do agente de origem -  32 bits
    float speed;  // speed value -  32 bits
    bool neighber; // vizinho do proximo no
    float confidence;
    simtime_t validityTimeToNext; // alcance do sinal de comunicacao do agente de origem  - 32 bits
    simtime_t  posTimeStamp; // timestamp da da coleta da posicao do agente antecessor
    simtime_t  msgTimeStamp;
    ICRNode();
    virtual ~ICRNode();
    void prepareICRNode(Agent * agent);

};

#endif /* SRC_VEINS_MODULES_APPLICATION_ICARNEW_MESSAGES_ICRNODE_H_ */
