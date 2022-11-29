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

#include "AgentPair.h"

AgentPair::AgentPair(){
    // TODO Auto-generated constructor stub

}

AgentPair::~AgentPair() {
    // TODO Auto-generated destructor stub
    delete (this);
}


//creating AgentPair
AgentPair::AgentPair(Agent* agentS, Agent* agentD, simtime_t msgReceivingTime, simtime_t msgSendingTime, RadiusEstimatorAgentPair * pRadiusEstimator, int neighbor, int event, simtime_t dataValidity, double pPeriodApp, double powerReceived_dbm, double snr)
{
 this->setUpdatingTime(simTime());
 this->setAgentS(agentS);
 this->setAgentD(agentD);
 this->currentMSGNumber = 0;
 this->lastMSGNumber=0;
 this->receivedMessageAmount = 0;
 this->oPeriodApplication = pPeriodApp;
 this->penultTMsgSending = 0;
 this->lastTMsgSending = msgSendingTime;
 this->setNeighbors(neighbor);
 this->setLastTMsgReceiving(msgReceivingTime);
 this->setCurrentTMsgReceiving(msgReceivingTime);
 this->setMessageTTime((msgReceivingTime - msgSendingTime).dbl());
 this->setTxReceivedDBm(powerReceived_dbm);
 this->setSnr(snr);
 this->radiusEstimator = pRadiusEstimator;
 this->agentPairMobility = new AgentPairMobility(this);
 this->addReceivedMsg();

 if (agentS->getSetRadius()<=0)
 {
     if (powerReceived_dbm<0 && powerReceived_dbm>-200){
         this->setCalcRadius();
     }
 } else this->calcRadius = agentS->getSetRadius();


 this->setCommnunicationState(0);
 updateCommnunicationState();
 this->qoCAgentPair = new QoCAgentPair(this,dataValidity);
 //std::cout << endl << "testando "<< endl;
}


//amount received message from source agent
void AgentPair::addReceivedMsg()
{
    this->receivedMessageAmount++;
}

long AgentPair::getReceivedMessageAmount(){
    return this->receivedMessageAmount;
}



//AgentPair updating when l receives r message
void AgentPair::updateAgentPair(simtime_t msgReceivingTime, simtime_t msgSendingTime, int neighbor,
        int event,  simtime_t dataValidity, double powerReceived_dbm, double snr )
{


 if (lastTMsgSending <= msgSendingTime)
 {
     // updating communication context of AgentPair
     this->setUpdatingTime(simTime());
     this->penultTMsgSending = this->lastTMsgSending;
     this->lastTMsgSending = msgSendingTime;
     this->setLastTMsgReceiving(getCurrentTMsgReceiving());
     this->setCurrentTMsgReceiving(msgReceivingTime);
     this->setMessageTTime((msgReceivingTime - msgSendingTime).dbl());
     this->setTxReceivedDBm(powerReceived_dbm);
     this->setSnr(snr);
     this->addReceivedMsg();

      // updating mobility context of AgentPair
      this->agentPairMobility->updateAgentPairMobility();

      if (this->getAgentS()->getSetRadius()<=0)
       {
             if (powerReceived_dbm<0 && powerReceived_dbm>-200)
             {
                 this->setCalcRadius();
             }else {
                 this->calcRadius = 0;
             }
       } else this->calcRadius = this->getAgentS()->getSetRadius();


     this->setOutOfRadiusRangeState();

     // updating neighbor condition
     this->updateNeighborhoodCondition();

     // updating AgentPair communication state
     updateCommnunicationState();

     // updating QoC of AgentPair
     this->getQoCAgentPair()->getTimelinessAgentPair()->update(dataValidity);
  }
}

//AgentsPair updating general events
void AgentPair::updateAgentPair()
{
    // updating communication context of AgentPair
    this->setUpdatingTime(simTime());

     //update distance between S and D
     getAgentPairMobility()->updateAgentPairMobility();

     this->setOutOfRadiusRangeState();

     // updating QoC of AgentPair
     this->getQoCAgentPair()->getTimelinessAgentPair()->update(-1);
}


void AgentPair::updateNeighborhoodCondition(){
    if (this->agentPairMobility->getDistancelr() > this->getCalcRadius())
            this->neighbors = false;
    else this->neighbors = true;
}

//update communication state
//void AgentPair::updateCommnunicationState(int event){
void AgentPair::updateCommnunicationState(){
    if (this->neighbors==false) //&& there is no communication route
        this->setCommnunicationState(CommState::noCommunicating);
    else
        this->setCommnunicationState(CommState::communicating);


    /*int cstate = CommState::updateCommnunicationState(event, this->getCommnunicationState());
    if (cstate >= 0) this->setCommnunicationState(cstate);
    else this->setCommnunicationState(-1);
    */
}

bool AgentPair::isInAgentPair(int id)
{
    if (id == this->getAgentS()->getId() || id == this->getAgentD()->getId())
        return true;
    else
        return false;
}


