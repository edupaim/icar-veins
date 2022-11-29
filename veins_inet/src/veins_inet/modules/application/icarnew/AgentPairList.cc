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

#include <veins_inet/modules/application/icarnew/AgentPairList.h>

AgentPairList::AgentPairList() {
    // TODO Auto-generated constructor stub
    this->agentPairs.clear();
}


AgentPair* AgentPairList::getAgentPair(int64_t sourceId, int64_t destinationId){
    AgentPair *c=NULL;

    bool find=false;
    unsigned int index = 0;
    //std::cout << "OLDbuscar " << "canal origem=" << sourceId << "   destino=" << destinationId  <<std::endl;

    //if (!this->agentPairs.empty())
    {

        while (!find && this->agentPairs.size() > index)
        {

            c = dynamic_cast<AgentPair*>(agentPairs[index]);

            if (c!=NULL)
                if (c->getAgentS()->getId() == sourceId && c->getAgentD()->getId()==destinationId)
                   find=true;
                else c=NULL;

            ++index;
        }
   }
    //std::cout << "OLDXXXXbuscar " << "canal origem=" << sourceId << "   destino=" << destinationId << std::endl;

    return c;
}

AgentPair* AgentPairList::getAgentPair(int64_t sourceId, int64_t destinationId, std::vector<AgentPair*> aAgentPairs){
    AgentPair *c=NULL;

    bool find=false;
    unsigned int index = 0;
    //std::cout << "buscar canal origem=" << sourceId << "   destino=" << destinationId << std::endl;
    std::cout << "NEWbuscar " << "canal origem=" << sourceId << "   destino=" << destinationId << std::endl;

    while (!find && aAgentPairs.size()> index)
    {
        //c = dynamic_cast<Channel*>(&channels[index]);
        c = dynamic_cast<AgentPair*>(aAgentPairs[index]);

        if (c->getAgentS()->getId() == sourceId && c->getAgentD()->getId()==destinationId)
           find=true;
        else c=NULL;

        ++index;
    }
     return c;
}


int AgentPairList::getAgentPairIndex(int64_t sourceId , int64_t destinationId)
{
    AgentPair *c=NULL;
    int index = -1;
    unsigned int i = 0;

    while (index==-1 && this->agentPairs.size()> i)
    {
        //c = dynamic_cast<Channel*>(&channels[i]);
        c = dynamic_cast<AgentPair*>(agentPairs[i]);

        if (c->getAgentS()->getId() == sourceId && c->getAgentD()->getId()==destinationId)
            index = i;

        ++i;
     }

    return index;
}

std::vector<AgentPair*>  AgentPairList::listNeighbors(int64_t agentId)
{
    AgentPair *c=NULL;

    std::vector<AgentPair*> neighborList;

    bool find=false;
    unsigned int index = 0;

    while (!find && agentPairs.size()> index)
    {

        c = dynamic_cast<AgentPair*>(agentPairs[index]);

    //   if (c->getAgentD()==NULL) std::cout << index << "encontrar vizinho de " << agentId<< std::endl;
    /*
     * Verifica se o destino desejado foi a origem do canal e se � comunic�vel e vizinho
     */

     if ((c->getAgentD()->getId() == agentId || c->getAgentS()->getId() == agentId) && c->isNeighbors())
             //|| (c->getAgentS()->getId()==agentId
             //&&  c->getChannelMobility()->getDistancelr() < c->getAgentS()->getSetRadius()))
     {
               neighborList.push_back(c);

     }

        ++index;
    }

    return  neighborList;
}

/*
 * list all channels that exist agentId
 */
std::vector<AgentPair*>  AgentPairList::listAgentPairsWith(int64_t agentId)
{
    AgentPair *c=NULL;

    std::vector<AgentPair*> cList;

    bool find=false;
    unsigned int index = 0;

    while (!find && agentPairs.size()> index)
    {

        c = dynamic_cast<AgentPair*>(agentPairs[index]);


     if ((c->getAgentD()->getId() == agentId || c->getAgentS()->getId()==agentId))
     {
         cList.push_back(c);
               //std::cout << endl << "  destino: " << c->getAgentD()->getId() << "   origem: " << c->getAgentS()->getId() << endl;
     }

        ++index;
    }

    return  cList;
}

AgentPairList::~AgentPairList() {
    // TODO Auto-generated destructor stub
    delete (this);
}

/**
 * @brief
 *
 * @param
 */
double AgentPairList::calcMaxDistancelr(int64_t agent ) {
    AgentPair *c=NULL;
    double maxDistance = 0;
    unsigned int index = 0;

    while (agentPairs.size()> index)
    {

        c = dynamic_cast<AgentPair*>(agentPairs[index]);

         if (c->getCommnunicationState()!=CommState::faulty
                 && (c->getAgentD()->getId() == agent || c->getAgentS()->getId())
                 && c->getAgentPairMobility()->getDistancelr(c->getAgentS()->getMobilityInfo()->getActualMove(),c->getAgentD()->getMobilityInfo()->getActualMove())
                         > maxDistance )
         {
             maxDistance = c->getAgentPairMobility()->getDistancelr(c->getAgentS()->getMobilityInfo()->getActualMove(),c->getAgentD()->getMobilityInfo()->getActualMove());
           //std::cout << endl << "  destino: " << c->getAgentD()->getId() << "   origem: " << c->getAgentS()->getId() << endl;
         }

        ++index;
    }

    return  maxDistance;

}

