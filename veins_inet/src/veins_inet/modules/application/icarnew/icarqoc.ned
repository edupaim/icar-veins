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

package org.car2x.veins.subprojects.veins_inet.modules.application.icarnew;
import org.car2x.veins.base.modules.IBaseApplLayer;
//
// TODO auto-generated module

//
// @author Margarete Sa
//
// @see Icarqoc
// @see Nic80211p
// @see Mac1609_4
// @see PhyLayer80211p
//
simple Icarqoc //like IBaseApplLayer
{
    parameters:
        @class(Icarqoc);         
        @display("p=60,50");
        string appName = default("Icarqoc!");
        string routingTableModule = default("^.routingTable");
        string interfaceTableModule = default("^.interfaceTable");
        string networkProtocolModule = default("^.networkLayer.ip");
        double maxJitter @unit("s") = default(5ms);
        volatile double jitter @unit("s") = default(uniform(0ms, maxJitter));
        
        int headerLength = default(88bit) @unit(bit); //header length of the application
        bool integratedScenario = default(false);
        double radiusSet = default(250); // valor baseado no artigo XXXXXXXXX meters 
        double maxSpeedRoute0= default(24.4);// em m/s 88 km/h máximo da via + 10% tolerado em lei municipal
        double maxSpeedRoute1= default(24.4);// em m/s 88 km/h máximo da via + 10% tolerado em lei municipal      

        gates:
//        input lowerLayerIn; // from mac layer
//        output lowerLayerOut; // to mac layer
//        input lowerControlIn;
//        output lowerControlOut;
        
        input icarqIn;
        input ipIn;
        output icarqOut;
        output ipOut;    
}
