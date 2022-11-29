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

#include <veins_inet/modules/application/icarnew/CommPerformance.h>

CommPerformance::CommPerformance() {
    // TODO Auto-generated constructor stub

}

double CommPerformance::getMeasurementPeriod(){
    return measurementPeriod;
}

void CommPerformance::setMeasurementPeriod(double measurementPeriod) {
    this->measurementPeriod = measurementPeriod;
}

CommPerformance::~CommPerformance() {
    // TODO Auto-generated destructor stub
}

std::string CommPerformance::info() {
    std::ostringstream ost;
    ost << "  Measurement Period: " << this->getMeasurementPeriod();
    return ost.str();
}

std::string CommPerformance::infoTrace(bool header=false){
    std::ostringstream ost;
    if (header)
        ost << ";Measurement Period";
    else
        ost <<";" << this->measurementPeriod;
    return ost.str();
}
