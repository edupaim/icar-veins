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

#ifndef SRC_VEINS_MODULES_APPLICATION_ICARNEW_LOCALAGENTCOMMPERFORMANCE_H_
#define SRC_VEINS_MODULES_APPLICATION_ICARNEW_LOCALAGENTCOMMPERFORMANCE_H_

#include "CommPerformance.h"

class LocalAgentCommPerformance : public CommPerformance {
private:
    unsigned long recievedMsgTemp;
    unsigned long rateRecievedMSGPperiod;
    unsigned long totalForwardedMsg;
    unsigned long totalReceivedMsg;

    unsigned long totalTransmittedMsg;

public:
    LocalAgentCommPerformance();
    virtual ~LocalAgentCommPerformance();
    std::string info();
    std::string infoTrace(bool header, std::string idagent);
    long getTotalReceivedMsg();
    void addCorrectMsgRecievedMsg();
    void addCorrectMsgTransmittedMsg();
    void addCorrectMsgForwardedMsg();
    void calcRateRecievedMsgPperiod();
    long getRecievedMsgTemp();
     long getRateRecievedMsgPperiod();
     long getTotalTransmittedMsg();
     long gettotalForwardedMsg();

};

#endif /* SRC_VEINS_MODULES_APPLICATION_ICARNEW_LOCALAGENTCOMMPERFORMANCE_H_ */
