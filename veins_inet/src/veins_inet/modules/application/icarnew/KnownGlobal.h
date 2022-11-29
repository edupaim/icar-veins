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

#ifndef __VEINS_APPVEHICULARFORMATION_H_
#define __VEINS_APPVEHICULARFORMATION_H_
#include <omnetpp.h>
#include <veins_inet/modules/application/icarnew/GeneralCommunicationService.h>
#include "messages/ICMessage_m.h"
#include <vector>
#include "Agent.h"
#include "RemoteAgent.h"
#include "LocalAgent.h"
#include "RemoteAgentGroup.h"
#include "messages/ICMessage_m.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "Routing.h"
#include <time.h>
#include <veins_inet/modules/application/icarnew/AgentPair.h>
#include <veins_inet/modules/application/icarnew/AgentPairList.h>
#include <veins_inet/modules/application/icarnew/RadiusEstimatorAgentPair.h>
#include <ctime>
using namespace omnetpp;


/**
 * TODO - Generated class
 */
class KnownGlobal : public cSimpleModule
{
  public:

   //"OutOfRadius - updating pair from Received Message"
   //updating pair from Received Message
   //"OutOfRadius - including pair from Received Message";
   // "including pair from Received Message";
   //OutOfRadius - Tracing Changed Mobility"
   //"updateTraceChangedMobility"

   static const int OutOfRadiusUpdatingReceivedMsg=0;
   static const int updatingPairFromReceivedMsg=1;
   static const int OutOfRadiusIncludingReceivedMsg=2;
   static const int includingPairFromReceivedMessage=3;
   static const int OutOfRadiusTracingChangedMobility=4;
   static const int updateTraceChangedMobility=5;

   bool printUpdateChangedMobility;
   std::vector <LocalAgent *> knownVehicles;
   // ManagerFile * mf;
   double measureDistance=0;
   fstream fileMessages;
   fstream fileChannels;
   fstream fileLocalAgents;
   fstream fileRemoteAgents;
   fstream fileReceivedMessages;
   fstream fileChannelsMinslr;
   fstream fileCommPerformance;
   stringstream instantTimeSimulation;
   RadiusEstimatorAgentPair * oRadiusEstimatorAgentPair;
   GeneralCommunicationService oGeneralCommunicationService;
   //double **distMatrix;
   int startSimulation; //time stamp to begin the message transmitting
   int finishSimulation; //time stamp to begin the message transmitting
   int maxMsgAmountVehicle;
   double loadPeriodApp; //application period load
   long vehicleAmout; //vehicle amount in the simulation
   double antennaSensitivity; // noise sensitivity of the antenna in dbm
   void calcTraceChangedMobility(std::string changedVehicle);
   Move * getTraciMobility(int64_t vehicle1id);
   double calcTraciDistanceMobility(int64_t agent1id, int64_t vehicle2id);
   double perforMesurementPeriod; // period de medicao do desempeno da comunicacao do agente CommPerformance
   double monitorPeriod; // periodo de monitoracao beacon msg
   double mobilityPeriod;
   double setRadius;
   double lostMessageRate; // taxa de perda de mensagem
   int getIntNamefromIdAgent(int agentId);
   bool simulationDataheaderLine;
   std::string getStringNamefromIdAgent(int agentId);
   std::string observationNameFile;
   void createGlobalTraces(int idVehicle);
   stringstream getFilePreFix(int idVehicle);
  protected:
    virtual void initialize(int stage);
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

#endif
