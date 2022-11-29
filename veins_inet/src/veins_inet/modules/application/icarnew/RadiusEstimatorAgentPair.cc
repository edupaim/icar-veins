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

#include <veins_inet/modules/application/icarnew/RadiusEstimatorAgentPair.h>

RadiusEstimatorAgentPair::RadiusEstimatorAgentPair(double radiusSecurity) {
    // TODO Auto-generated constructor stub
    this->radiusSecurity = radiusSecurity;
}

RadiusEstimatorAgentPair::~RadiusEstimatorAgentPair() {
    // TODO Auto-generated destructor stub
}

double RadiusEstimatorAgentPair::getSignalRange(double transPower_dbm, double d_lr) {
 /*
        double wavelength = BaseWorldUtility::speedOfLight() / 5.89e9;
        //double transPower_dmw = std::pow(10, (this->sensitivityPower_dbm/10));
        double transPower_dmw = 0.02;
        double denominador = (16.0 * M_PI * M_PI)*(d_lr*d_lr);
        double numerador = transPower_dmw * (wavelength * wavelength);
        double pr = numerador / denominador; // potencia de recepção encontrada
        double x = transPower_dmw / pr;
        double raizx = std::sqrt(x);
        double d = (d_lr / raizx);
        std::cout << endl<< " raio = " << d << "   potencia= " << pr;

    return 250;
*/
 //---------- calculo com a potência dada

       double transPower_domw = std::pow(10, (transPower_dbm/10));
       double transPower_dmw = std::pow(10, (this->sensitivityPower_dbm/10));
        //std::cout << endl << " teste aqui" << endl;
        double x = transPower_dmw / transPower_domw;
        double raizx = std::sqrt(x);
        //std::cout << endl << "transPower_dbm=  " << transPower_dbm <<  "   sensitivityPower_dbm=" << sensitivityPower_dbm;
        //std::cout << endl << "x= " << transPower_dmw << "/" << transPower_domw << "  square root=" << raizx << "   dist=" << d << " dist=" << d_lr << "/" << raizx;

        double d = (d_lr / raizx);
        //std::cout << endl << "drl= " << d_lr << "   radius= " << d;
        if (radiusSecurity>0){
            if (d<=radiusSecurity) return d;
            else return radiusSecurity;
        } return d;

 //---------


        //std::cout << endl << "transPower_dbm=  " << transPower_dbm <<  "   sensitivityPower_dbm=" << sensitivityPower_dbm;
        //std::cout << endl << "x= " << transPower_dmw << "/" << transPower_domw << "  square root=" << raizx << "   dist=" << d << " dist=" << d_lr << "/" << raizx;


        //return d;
        //------------------------------------------------------------
        // the part of the attenuation only depending on the distance

       /* double transPower_domw = pow(10, ((transPower_dbm-30)/10));
        double lambida = BaseWorldUtility::speedOfLight()/5890500000;
        double x = lambida *lambida;

        double raizx = sqrt(x/((16* M_PI * M_PI)* transPower_domw));
        //std::cout << endl << "transPower_dbm=  " << transPower_dbm <<  "   sensitivityPower_dbm=" << sensitivityPower_dbm;
        //std::cout << endl << "x= " << transPower_dmw << "/" << transPower_domw << "  square root=" << raizx << "   dist=" << d << " dist=" << d_lr << "/" << raizx;
        std::cout << endl << "raizx= " << raizx << endl;

        return raizx;
        */
}