Agent* AgentPair::getAgentD() {
    return agent_d;
}

void AgentPair::setAgentD( Agent* agentD) {
    agent_d = agentD;
}

Agent* AgentPair::getAgentS() {
    return agent_s;
}

void AgentPair::setAgentS(Agent* agentS) {
    agent_s = agentS;
}

double AgentPair::getCalcRadius() {
    return calcRadius;
}

void AgentPair::setUpdatingTime(simtime_t timeupdate) {
    this->updatingTime=timeupdate;
}

long AgentPair::getLastMsgNumber() {
    return lastMSGNumber;
}

void AgentPair::setLastMsgNumber(long lastMsgNumber) {
    lastMSGNumber = lastMsgNumber;
}

simtime_t AgentPair::getUpdatingTime() {
    return this->updatingTime;
}
void AgentPair::setCalcRadius() {
    //this->calcRadius = this->radiusEstimator->getSignalRange(this->txReceived_dBm,
    //        this->channelMobility->getDistancelr(this->agent_s->getMobilityInfo()->getActualMove(),
   //         this->agent_d->getMobilityInfo()->getActualMove()));
    Coord r = this->getAgentS()->getMobilityInfo()->getPositionAtTime(simTime());
    Coord l = this->getAgentD()->getMobilityInfo()->getPositionAtTime(simTime());
    drlRadius = this->getAgentPairMobility()->calcEuclideanDistancelr(this->getAgentD()->getMobilityInfo()->getActualMove()->getStartPos(), r);
    //double drlNext = this->getAgentPairMobility()->calcEuclideanDistancelr(l, r);
    //this->calcRadius = this->radiusEstimator->getSignalRange(this->txReceived_dBm,
    //        this->agentPairMobility->getDistancelr());
    this->calcRadius = this->radiusEstimator->getSignalRange(this->txReceived_dBm, drlRadius);
}

AgentPairMobility* AgentPair::getAgentPairMobility() {
    return agentPairMobility;
}

void AgentPair::setAgentPairMobility(AgentPairMobility* agentPairMobility) {

    this->agentPairMobility = agentPairMobility;
}

int AgentPair::getCommnunicationState() {

    return commnunicationState;
}

void AgentPair::setCommnunicationState(int pcommnunicationState) {
    this->commnunicationState = pcommnunicationState;
}

simtime_t AgentPair::getCurrentTMsgReceiving() {
    return currentTMsgReceiving;
}

void AgentPair::setCurrentTMsgReceiving( simtime_t correntTMsgReceiving) {
    this->currentTMsgReceiving = correntTMsgReceiving;
}

simtime_t AgentPair::getLastTMsgReceiving() {
    return lastTMsgReceiving;
}

void AgentPair::setLastTMsgReceiving(simtime_t lastTMsgReceiving) {
    this->lastTMsgReceiving = lastTMsgReceiving;
}


double AgentPair::getMessageTTime(){
    return messageTTime;
}

void AgentPair::setMessageTTime(double messageTTime) {
    this->messageTTime = messageTTime;
}

bool AgentPair::isNeighbors(){
    if (neighbors==1) return true;
    else return false;
}

void AgentPair::setNeighbors(int neighbors) {
    this->neighbors = neighbors;
}

int AgentPair::getNeighbors() {
    return neighbors;
}

double AgentPair::getMessageDelay()
{
   return (currentTMsgReceiving - lastTMsgSending).dbl();
}

std::string AgentPair::info() {
    std::ostringstream ost;
    ost << "  agente destino: " << this->agent_d->info()
        << "  agente origem : " << this->agent_s->info()
        << "  dados da mobie: " << this->agentPairMobility->info()
        << "  estado canal  : " << this->commnunicationState
        << "  ultimo ST Rec. MSG: " << this->currentTMsgReceiving
        << "  penultimo ST Rec.MSG:  " << this->lastTMsgReceiving
        << "  tempo viagem MSG  : " << this->messageTTime
        << "  vizinho : " << this->neighbors
        << "  QOC :: " <<  ((TimelinessAgentPair *) this->qoCAgentPair->getTimeliness())->info();

    return ost.str();
}

double AgentPair::getPeriodApplication() {
    return oPeriodApplication;
}

double AgentPair::getTxReceivedDBm() {
    return txReceived_dBm;
}

void AgentPair::setTxReceivedDBm(double txReceivedDBm) {
    txReceived_dBm = txReceivedDBm;
}

void AgentPair::setPeriodApplication(double periodApplication) {
    oPeriodApplication = periodApplication;
}

