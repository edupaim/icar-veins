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

#include "QoCInfo.h"

QoCInfo::QoCInfo() {
    // TODO Auto-generated constructor stub
    this->age =0;

}


Timeliness* QoCInfo::getTimeliness(){
    return timeliness;
}

void QoCInfo::setTimeliness(Timeliness* timeliness) {
    this->timeliness = timeliness;
}

QoCInfo::~QoCInfo() {
    // TODO Auto-generated destructor stub
    delete(this);

}

double QoCInfo::getAge(){
    return age;
}

void QoCInfo::setAge(double age) {
    this->age = age;
}
