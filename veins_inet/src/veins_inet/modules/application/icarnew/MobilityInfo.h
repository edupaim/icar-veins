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

#ifndef MOBILITYINFO_H_
#define MOBILITYINFO_H_

#include "veins/base/utils/Coord.h"
#include "veins/base/utils/Move.h"

//using namespace veins;
//using veins::Move;
//using veins::Coord;
//class Coord;
#include "veins/base/utils/FWMath.h"

class MobilityInfo {


private:

protected:
    /** @brief current vehicle movement **/
    Move * actualMove= new Move();

    /** @brief last vehicle movement before current position **/
    Move * lastMove = new Move();

    /** @brief collected vehicle movement **/
    Move * collectedMove= new Move();

     // The max car speed
     float maxSpeed=0;

     //sensor Period to collect coordinate in local agent
     float sensorPeriod;

     //aceleration
     float acceleration;

     //angle of vehicle moving
     float angle;

     //temporario - AGE TIMEaCTUAL - TIME OF DATA RECEIVING
     double age;

     //modify the last move
     void setLastMove(Move * pMove);

     void calcAcceleration();




public:
    MobilityInfo();
    virtual ~MobilityInfo();
    std::string info();
    MobilityInfo(Move * mSrc);

    virtual Move * getLastMove();
    /** @brief Returns a string with the value of the coordinate. */
    float getMaxSpeed();

    void setMaxSpeed(float maxSpeed);

    virtual double calcEuclideanDistanceAgentMoving();

    double calcEuclideanDistancelr(Coord l, Coord r);

    virtual std::string moveInfoTrace(bool header);

    std::string infoTrace(bool header, std::string idagent);

    float getAcceleration();

    void setAcceleration(float acceleration);


    Move* getActualMove();

    void setActualMove(Move* actualMove);

    double getSensorPeriod();

    void setSensorPeriod(float sensorPeriod);

    int getQuadrant(double angleRad);
    //virtual void  updateMobilityInfo(simtime_t tsCorrent, double x, double y, double speed, double maxSpeed);
    float getAngle();

    Coord getPositionAtTime(simtime_t pTime);

    void setAngle(float angle);
    double getAge();

    void setAge(double age);
    const Move* getCollectedMove();
    void setCollectedMove(Move* collectedMove);
};

#endif /* MOBILITYINFO_H_ */
