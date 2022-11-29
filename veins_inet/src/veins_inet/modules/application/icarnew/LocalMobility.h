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

#ifndef LOCALMOBILITY_H_
#define LOCALMOBILITY_H_
#include "MobilityInfo.h"
#include "QoCInfo.h"
//#include <veins_inet/modules/application/icarnew/LocalMobilityTraci.h>
#include <veins_inet/modules/application/icarnew/LocalMobilityVeinsInet.h>
//#include <veins_inet/VeinsInetMobility.h>

//using Veins::TraCIMobility;
using Veins::VeinsInetMobility;

class LocalMobility : public MobilityInfo  {
private:
    // Get vehicle coordinates from VeinsMobilityService
    LocalMobilityVeinsInet* traci;

    //variable to support the previous movement of the vehicle (lastMove)
    Move * actualTemp;

    //
public:
    LocalMobility();
    LocalMobility(VeinsInetMobility* traci, simtime_t tsStart, double x, double y, double sensorPeriod, double speed);
    LocalMobility(VeinsInetMobility* traci, float pMaxSpeed);
    virtual ~LocalMobility();
    Move * getMove();
    Move * getLastMove();
    double getSensorPeriod();
    void setSensorPeriod(double sensorPeriod);
    std::string info();
    LocalMobilityVeinsInet * getTraci();
    void setTraci(LocalMobilityVeinsInet * traci);
    void updateLocalMobilityPos();
    std::string moveInfoTrace(bool header);

};

#endif /* LOCALMOBILITY_H_ */
