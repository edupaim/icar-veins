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

#include "TimelinessAgentPair.h"

TimelinessAgentPair::TimelinessAgentPair(AgentPair *c, simtime_t dataValidity) {
    // TODO Auto-generated constructor stub
    this->priorAge=0;
    priorValidityST = 0;
    priorvalidity = 0;
    priorVTmin = 0;
    priorVTminST=0;
    priorAge=0;
    priorcoConfianca = 0;
    priortimeliness = 0;
    validity = 0;
    ValidityST = 0;
    VTmin=0;
    coConfianca=0;
    coConfedenceTimeliness=0;
    timeliness = 0;

    this->c = c;
    update(dataValidity);
}


TimelinessAgentPair::~TimelinessAgentPair() {
    // TODO Auto-generated destructor stub
    delete(this);
}



double TimelinessAgentPair::calcValidityTeste(simtime_t dataValidity){

    priorValidityST = ValidityST;
    priorvalidity = validity;
    priorVTmin = VTmin;
    priorVTminST = VTminST;
    priorcoConfianca = coConfianca;
    priortimeliness = timeliness;

    alfa =0;
    Srl = 0;
    slrmenos=0;
    slrmais=0;
    vRX =0;
    vRY =0;
    vR = 0;
    validity = 0;
    ValidityST = 0;
    VTTeste = 0;
    VTmin=0;
    coConfianca=0;
    coConfedenceTimeliness=0;
    timeliness = 0;
    timeStampAgentPair = c->getUpdatingTime();
    SrlMin = 0;

//    if (c->isNeighbors()
//             && c->getCommnunicationState()!=CommState::faulty
//             && c->getTxReceivedDBm()<0
//             && (c->getCalcRadius() - this->c->getAgentPairMobility()->getDistancelr())>=0)
//      {
//
//
//
//            angler = c->getAgentS()->getMobilityInfo()->getAngle();
//            anglel = c->getAgentD()->getMobilityInfo()->getAngle();
//            radiusR = c->getCalcRadius();
//
//            //Calc vehicles distance
//            drl = this->c->getAgentPairMobility()->getDistancelr();
//            tetarl = this->c->getAgentPairMobility()->getDistancelrAngleRad(); //rl Distance angle
//
//
//            vRX =this->c->getAgentPairMobility()->getRelativeSpeedX();
//            vRY =this->c->getAgentPairMobility()->getRelativeSpeedY();
//            vR = this->c->getAgentPairMobility()->getRelativeSpeed();
//            tetarv =this->c->getAgentPairMobility()->getRelativeSpeedAngle(); //Relative Velocity angle
//
//            //Max Relative Speed
//            this->maxRelativeSpeed = c->getAgentS()->getMobilityInfo()->getMaxSpeed()
//                    + c->getAgentD()->getMobilityInfo()->getMaxSpeed();
//
//            //srl calc - using movement direction applying
//            alfa = tetarl - tetarv;
//
//            Srl = calcLeicosseno(alfa);
//
//            //SrlMin = (radiusR - drl);
//            //if (SrlMin < 0) SrlMin =0;
//
//            SrlMin = std::abs(slrmais) > std::abs(slrmenos)? std::abs(slrmenos) : std::abs(slrmais);
//
//            double vrvt;
//            if (vR <0.001) vrvt=0.001;
//            else vrvt = vR;
//
//            validity = (Srl/vrvt);
//            if (validity<age) ValidityST = 0;
//            else ValidityST = validity - age;
//
//            //conservative validity time -- max Relative Speed
//            if (maxRelativeSpeed!=0)
//            {
//                //conservative validity time Minimum Validity Time -- considering only vehicles approaching
//                this->VTmin = SrlMin/maxRelativeSpeed;
//                if (VTmin < age) VTminST=0;
//                else VTminST=VTmin - age;
//            }
//
//            if (validity!=0)
//            {
//                if (age<validity) timeliness = 1-(this->age/validity);
//
//                if (VTmin<=age && age<validity)
//                {
//                    this->coConfianca = (1-(age - VTmin)/(validity - VTmin));
//                }
//                else
//                    if (VTmin > age)
//                    {
//                        this->coConfianca=1;
//                    }
//            }
//        }
    }


double TimelinessAgentPair::calcLeicosseno(double angle)
{
    double final = std::sqrt((drl*drl)*(std::cos(angle)*std::cos(angle))-(drl*drl)+(radiusR*radiusR));
    slrmais= drl*-1.0 *std::cos(angle) + final;
    slrmenos=drl*-1.0 *std::cos(angle) - final;

    double vSrl = 0;

    //if (slrmais<0)
    //    vSrl = slrmenos;
    //else
    //    if (slrmenos<0)
    //        vSrl = slrmais;

   // if (vSrl <0 )
   //     vSrl = 0;
   // std::cout << endl << c->getAgentD()->getId()<< "  mais = " << slrmais << "    slrmenos = " << slrmenos << "    Vslr ="
    //        << vSrl << std::endl;
    alphaQuadrant=calcAngleQuadrant(alfa);
    drlQuadrant=calcAngleQuadrant(tetarl);
    vrlQuadrant=calcAngleQuadrant(tetarv);
    Squadrant=calcAngleQuadrant(angler);
    Dquadrant=calcAngleQuadrant(anglel);


    if (this->Dquadrant==this->vrlQuadrant){
        vSrl = std::abs(slrmenos);
        if (this->c->getAgentS()->getMobilityInfo()->getActualMove()->getSpeed() >
            this->c->getAgentD()->getMobilityInfo()->getActualMove()->getSpeed())
            vSrl = std::abs(slrmais);
    }else
        vSrl = std::abs(slrmais);

    return vSrl;
}

