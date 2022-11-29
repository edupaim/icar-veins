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

#include <veins_inet/modules/application/icarnew/messages/ICRNode.h>

ICRNode::ICRNode() {
    // TODO Auto-generated constructor stub

}

ICRNode::~ICRNode() {
    // TODO Auto-generated destructor stub
}

void ICRNode::prepareICRNode(Agent* a) {
    maxSpeed = a->getMobilityInfo()->getMaxSpeed();
    angle = a->getMobilityInfo()->getAngle();
    aceleration = a->getMobilityInfo()->getAcceleration();
    posTimeStamp = a->getMobilityInfo()->getActualMove()->getStartTime();
    speed = a->getMobilityInfo()->getActualMove()->getSpeed();
    posX = a->getMobilityInfo()->getActualMove()->getStartPos().x;
    posY = a->getMobilityInfo()->getActualMove()->getStartPos().y;
    posZ = a->getMobilityInfo()->getActualMove()->getStartPos().z;
    msgTimeStamp = simTime();
    nodeId = a->getId();
}
