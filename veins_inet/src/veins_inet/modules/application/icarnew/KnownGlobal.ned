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

//
// TODO auto-generated module
//
simple KnownGlobal
{
        int vehicleAmount= default(0); 
        double startSimulation = default(0);
        double endSimulation = default(0);   
        //double simulationDuration= default(610s) @unit(s);  
        string observation  = default("");
        //simulation files directory;  
        string filesDirectory = default("");
 
        // if use the simulation data with header line
        bool simulationDataheaderLine = default(true);   
       
        // gravar a atualização do contexto quando a mobilidade eh alterada em cada veiculo
        bool traceChangedMobility= default(true); 

		// percentual de perda de mensagens por veículo
		double lostMessageRate=default(0); 
		
        double mobilityPeriod=default(1);
                        
		// General Communication Service of icarqoc
		int appTypeId = default(0); // este valor foi adotado para codificar o tipo de aplicacao
		double loadPeriodApp = default(0);
        double deliveryDelay = default(0); //atraso maximo tolerado pela aplicacao
		double timeToLive = default(0s) @unit(s); // tempo de vida da mensagem na rede 
		double perforMesurementPeriod = default(1); // Período de medição do desempeno da comunicação do agente CommPerformance
        double monitorPeriod = default(1); // Período de monitoração beacon msg 
        double radiusSet = default(0); // set radius  - declared radius in meters       
		     
}