double TimelinessAgentPair::calcValidity(simtime_t dataValidity){
    calcValidityTeste(dataValidity);
     return (-1);
}




void TimelinessAgentPair::update(simtime_t dataValidity){

    this->calcAge();
    if (c->isNeighbors()) this->calcValidity(dataValidity);
    else {
        ICarRoute * icR;
        for (int index=0; index < c->pairRoutes.size();index++){
            icR = c->pairRoutes.at(index);
            icR->update();
            if (icR->qocValidityTime > c->getQoCAgentPair()->getTimelinessAgentPair()->timeoutValidity )
            {
                c->getQoCAgentPair()->getTimelinessAgentPair()->timeoutValidity = icR->qocValidityTime;
            }
        }
    }
}

void TimelinessAgentPair::calcTimeoutValidity()
{
    if (this->getValidity() > 0)
        this->timeoutValidity = this->timeStampUpdate + this->getValidity() + c->getMessageTTime();
    else  this->timeoutValidity = 0;
}

double TimelinessAgentPair::calcAngleQuadrant(double oAngle)
{
    double anglec=oAngle;

    if (oAngle<0) anglec = 2*M_PI +oAngle;

    anglec = floor(anglec * 1000)/1000;

    //cout << endl << "  floor " << floor(anglec * 100)/100 << "  << " << anglec;

   //if (oAngle<=0.09 && oAngle>-0.09) anglec=0;

    double angleQuadrant = ceil(anglec/(M_PI/2));
    return angleQuadrant;
}

double TimelinessAgentPair::calcAge()
{

    //ATENCAO -- TEMPORARIAMENTE ASSIM
   // double i = this->c->getAgentS()->getMobilityInfo()->getActualMove()->getStartTime().dbl();
   // double j = this->c->getAgentD()->getMobilityInfo()->getActualMove()->getStartTime().dbl();
    double i = this->c->getAgentS()->getMobilityInfo()->getCollectedMove()->getStartTime().dbl();
    double j = this->c->getAgentD()->getMobilityInfo()->getCollectedMove()->getStartTime().dbl();
    double k = this->c->getLastTMsgSending().dbl();

    this->priorAge = this->ageAgentPair;
    double vNow = simTime().dbl();
    double menor =i;
    if (i<j && i<k) menor = i;
    if (j<i && j<k) menor = j;
    if (k<j && k<i) menor = k;
    this->ageAgentPair = vNow - menor;

    if (i<j) menor = i;
    else menor = j;
    this->ageMobility = vNow - menor;

    this->ageLastMsgReceiving = vNow - k;

    this->age = this->ageAgentPair;
    return this->ageAgentPair;
}

std::string TimelinessAgentPair::info(){
    std::ostringstream ost;
    ost << "  TIMELINESS: "
        << "  VALIDITY - " << this->validity
        << "  AGE - " << this->calcAge();
    return ost.str();

}
std::string TimelinessAgentPair::infoTrace(bool header){

    std::ostringstream ost;
    if (header)
        ost <<  ";timeStampAgentPair;"
                "alfa;"
                "alfaGrau;"
                "Srl;srlmais;srlmenos;SrlMin;priorVTinT;VTinT;priorVT;VT;"
                "priorVTmin;VTmin;priorVTminT;VTminT;"
                "AnteriorAgePair;ageAgentPair;"
                "priorcoConfianca;coConfianca;"
                "priortimeliness;timeliness;alpha;quadranteDrl;quadranteVrl;quadranteS;quadranteD";
    else{
        ost
        <<";" << this->timeStampAgentPair
        <<";" << alfa
        <<";" << alfa*180/M_PI
        <<";" << Srl
        <<";" << slrmais
        <<";" << slrmenos
        <<";" << SrlMin
        <<";" << priorValidityST
        <<";" << ValidityST
        <<";" << priorvalidity
        <<";" << validity
        <<";" << priorVTmin
        <<";" << VTmin
        <<";" << priorVTminST
        <<";" << VTminST
        <<";" << priorAge
        <<";" << ageAgentPair
        <<";" << priorcoConfianca
        <<";" << coConfianca
        <<";" << priortimeliness
        <<";" << timeliness
        <<";" << alphaQuadrant
        <<";" << drlQuadrant
        <<";" << vrlQuadrant
        <<";" << Squadrant
        <<";" << Dquadrant;
    }
    return ost.str();
}

TimelinessAgentPair::TimelinessAgentPair() {
}

double TimelinessAgentPair::getSlr() {
    return this->Srl;
}

void TimelinessAgentPair::setSlr(double Srl) {
    this->Srl = Srl;
}

