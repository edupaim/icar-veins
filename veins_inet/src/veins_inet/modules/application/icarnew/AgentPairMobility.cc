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

#include "AgentPairMobility.h"

AgentPairMobility::AgentPairMobility() {
    // TODO Auto-generated constructor stub

}

AgentPairMobility::~AgentPairMobility() {
    // TODO Auto-generated destructor stub
}

std::string AgentPairMobility::info() {
    std::ostringstream ost;
    ost << "  l r agents distance: " << this->distancelr
        << "  l r relative speed : " << this->relativeSpeed;
     //   << "  l r agents movement time: " << this->movementCurrentTime;
    return ost.str();
}

std::string AgentPairMobility::infoTrace(bool header)
{
    std::ostringstream ost;
    if (header)
       ost << ";drl;drlAngleRad;drlX;drlY;rSpeed_rl;rSpeedrlAngle;rSpeedXrl;rSpeedYrl";
    else
    {
        ost << ";" << this->distancelr
            << ";" << this->distancelrAngleRad
            << ";" << this->distancelrX
            << ";" << this->distancelrY
            << ";" << this->relativeSpeed
            << ";" << this->relativeSpeedAngleRad
            << ";" << this->relativeSpeedX
            << ";" << this->relativeSpeedY;
    }
    return ost.str();
}



AgentPairMobility::AgentPairMobility(AgentPair * c)
{
    this->c = c;
    this->calcDistance();
    this->lastDistancelr = this->distancelr;
    this->calcRelativeSpeed();
}

void AgentPairMobility::updateAgentPairMobility()
{
    this->lastDistancelr = this->distancelr;
    this->calcDistance();
    this->calcRelativeSpeed();
}

void AgentPairMobility::calcRelativeSpeed(){
    double angleS = c->getAgentS()->getMobilityInfo()->getAngle();
    double angleD = c->getAgentD()->getMobilityInfo()->getAngle();

    double vxS=c->getAgentS()->getMobilityInfo()->getActualMove()->getSpeed()* std::cos(angleS);
    double vyS=c->getAgentS()->getMobilityInfo()->getActualMove()->getSpeed()* std::sin(angleS);
    double vxD=c->getAgentD()->getMobilityInfo()->getActualMove()->getSpeed()* std::cos(angleD);
    double vyD=c->getAgentD()->getMobilityInfo()->getActualMove()->getSpeed()* std::sin(angleD);
    relativeSpeedX =vxS-vxD;
    relativeSpeedY =vyS-vyD;
    relativeSpeed = std::sqrt((relativeSpeedX*relativeSpeedX) + (relativeSpeedY*relativeSpeedY));
    relativeSpeedAngleRad = std::atan2(relativeSpeedY, relativeSpeedX); //Relative Velocity angle

    //Max Relative Speed
    this->maxRelativeSpeed = c->getAgentS()->getMobilityInfo()->getMaxSpeed()
            + c->getAgentD()->getMobilityInfo()->getMaxSpeed();

    double vxSMax=c->getAgentS()->getMobilityInfo()->getMaxSpeed() * std::cos(angleS);
    double vySMax=c->getAgentS()->getMobilityInfo()->getMaxSpeed() * std::sin(angleS);
    double vxDMax=c->getAgentD()->getMobilityInfo()->getMaxSpeed() * std::cos(angleD);
    double vyDMax=c->getAgentD()->getMobilityInfo()->getMaxSpeed() * std::sin(angleD);
    double relativeSpeedXMax =vxSMax-vxDMax;
    double relativeSpeedYMax =vxSMax- vyDMax;
    this->maxAngleRelativeSpeed  = std::sqrt((relativeSpeedXMax*relativeSpeedXMax) + (relativeSpeedYMax*relativeSpeedYMax));
}

void AgentPairMobility::calcDistance(){
    //Calc vehicles distance
    Coord s = c->getAgentS()->getMobilityInfo()->getActualMove()->getStartPos();
    Coord d = c->getAgentD()->getMobilityInfo()->getActualMove()->getStartPos();

    this->distancelrX = (s.x - d.x);
    this->distancelrY = (s.y - d.y);
    this->distancelrZ = (s.z - d.z);
    //this->distancelrZ = 0;
    this->distancelr = std::sqrt((this->distancelrX*this->distancelrX) + (this->distancelrY*this->distancelrY));
    this->distancelrAngleRad = std::atan2(this->distancelrY,this->distancelrX); //rl Distance angle
}

double AgentPairMobility::getRelativeSpeedX(){
    return relativeSpeedX;
}

AgentPair* AgentPairMobility::getC(){
    return c;
}

void AgentPairMobility::setC(AgentPair* c) {
    this->c = c;
}


double AgentPairMobility::getDistancelrAngleRad() {
    return distancelrAngleRad;
}

void AgentPairMobility::setDistancelrAngleRad(double distancelrAngle) {
    this->distancelrAngleRad = distancelrAngle;
}


void AgentPairMobility::setLastDistancelr(double lastDistancelr) {
    this->lastDistancelr = lastDistancelr;
}

void AgentPairMobility::setRelativeSpeed(double relativeSpeed) {
    this->relativeSpeed = relativeSpeed;
}

double AgentPairMobility::getRelativeSpeedAngleRad(){
    return relativeSpeedAngleRad;
}

void AgentPairMobility::setRelativeSpeedAngleRad(double relativeSpeedAngle) {
    this->relativeSpeedAngleRad = relativeSpeedAngle;
}

void AgentPairMobility::setRelativeSpeedX(double relativeSpeedX) {
    this->relativeSpeedX = relativeSpeedX;
}

double AgentPairMobility::getDistancelrX(){
    return distancelrX;
}

double AgentPairMobility::getDistancelrY(){
    return distancelrY;
}

double AgentPairMobility::getMaxRelativeSpeed(){
    return maxRelativeSpeed;
}

void AgentPairMobility::setMaxRelativeSpeed(double maxRelativeSpeed) {
    this->maxRelativeSpeed = maxRelativeSpeed;
}

void AgentPairMobility::setRelativeSpeedY(double relativeSpeedY) {
    this->relativeSpeedY = relativeSpeedY;
}

double AgentPairMobility::getRelativeSpeedY(){
    return relativeSpeedY;
}

double AgentPairMobility::getRelativeSpeed(){
    return relativeSpeed;
}

double AgentPairMobility::getRelativeSpeedAngle(){
    return relativeSpeedAngleRad;
}


float AgentPairMobility::getLastDistancelr(){
    return lastDistancelr;
}

double AgentPairMobility::getDistancelr(){
    return distancelr;
}

double AgentPairMobility::getDistancelr(Move * l, Move * r){
    return this->calcEuclideanDistancelr(l->getStartPos(), r->getStartPos());
}

double AgentPairMobility::calcEuclideanDistancelr(Coord l, Coord r){
    return std::sqrt((r.x - l.x)*(r.x - l.x) + (r.y - l.y)*(r.y - l.y));
}

double AgentPairMobility::getMaxAngleRelativeSpeed() {
    return maxAngleRelativeSpeed;
}

void AgentPairMobility::setMaxAngleRelativeSpeed(double maxAngleRelativeSpeed) {
    this->maxAngleRelativeSpeed = maxAngleRelativeSpeed;
}