std::string AgentPair::infoTrace(bool header) {
    std::ostringstream ost;
     if (header){
       ost
               << ";agOID"
               << ";agDID"
               <<";neighbor"
                 ";pairState"
                 ";delayMSG"
               ";currentReceviedMSGNumber"
               ";LastReceviedMSGNumber"
               //";lostMessage"
               <<";CurrTSMsgSending"
               << ";receivedMessageAmount"
               << ";currSTRecMSG"
               << ";lastSTRecMSG"
               << ";travelTimeMSG"
               << ";receivedPowerDbm"
               << ";snr"
               << ";radiusCalc;drlRadius"
               << ";outOfRadiusRange";
               Agent * temp = new Agent();
               AgentPairMobility* temp1 = new AgentPairMobility();
               TimelinessAgentPair * temp2 = new TimelinessAgentPair();
               ost << temp1->infoTrace(true)
               << temp2->infoTrace(true)
               <<  temp->infoTrace(true, "S")
               << temp->infoTrace(true, "D")
               << this->infoTraceRoutes(true);
   } else {
       ost
           << ";" << this->agent_s->getId()
           << ";" << this->agent_d->getId()
           << ";" << this->neighbors
           << ";" << this->commnunicationState
            //<< ";" << this->getMessageDelay()
           << ";" << this->messageTTime
           << ";" << this->getCurrentMsgNumber()
           << ";" << this->getLastMsgNumber()
           //<< ";" << this->getCurrentMsgNumber() - (this->getLastMsgNumber()-1)
           << ";" << this->lastTMsgSending
           << ";" << this->receivedMessageAmount
           << ";" << this->currentTMsgReceiving
           << ";" << this->lastTMsgReceiving
           << ";" << this->messageTTime
           << ";" << this->txReceived_dBm
           << ";" << this->snr
           << ";" << this->calcRadius
           << ";" << this->drlRadius
           << ";" << this->outOfRadiusRange
           <<  this->agentPairMobility->infoTrace(false)
           <<  ((TimelinessAgentPair *) this->qoCAgentPair->getTimeliness())->infoTrace(false)
           << this->agent_s->infoTrace(false,"")
           << this->agent_d->infoTrace(false, "")
           << this->infoTraceRoutes(false);

   }
    return ost.str();
}

/*
 * @brief info trace routes of an agent pair
 */

std::string AgentPair::infoTraceRoutes(bool header){
    ICarRoute *c=NULL;
    std::ostringstream ost;
    unsigned int index = 0;
    unsigned int sizeofRoute;

    /// VER COMO FICA  A O VALOR MAXIMO DE SALTOS RELACIONANDO COM O PAR DE AGENTES
    if (header) sizeofRoute = 32;
    else sizeofRoute = pairRoutes.size();

    while (sizeofRoute > index)
    {
       if (!header)
        c = dynamic_cast<ICarRoute*>(pairRoutes[index]);

        ost << c->infoTrace(header);

        ++index;
    }
     return ost.str();
}


Agent* AgentPair::getAgentDs() {
    return agent_ds;
}

void AgentPair::setAgentDs(Agent* agentDs) {
    agent_ds = agentDs;
}

QoCAgentPair* AgentPair::getQoCAgentPair(){
    return qoCAgentPair;
}


void AgentPair::setQoCAgentPair(QoCAgentPair* qoCAgentPair) {
    this->qoCAgentPair = qoCAgentPair;
}

 cMessage * AgentPair::getAgentPairMessage() {
    return agentPairMessage;
}

double AgentPair::getOutOfRadiusRangeState() {
    return outOfRadiusRange;
}

void AgentPair::setOutOfRadiusRangeState() {
    //this->outOfRadiusRange = this->channelMobility->getDistancelr(this->agent_s->getMobilityInfo()->getActualMove(),
    //        this->agent_d->getMobilityInfo()->getActualMove()) - this->calcRadius;
    this->outOfRadiusRange = this->agentPairMobility->getDistancelr() - this->calcRadius;
}

void AgentPair::setAgentPairMessage(cMessage* agentPairMessage) {
    this->agentPairMessage = agentPairMessage;
}

simtime_t AgentPair::getLastTMsgSending() {
    return lastTMsgSending;
}

void AgentPair::setLastTMsgSending(simtime_t lastTMsgSending) {
    this->lastTMsgSending = lastTMsgSending;
}

long AgentPair::getTransmittedMsgAmount(){
    return this->transmittedMsgAmount;
}

long AgentPair::getCurrentMsgNumber() {
    return currentMSGNumber;
}

void AgentPair::setCurrentMsgNumber(long currentMsgNumber) {
    if (currentMsgNumber > this->currentMSGNumber)
    {
        this->lastMSGNumber = this->currentMSGNumber;
        this->currentMSGNumber = currentMsgNumber;
    }
}

void AgentPair::setTransmittedMsgAmount(long msgNumber){
    if (msgNumber > this->transmittedMsgAmount)
        this->transmittedMsgAmount = msgNumber;
}

double AgentPair::getSnr() {
    return snr;
}

void AgentPair::setSnr(double snr) {
    this->snr = snr;
}
