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

package org.car2x.veins.subprojects.veins_inet.nodes;

import inet.node.inet.WirelessHost;
import inet.applications.udpapp.UDPBasicApp;
import inet.transportlayer.udp.UDP;
import org.car2x.veins.subprojects.veins_inet.modules.application.icarnew.Icarqoc;

//
// TODO auto-generated type
//
module ICARRouter extends WirelessHost
{
    parameters:
        @display("i=device/cellphone");
        int numIcarApps = default(0);
        forwarding = true;
        wlan[*].mgmtType = default("Ieee80211MgmtAdhoc");
    submodules:
        myUdpApp[numIcarApps]: UDPBasicApp {}
        myUdp: UDP {}
        icar: Icarqoc {}
    connections:
        for i=0..numIcarApps-1 {
            myUdpApp[i].udpOut --> myUdp.appIn++;
            myUdpApp[i].udpIn <-- myUdp.appOut++;
        }
        
        myUdp.ipOut --> icar.icarqIn;
        myUdp.ipIn <-- icar.icarqOut;

        icar.ipOut --> networkLayer.transportIn++;
        icar.ipIn <-- networkLayer.transportOut++;
}
