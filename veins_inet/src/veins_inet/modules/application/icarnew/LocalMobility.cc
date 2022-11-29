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

#include "LocalMobility.h"




LocalMobility::LocalMobility(){
    // TODO Auto-generated constructor stub
    this->sensorPeriod = 0;
}

LocalMobility::LocalMobility(VeinsInetMobility* traci, float pMaxSpeed) {
    this->sensorPeriod = 0;
    this->setTraci((LocalMobilityVeinsInet *)traci);
    this->setMaxSpeed(pMaxSpeed);
    //this->actualTemp = new Move();
    this->actualMove = new Move();
    //this->actualMove = this->traci->getMove();
    updateLocalMobilityPos();
}


Move* LocalMobility::getMove() {
    //this->updateLocalMobilityPos();
    return this->actualMove;
}

Move* LocalMobility::getLastMove() {
    //this->updateLocalMobilityPos();
    return this->lastMove;
}

void LocalMobility::updateLocalMobilityPos()
{
     if(this->traci!=NULL)
     {
         if (!this->traci->isTerminated())
         {
             /* to keep the last move
             this->lastMove = this->actualTemp;
             this->actualTemp = new Move();
             this->actualTemp->setStart(this->actualMove->getStartPosition(), this->actualMove->getStartTime());
             this->actualTemp->setSpeed(this->actualMove->getSpeed());

             this->actualMove = this->traci->getMove();
             double ay = actualMove->getStartPosition().y - this->lastMove->getStartPosition().y;
             double ax = actualMove->getStartPosition().x - this->lastMove->getStartPosition().x;
             this->angle = std::atan2(ay, ax);
             //this->angle = traci->getAngleRad();
             this->calcAcceleration();
             */
             //double ay = this->traci->getMove()->getStartPosition().y - this->lastMove->getStartPosition().y;
             //double ax = this->traci->getMove()->getStartPosition().x - this->lastMove->getStartPosition().x;
             //this->angle = std::atan2(ay, ax);
             //this->angle = traci->getAngleRad();

             this->lastMove = this->actualMove;
             this->actualMove = new Move();
             this->actualMove->setStart(this->traci->getMove()->getStartPos(), this->traci->getMove()->getStartTime());
             this->actualMove->setSpeed(this->traci->getMove()->getSpeed());

             //dados do sensor
             Move * om1 = new Move();
             om1->setStart( this->actualMove->getStartPos(), this->actualMove->getStartTime());
             om1->setSpeed(this->actualMove->getSpeed());
             this->setCollectedMove(om1);

             double ay = actualMove->getStartPos().y - this->lastMove->getStartPos().y;
             double ax = actualMove->getStartPos().x - this->lastMove->getStartPos().x;


             if (ay!=0 || ax!=0) {
                 this->angle = std::atan2(ay, ax);

             }

             //this->angle = traci->getAngleRad();
             this->calcAcceleration();
             this->age = this->traci->getMove()->getStartTime().dbl() - actualMove->getStartTime().dbl();
             //std::cout << endl << "eixo X " << actualMove->getStartPosition().x << "  -  " << this->lastMove->getStartPosition().x;
             //std::cout << endl << "eixo Y " << actualMove->getStartPosition().y << "  -  " << this->lastMove->getStartPosition().y;
             //std::cout << endl << "angleLocal = " << this->angle;
         }
     }

}


double LocalMobility::getSensorPeriod(){
    //this->updateLocalMobilityPos();
    return sensorPeriod;
}

void LocalMobility::setSensorPeriod(double sensorPeriod) {
    //updateLocalMobilityPos();
    this->sensorPeriod=sensorPeriod;
}

LocalMobilityVeinsInet * LocalMobility::getTraci()
{
    return (LocalMobilityVeinsInet  * ) traci;
}

LocalMobility::~LocalMobility() {
    //delete (this);
}

void LocalMobility::setTraci(LocalMobilityVeinsInet * traci) {
    this->traci = traci;
}


std::string LocalMobility::info() {

    std::ostringstream ost;
    ost << " sensor period: " << sensorPeriod << " acceleration: " << this->acceleration << " traci: " << traci->info();

    return ost.str();
}

std::string LocalMobility::moveInfoTrace(bool header=false){

       std::ostringstream ost;
       ost << ";" << MobilityInfo::moveInfoTrace(header);
       return ost.str();
}

