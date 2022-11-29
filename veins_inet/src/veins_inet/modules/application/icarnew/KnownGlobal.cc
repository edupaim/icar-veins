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

#include <veins_inet/modules/application/icarnew/KnownGlobal.h>

Define_Module(KnownGlobal);

void KnownGlobal::initialize()
{
    // TODO - Generated method body
}

void KnownGlobal::initialize(int stage) {
    if (stage == 0) {

       //Initializing members and pointers of your application goes here
       EV << "Initializing " << "Known Global " << std::endl;

       // when begin to send messages in the simulation
       this->startSimulation = par("startSimulation").doubleValue();

       // when end the sending messages during the simulation
       this->finishSimulation = par("endSimulation").doubleValue();

       // period de medicao do desempeno da comunicacao do agente CommPerformance
       this->perforMesurementPeriod = par("perforMesurementPeriod").doubleValue();

       // periodo de monitoramento -- beacon msg
       this->monitorPeriod= par("monitorPeriod").doubleValue();

       this->mobilityPeriod = par("mobilityPeriod").doubleValue();

       if (monitorPeriod!=0)
           this->maxMsgAmountVehicle = (finishSimulation - startSimulation)/ monitorPeriod;
       else this->maxMsgAmountVehicle = (finishSimulation - startSimulation)/0.5; //TEMPORARIO => ATENCAO DEVE VARIAR DE ACORDO AO INTERVALO RANDÔMICO PARA DISTRIBUIÇÃO DE PERÍODO

       //std::cout << endl<< "maxMsgAmountVehicle= " << maxMsgAmountVehicle;
       // vehicle amount

       //rate of lost message
       this->lostMessageRate=par("lostMessageRate").doubleValue();

       this->vehicleAmout = par("vehicleAmount").longValue();

       this->simulationDataheaderLine = par("simulationDataheaderLine").boolValue();

       this->setRadius = par("radiusSet").doubleValue();

       // gravar a atualização do contexto quando a mobilidade eh alterada em cada veiculo
       this->printUpdateChangedMobility = par("traceChangedMobility").boolValue();

        // period to send application messages
       this->loadPeriodApp = par("loadPeriodApp").doubleValue();

       this->oGeneralCommunicationService.appPeriodLoadMsg =   this->loadPeriodApp;

       //  code of application type
       this->oGeneralCommunicationService.appTypeId = par("appTypeId").longValue();

       // tolerated delay of application
       this->oGeneralCommunicationService.deliveryDelay = par("deliveryDelay").doubleValue();

       // application message time in the network
       this->oGeneralCommunicationService.timeToLive =  par("timeToLive").doubleValue();


       this->knownVehicles.clear();

       // the estimator of channel radius
       this->oRadiusEstimatorAgentPair = new RadiusEstimatorAgentPair(this->setRadius);

       observationNameFile = par("observation").stdstringValue();

       // current date/time based on current system
       time_t now = time(0);
       tm *ltm = localtime(&now);
       instantTimeSimulation << 1900 + ltm->tm_year << "."
               << 1 + ltm->tm_mon << "." << ltm->tm_mday << "." << ltm->tm_hour << "."
               << ltm->tm_min<< "_";
       // creating trace files
       this->createGlobalTraces(-1);

    }
}

stringstream KnownGlobal::getFilePreFix(int idVehicle){
    stringstream sufixo;
    /// =====

    stringstream idVehicleStr;

    //sufixo << par("filesDirectory").stdstringValue() << instantTimeSimulation.str()
    sufixo << par("filesDirectory").stdstringValue()
            << idVehicleStr.str()
            << par("observation").stdstringValue();

    if (idVehicle >=0) sufixo << "V" << idVehicle;
    sufixo  << "Amount" << this->vehicleAmout
            << "lostRate" << this->lostMessageRate
            << "AppLoad" << loadPeriodApp
            << "MobPeriod" << mobilityPeriod
            << "beaconLoad" << monitorPeriod
            << "radius" << this->setRadius;

    return sufixo;
}

