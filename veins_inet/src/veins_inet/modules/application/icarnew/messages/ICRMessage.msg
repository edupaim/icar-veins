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

//
// TODO generated message class
//

cplusplus {{
#include "veins/base/utils/Coord.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/base/utils/SimpleAddress.h"
#include "veins_inet/modules/application/icarnew/messages/ICRNode.h"
}}

class WaveShortMessage;
class noncobject Coord;
class noncobject ICRNode;
class LAddress::L2Type extends void;

packet ICRMessage extends WaveShortMessage {
   unsigned long numMsg; // number of message
   int msgType; // message type 
   simtime_t msgLifeTime; // message life time - 16 bits
   simtime_t validityDataTimeStamp; // validity timeout of the menor neighborhood condition of the route nodes
   double confidence;
   unsigned int hopNumber;
   simtime_t msgTimeStamp = 0; // timestamp da mensagem

   ICRNode routeNodes[];
}
