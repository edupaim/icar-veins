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

#include <veins_inet/modules/application/icarnew/LocalAgentCommPerformance.h>

LocalAgentCommPerformance::LocalAgentCommPerformance() {
    // TODO Auto-generated constructor stub
    this->recievedMsgTemp=0;
    this->rateRecievedMSGPperiod=0;
    this->totalReceivedMsg=0;
    this->totalForwardedMsg=0;
    this->totalTransmittedMsg=0;
}

long LocalAgentCommPerformance::getTotalReceivedMsg(){
    return totalReceivedMsg;
}

long LocalAgentCommPerformance::getTotalTransmittedMsg() {
    return totalTransmittedMsg;
}

long LocalAgentCommPerformance::getRecievedMsgTemp(){
    return recievedMsgTemp;
}


void LocalAgentCommPerformance::calcRateRecievedMsgPperiod() {
    this->rateRecievedMSGPperiod=this->recievedMsgTemp;
    this->recievedMsgTemp=0;
}

void LocalAgentCommPerformance::addCorrectMsgRecievedMsg() {
    this->recievedMsgTemp++;
    this->totalReceivedMsg++;
}

void LocalAgentCommPerformance::addCorrectMsgTransmittedMsg() {
    this->totalTransmittedMsg++;
}

void LocalAgentCommPerformance::addCorrectMsgForwardedMsg() {
    this->totalForwardedMsg++;
}

LocalAgentCommPerformance::~LocalAgentCommPerformance() {
    // TODO Auto-generated destructor stub
}

std::string LocalAgentCommPerformance::info() {
    std::ostringstream ost;
    ost << CommPerformance::info()
        << "  Local recievedMsgTemporario: " << this->recievedMsgTemp
        << "  Local rateRecievedMSGPperiod: " << this->rateRecievedMSGPperiod
        << "  Local totalTransmittedMsg: " << this->totalTransmittedMsg
        << "  Local totalFowardedMsg: " << this->totalForwardedMsg
        << "  Local totalReceivedMsg: " << this->totalReceivedMsg;

    return ost.str();
}

std::string LocalAgentCommPerformance::infoTrace(bool header=false, std::string idagent=""){
    std::ostringstream ost;


    if (header)
       ost <<";Local recievedMsgTemporario;Local rateRecievedMSGPperiod;Local totalReceivedMsg;totalForwardedMsg;totalTransmittedMsg";
    else
       ost << ";" << this->recievedMsgTemp
           << ";" << this->rateRecievedMSGPperiod
           << ";" << this->totalReceivedMsg
           << ";" << this->totalForwardedMsg
           << ";" << this->totalTransmittedMsg;

    return ost.str();
}

