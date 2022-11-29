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

#ifndef SRC_VEINS_MODULES_APPLICATION_ICARNEW_RADIUSESTIMATORAGENTPAIR_H_
#define SRC_VEINS_MODULES_APPLICATION_ICARNEW_RADIUSESTIMATORAGENTPAIR_H_
#include <omnetpp.h>
#include <veins_inet/modules/application/icarnew/RadiusEstimatorAgentPair.h>
#include <iostream>
#include "veins/base/modules/BaseWorldUtility.h"
//using namespace veins;

class RadiusEstimatorAgentPair {
public:
    RadiusEstimatorAgentPair(double radiusSecurity);
    virtual ~RadiusEstimatorAgentPair();
    double getSignalRange(double transPower_dbm, double d_lr);
    double sensitivityPower_dbm;
    double drlRadius;
    double radiusSecurity;
};

#endif /* SRC_VEINS_MODULES_APPLICATION_ICARNEW_RADIUSESTIMATORAGENTPAIR_H_ */