void KnownGlobal::createGlobalTraces(int idVehicle){
    // trace message file
    stringstream nameFileMessage;

    string aux=getFilePreFix(idVehicle).str();

    nameFileMessage << aux << "Messages.csv";
    this->fileMessages.open(nameFileMessage.str(), fstream::in | fstream::out | fstream::app);
    //std::cout << nameFileMessage.str() << endl;

    nameFileMessage.str("");
    nameFileMessage << aux << "PairAgents.csv";
    this->fileChannels.open(nameFileMessage.str(), fstream::in | fstream::out | fstream::app);

    nameFileMessage.str("");
    nameFileMessage << aux << "LocalAgents.csv";
    this->fileLocalAgents.open (nameFileMessage.str(), fstream::in | fstream::out | fstream::app);
    //std::cout << nameFileMessage.str() << endl;

    nameFileMessage.str("");
    nameFileMessage  << aux << "RemoteAgents.csv";
    this->fileRemoteAgents.open (nameFileMessage.str(), fstream::in | fstream::out | fstream::app);
    //std::cout << nameFileMessage.str() << endl;

   nameFileMessage.str("");
   nameFileMessage  << aux << "CommPerformance.csv";
   this->fileCommPerformance.open(nameFileMessage.str(), fstream::in | fstream::out | fstream::app);

   //std::cout << nameFileMessage.str() << endl;

   nameFileMessage.str("");
   nameFileMessage  << aux << "traceMinslr.csv";
   this->fileChannelsMinslr.open(nameFileMessage.str(), fstream::in | fstream::out | fstream::app);

   //std::cout << nameFileMessage.str() << endl;

   nameFileMessage.str("");
  nameFileMessage  << aux << "ReceivedMessages.csv";
  this->fileReceivedMessages.open(nameFileMessage.str(), fstream::in | fstream::out | fstream::app);
  //std::cout << nameFileMessage.str() << endl;

}
/*
 *
 */
double KnownGlobal::calcTraciDistanceMobility(int64_t vehicle1id, int64_t vehicle2id )
{

    double calDistance=0;

    LocalAgent * agent1= this->knownVehicles.at(vehicle1id);
    LocalAgent * agent2= this->knownVehicles.at(vehicle2id);

    if (agent1!=NULL &&  agent2!=NULL)
       /* if (agent1->getLocalMobility()->getMove()!=NULL && agent2->getLocalMobility()->getMove()!=NULL)
        {
            calDistance = agent1->getMobilityInfo()->calcEuclideanDistancelr(
                    agent1->getLocalMobility()->getMove()->getStartPos(),
                    agent2->getLocalMobility()->getMove()->getStartPos());
        }
*/

   if (agent1->getLocalMobility()->getTraci()->getMove()!=NULL && agent2->getLocalMobility()->getTraci()->getMove()!=NULL)
          {
              calDistance = agent1->getMobilityInfo()->calcEuclideanDistancelr(
                      agent1->getLocalMobility()->getTraci()->getMove()->getStartPos(),
                      agent2->getLocalMobility()->getTraci()->getMove()->getStartPos());
          }


    return calDistance;
}

Move * KnownGlobal::getTraciMobility(int64_t vehicle1id)
{

    Move * agentTraciPosition=NULL;

    LocalAgent * agent1= this->knownVehicles.at(vehicle1id);

    if (agent1!=NULL)
        if (agent1->getLocalMobility()->getTraci()->getMove()!=NULL)
        {
            agentTraciPosition = agent1->getLocalMobility()->getTraci()->getMove();
        }

    return agentTraciPosition;
}



int KnownGlobal::getIntNamefromIdAgent(int agentId)
{
    bool find;
    LocalAgent * la;
    unsigned int i=0;
    while (i < this->knownVehicles.size() && !find )
    {
       la= knownVehicles.at(i);

       if (la->getId()==agentId) find = true;
       else i++;
    }

    if (find) return i;
    else return -1;
}

std::string KnownGlobal::getStringNamefromIdAgent(int agentId)
{
    bool find;
    LocalAgent * la;
    unsigned int i=0;
    while (i < this->knownVehicles.size() && !find )
    {
       la= knownVehicles.at(i);

       if (la->getId()==agentId) find = true;
       else i++;
    }

    if (find) return la->getAgentName();
    else return "";
}


void KnownGlobal::finish()
{
    // TODO - Generated method body
    this->fileChannels.close();
    this->fileMessages.close();
    this->fileLocalAgents.close();
    this->fileRemoteAgents.close();
    this->fileChannelsMinslr.close();
    this->fileCommPerformance.close();
}

void KnownGlobal::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

