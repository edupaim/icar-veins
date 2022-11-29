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

#include "MobilityInfo.h"

MobilityInfo::MobilityInfo() {
    // TODO Auto-generated constructor stub
    //this->lastTime = 0.0;
    //this->maxSpeed = 0;
    //this->lastMove = new Move();
    //this->actualMove = new Move();

}

MobilityInfo::MobilityInfo(Move * mSrc) {
    // TODO Auto-generated constructor stub
    //this->lastTime = 0.0;
    this->lastMove = new Move();
    this->maxSpeed = 0;
    this->actualMove = mSrc;
}

Move * MobilityInfo::getLastMove(){
    return this->lastMove;
}


void MobilityInfo::setLastMove(Move * pMove){
    this->lastMove = pMove;
}

MobilityInfo::~MobilityInfo() {
    // TODO Auto-generated destructor stub
    //delete this;
}


float MobilityInfo::getAcceleration() {
    return acceleration;
}

void MobilityInfo::setAcceleration(float acceleration) {
    this->acceleration = acceleration;
}

Move* MobilityInfo::getActualMove() {
    return actualMove;
}

void MobilityInfo::setActualMove(Move* pActualMove) {
    this->lastMove =   this->actualMove;
    this->actualMove = pActualMove;
}

double MobilityInfo::getSensorPeriod() {
    return sensorPeriod;
}

void MobilityInfo::setSensorPeriod(float sensorPeriod) {
    this->sensorPeriod = sensorPeriod;
}


float MobilityInfo::MobilityInfo::getMaxSpeed(){
    return maxSpeed;
}


void MobilityInfo::setMaxSpeed(float maxSpeed) {
    this->maxSpeed = maxSpeed;
}


double MobilityInfo::calcEuclideanDistanceAgentMoving() {
    //std::cout << "calcEuclideanDistancelr  - lastPosition=" <<this->getLastPosition() <<  "    -- startPos=" << this->startPos << std::endl;

    return  calcEuclideanDistancelr(this->lastMove->getStartPos(), this->actualMove->getStartPos());

}

double MobilityInfo::calcEuclideanDistancelr(Coord l, Coord r){
    return sqrt((r.x - l.x)*(r.x - l.x) + (r.y - l.y)*(r.y - l.y));
}

//ESTÁ FUNCIONANDO, MAS PRECISA MELHORAR ESTA FUNÇÃO PARA RETORNAR VALORES ENTRE 1 E 4
int MobilityInfo::getQuadrant(double oAngle)
{
    double anglec=oAngle;

    if (oAngle<0) anglec = 2*M_PI +oAngle;

    anglec = floor(anglec * 1000)/1000;

    int angleQuadrant = ceil(anglec/(M_PI/2));
    return angleQuadrant;
}

std::string MobilityInfo::info(){
    std::ostringstream ost;
    ost << "MobilityInfo"
        << ";maxSpeed;" << this->maxSpeed
        << ";last Move; " << lastMove->info()
        << ";actual Move; " << actualMove->info();
;
    return ost.str();
}


/*
std::string MobilityInfo::infoTrace(bool header=false){
    std::ostringstream ost;
    if (header)
        ost << ";maxSpeed"
        << moveInfoTrace(true);
    else
        ost
            << ";" << this->maxSpeed
            << moveInfoTrace(false);

    return ost.str();
}
*/


/**
 * @brief Returns information about the current state.
 */
    std::string MobilityInfo::moveInfoTrace(bool header=false) {
    }

/**
 * @brief Returns information about the current state.
 */
    std::string MobilityInfo::infoTrace(bool header=false, std::string idagent ="") {
        std::ostringstream ost;

        if (header)
            ost
                << ";ageCurrentPos" << idagent
                << ";maxSpeed" << idagent
                << ";currUpdatePosTime" << idagent
                << ";acceleration" << idagent
                << ";angleRad" << idagent
                << ";currentPosx" << idagent
                << ";currentPosy" << idagent
                << ";currentPosz" << idagent
                << ";speed" << idagent
                << ";lastUpdatePosTime" << idagent
                     << ";lastPosx" << idagent
                     << ";lastPosy" << idagent
                     << ";lastPosz" << idagent
                     << ";lastSpeed" << idagent;
                //       << ";nextTime" << idagent
                //        << ";nextPosx" << idagent
                //        << ";nextPosy" << idagent
                //        << ";nextPosz" << idagent;

        else
        {
            ost
                << ";" << this->age
                << ";" << this->maxSpeed
                << ";" << this->actualMove->getStartTime()
                << ";" << this->acceleration
                << ";" << this->angle
                << ";" << this->actualMove->getStartPos().x
                << ";" << this->actualMove->getStartPos().y
                << ";" << this->actualMove->getStartPos().z
                << ";" << this->actualMove->getSpeed()

                << ";" << this->lastMove->getStartTime()
                << ";" << this->lastMove->getStartPos().x
                << ";" << this->lastMove->getStartPos().y
                << ";" << this->lastMove->getStartPos().z
                << ";" << this->lastMove->getSpeed();

                //simtime_t nextTime = simTime();
                //Coord c = getPositionAtTime(nextTime);
                //ost << ";" << nextTime<< ";" << c.x << ";" << c.y << ";" << c.z;
        }

        return ost.str();
}

float MobilityInfo::getAngle(){
    return angle;
}

void MobilityInfo::setAngle(float angle) {
    this->angle = angle;
}

Coord MobilityInfo::getPositionAtTime(simtime_t pTime)
{
    double dTime = pTime.dbl() - this->actualMove->getStartTime().dbl();

    //std::cout << endl << "diff time = " << dTime;
    if (dTime > 0)
    {
        double x = this->actualMove->getStartPos().x + this->actualMove->getSpeed()*cos(this->angle)*dTime +
                (this->acceleration * dTime* dTime)/2;

        double y = this->actualMove->getStartPos().y + this->actualMove->getSpeed()*sin(this->angle)*dTime +
                (this->acceleration * dTime* dTime)/2;

        //std::cout << endl << "aceleracao = " << this->acceleration << "   x=" << this->actualMove->getStartPosition().x << "   x.n=" << x <<  "    y=" << this->actualMove->getStartPosition().y << "   y.n=" << y;
        return Coord(x,y,this->actualMove->getStartPos().z);
    }else
        return (this->actualMove->getStartPos());

}


void MobilityInfo::calcAcceleration()
{

    if (this->actualMove->getSpeed()> -1 && this->lastMove->getSpeed()> -1)
    {

        this->sensorPeriod = (this->actualMove->getStartTime().dbl() - this->lastMove->getStartTime().dbl());
        //std::cout << endl << "mobility sensor delta T = " << sensorPeriod  << endl;

       if (sensorPeriod != 0)
           this->acceleration = (this->actualMove->getSpeed()- this->lastMove->getSpeed()) / sensorPeriod;
    }
    //std::cout << endl << "sensorPeriod = " << sensorPeriod;
}



double MobilityInfo::getAge(){
    return age;
}

void MobilityInfo::setAge(double age) {
    this->age = age;
}

const Move* MobilityInfo::getCollectedMove() {
    return collectedMove;
}

void MobilityInfo::setCollectedMove(Move* collectedMove) {
    this->collectedMove = collectedMove;
}
