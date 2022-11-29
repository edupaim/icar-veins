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

#ifndef TIMELINESSCHANNEL_H_
#define TIMELINESSCHANNEL_H_

//#ifndef DBG
//#define DBG EV
//#endif
#define DBG std::cerr << "[" << simTime().raw() << "] " << getParentModule()->getFullPath() << " "
#include <omnetpp.h>
#include "Timeliness.h"
#include <veins_inet/modules/application/icarnew/AgentPair.h>
#include "math.h"
#include <iostream>
using namespace omnetpp;

class AgentPair;

class TimelinessAgentPair : public Timeliness {
private:

    AgentPair * c;
    double ageLastMsgReceiving;
    double ageAgentPair;
    double ageMobility;
    double SrlMin;
    void calcTimeoutValidity();
    double Srl;
    double slrmenos;
    double slrmais;
    double alfa;
    double maxRelativeSpeed=0;
    double timeliness;
    Coord sNext;
    Coord dNext;
    double VTmin;
    double VTminST;
    double VTTeste;
    double priorVTminST;
    double priorvalidity;
    double priorVTmin;
    double priorcoConfianca;
    double priortimeliness;
    double priorValidityST;
    double ValidityST;
    double coConfedenceTimeliness;
    double rounding(double src, int precision);

    double radiusR;
    double drl;
    double angler;

    double anglel;
    double tetarl;
    double tetarv;
    double vxr;
    double vyr;
    double vxl;
    double vyl;
    Coord r, l;
    double vRX;
    double vRY;
    double vR;
    simtime_t timeStampAgentPair;
    double alphaQuadrant;
    double drlQuadrant;
    double vrlQuadrant;
    double Squadrant;
    double Dquadrant;

public:
    double calcAngleQuadrant(double angleRad);
    double calcLeicosseno(double angle);
    TimelinessAgentPair();
    TimelinessAgentPair(AgentPair* c, simtime_t dataValidity);
    std::string infoTrace(bool header);
    virtual ~TimelinessAgentPair();
    double calcValidity(simtime_t dataValidity);
    double calcAge();
    void update(simtime_t dataValidity);
    std::string  info();
    double getSlr();
    void setSlr(double slr);
    double calcValidityTeste(simtime_t dataValidity);
    double getAgeCommState();
    void setAgeCommState(double ageCommState);
    double getAgeMobilityCondition();
    void setAgeMobilityCondition(double ageMobilityCondition);
    double getAgeNeighberCondition();
    void setAgeNeighberCondition(double ageNeighberCondition);
    simtime_t getTimeoutValidityCommState();
    void setTimeoutValidityCommState(simtime_t timeoutValidityCommState);
    simtime_t getTimeoutValidityMobilityCondition();
    void setTimeoutValidityMobilityCondition(
            simtime_t timeoutValidityMobilityCondition);
    simtime_t getTimeoutValidityNeighberCondition();
    void setTimeoutValidityNeighberCondition(
            simtime_t timeoutValidityNeighberCondition);
    simtime_t getTimeStampUpdateCommState();
    void setTimeStampUpdateCommState(simtime_t timeStampUpdateCommState);
    simtime_t getTimeStampUpdateMobilityCondition();
    void setTimeStampUpdateMobilityCondition(
            simtime_t timeStampUpdateMobilityCondition);
    simtime_t getTimeStampUpdateNeighberCondition();
    void setTimeStampUpdateNeighberCondition(
            simtime_t timeStampUpdateNeighberCondition);
    double getValidityCommState();
    void setValidityCommState(double validityCommState);
    double getValidityMobilityCondition();
    void setValidityMobilityCondition(double validityMobilityCondition);
    double getValidityNeighberCondition();
    void setValidityNeighberCondition(double validityNeighberCondition);
    double getRelativeSpeedX();
    double getRelativeSpeedY();
    double getRelativeSpeed();
    double getRelativeSpeedAngle();

};

#endif /* TIMELINESSAgentPair_H_ */
