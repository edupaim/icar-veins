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

#include <veins_inet/modules/application/icarnew/LocalMobilityVeinsInet.h>

LocalMobilityVeinsInet::LocalMobilityVeinsInet() {
    // TODO Auto-generated constructor stub

}

LocalMobilityVeinsInet:: ~LocalMobilityVeinsInet() {
    // TODO Auto-generated destructor stub
}

Move * LocalMobilityVeinsInet::getMove(){
    Move *move = new Move();
//    move->setStart(this->getCurrentPosition());
//    move->setSpeed(this->getCurrentSpeed());
    return move;

}

//std::string LocalMobilityVeinsInet::getExternal_id() {
//    return this->external_id;
//}

//double LocalMobilityVeinsInet::getAntennaPositionOffset() {
//    return this->antennaPositionOffset;
//}

//simtime_t LocalMobilityVeinsInet::getLastUpdate() {
//    return this->get;
//}

//Coord LocalMobilityVeinsInet::getNextRoadPosition() {
//    return this->roadPosition;
//}

//std::string LocalMobilityVeinsInet::getNextRoad_id() {
//    return this->road_id;
//}

//double LocalMobilityVeinsInet::getNextSpeed() {
//    return this->speed;
//}

//double LocalMobilityVeinsInet::getNextAngle() {
//    return this->angle;
//}


