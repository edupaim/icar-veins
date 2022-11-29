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

#ifndef AGENTPAIRMOBILITY_H_
#define AGENTPAIRMOBILITY_H_

#include <omnetpp.h>
#include <veins_inet/modules/application/icarnew/AgentPair.h>
#include "MobilityInfo.h"

class AgentPair;

class AgentPairMobility : public MobilityInfo
{
private:

    /**
     * @brief relative speed between l and r
     */
    AgentPair * c;
    /**
     * @brief relative speed between l and r
     */
    double relativeSpeed;

    /**
     * @brief X relative speed between l and r
     */
    double relativeSpeedX;

    /**
     * @brief Y relative speed between l and r
     */
    double relativeSpeedY;

    /**
     * @brief relative speed angle between l and r
     */
    double relativeSpeedAngleRad;

    /**
     * @brief
     */
    double distancelrAngleRad;

    /*
     *@brief actual distance between l and r agents
     */
    double distancelr;

    /*
     *@brief actual X distance between l and r agents
     */
    double distancelrX;

    /*
     *@brief actual y distance between l and r agents
     */
    double distancelrY;

    /*
     *@brief actual z distance between l and r agents
     */
    double distancelrZ;
    /*
      *@brief last distance between l and r agents
      */
    double lastDistancelr;


    /*
     *@brief time stamp of movement
     */


    double maxRelativeSpeed;
    //simtime_t movementCurrentTime;

    double maxAngleRelativeSpeed;
public:
    /**
       * @brief
       * @param
       * @return
       */
    AgentPairMobility();
    /**
       * @brief
       * @param
       * @return
       */
    virtual ~AgentPairMobility();
    /**
       * @brief
       * @param
       * @return
       */
    AgentPairMobility(AgentPair * c);

    /**
       * @brief
       * @param
       * @return  the string with all attributes data  of this object
       */

    std::string info();

    /**
       * @brief
       * @param
       * @return
       */
    void calcRelativeSpeed();
    /**
       * @brief
       * @param
       * @return
       */
    std::string infoTrace(bool header);

    /**
       * @brief
       * @param
       * @return
       */
    void updateAgentPairMobility();

    /**
       * @brief
       * @param
       * @return
       */
    double getDistancelr(Move * l, Move * r);

    /**
       * @brief TEMPORARIO velocidade calculo da referencia vref e do movel v
       * @param
       * @return
       */

    double getDistancelr();

    void calcDistance();

    double calcEuclideanDistancelr(Coord l, Coord r);
    /**
        * @brief
        * @param
        * @return
        */
    float getLastDistancelr();

    double getRelativeSpeedX();
    double getRelativeSpeedY();
    double getRelativeSpeed();
    double getRelativeSpeedAngle();

    void setRelativeSpeed(double relativeSpeed);

    AgentPair* getC();
    void setC(AgentPair* c);
    double getDistancelrAngleRad();
    void setDistancelrAngleRad(double distancelrAngle);
    simtime_t getLasDistanceTime();
    void setLasDistanceTime(simtime_t lasDistanceTime);
    void setLastDistancelr(double lastDistancelr);
    double getRelativeSpeedAngleRad();
    void setRelativeSpeedAngleRad(double relativeSpeedAngle);
    void setRelativeSpeedX(double relativeSpeedX);
    void setRelativeSpeedY(double relativeSpeedY);
    double getDistancelrX();
    double getDistancelrY();
    double getMaxRelativeSpeed();
    void setMaxRelativeSpeed(double maxRelativeSpeed);
    double getMaxAngleRelativeSpeed();
    void setMaxAngleRelativeSpeed(double maxAngleRelativeSpeed);
};

#endif /* AGENTPAIRMOBILITY_H_ */