/**
 * @brief
 *
 * @param
 */
double AgentPairList::calcMinDistancelr(int64_t agent) {
    AgentPair *c=NULL;
    double minDistance = 0;
    unsigned int index = 0;

    while (agentPairs.size()> index)
    {

        c = dynamic_cast<AgentPair*>(agentPairs[index]);

        if (index == 0 ) minDistance = c->getAgentPairMobility()->getDistancelr(c->getAgentS()->getMobilityInfo()->getActualMove(),c->getAgentD()->getMobilityInfo()->getActualMove());

         if (c->getCommnunicationState()!=CommState::faulty
                 && (c->getAgentD()->getId() == agent || c->getAgentS()->getId())
                 && c->getAgentPairMobility()->getDistancelr(c->getAgentS()->getMobilityInfo()->getActualMove(),c->getAgentD()->getMobilityInfo()->getActualMove()) < minDistance )
         {
             minDistance = c->getAgentPairMobility()->getDistancelr(c->getAgentS()->getMobilityInfo()->getActualMove(),c->getAgentD()->getMobilityInfo()->getActualMove());
           //std::cout << endl << "  destino: " << c->getAgentD()->getId() << "   origem: " << c->getAgentS()->getId() << endl;
         }

        ++index;
    }

    return  minDistance;
}

/**
 * @brief
 *
 * @param
 */
double AgentPairList::calcMaxSlr(int64_t agent) {
    AgentPair *c=NULL;
    double maxSlr = 0;
    unsigned int index = 0;
    double aux;

    while (agentPairs.size()> index)
    {
        c = dynamic_cast<AgentPair*>(agentPairs[index]);

        aux = fabs(c->getQoCAgentPair()->getTimelinessAgentPair()->getSlr());

         if (c->getCommnunicationState()!=CommState::faulty
                 && (c->getAgentD()->getId() == agent || c->getAgentS()->getId())
                 && aux > maxSlr )
         {
             maxSlr = aux;
           //std::cout << endl << "  destino: " << c->getAgentD()->getId() << "   origem: " << c->getAgentS()->getId() << endl;
         }

        ++index;
    }

    return  maxSlr;
}

/**
 * @brief
 *
 * @param
 */
double AgentPairList::calcMinSlr(int64_t agent) {
    AgentPair *c=NULL;
    double minSlr = -1;
    unsigned int index = 0;
    double aux;

    while (agentPairs.size()> index)
    {
        c = dynamic_cast<AgentPair*>(agentPairs[index]);

         if ((c->getAgentD()->getId() == agent || c->getAgentS()->getId())
                 && c->getCommnunicationState()!=CommState::faulty
                 && c->isNeighbors())
                 {
                 aux = fabs(c->getQoCAgentPair()->getTimelinessAgentPair()->getSlr());

                 if (minSlr == -1)
                  {
                     minSlr = aux;
                  }

                  if (aux < minSlr )
                     {
                      minSlr = aux;
                     }

                 // std::cout << endl << "  destino: " << c->getAgentD()->getId() << "   origem: " << c->getAgentS()->getId()
                //          << " min slr " << minSlr;

                 }

        ++index;
    }

    return  minSlr;
}


std::string AgentPairList::infoTraceAgentPairList(){
    AgentPair *c=NULL;
    std::ostringstream ost;

    unsigned int index = 0;
    while (this->agentPairs.size()> index)
    {
        c = dynamic_cast<AgentPair*>(agentPairs[index]);

        if (index==0) ost << c->infoTrace(true) << endl;

        ost << c->infoTrace(false) << endl;

        ++index;
    }
     return ost.str();
}

AgentPair * AgentPairList::updatePairListFromMsg(long numMsg, Agent* agentS, Agent* agentD, simtime_t pMsgReceivingTime, simtime_t msgSendingTime,
        RadiusEstimatorAgentPair * pRadiusEstimator, int neighbor, int event, simtime_t dataValidity, double pPeriodApp, double powerReceived_dbm, double snr)
{
    AgentPair * pair = getAgentPair(agentS->getId(), agentD->getId());

    if (pair==NULL){
       pair = new AgentPair(agentS, agentD, pMsgReceivingTime, msgSendingTime,  pRadiusEstimator, neighbor,
                                  0, dataValidity, pPeriodApp, powerReceived_dbm, snr);
       pair->addReceivedMsg();
       pair->setCurrentMsgNumber(numMsg);

       this->agentPairs.push_back(pair);
    }else{
        pair->setCurrentMsgNumber(numMsg);
        pair->updateAgentPair(pMsgReceivingTime, msgSendingTime, neighbor, 0, dataValidity, powerReceived_dbm, snr);
    }


    return pair;

}

