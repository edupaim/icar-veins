//
// Copyright (C) 2006-2017 Christoph Sommer <sommer@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

package org.car2x.veins.subprojects.veins_inet.example;

import org.car2x.veins.subprojects.veins_inet.modules.application.icarnew.KnownGlobal;


network Network_AODV extends Scenario
{
    submodules:
        node[0]: Car_AODV {
            @display("p=146,122");
        }
}

network Network_GPSR extends Scenario
{
    submodules:
        node[0]: Car_GPSR {
            @display("p=146,122");
        }
}

network Network_ICAR extends Scenario
{
    submodules:
        oracle: KnownGlobal {
            parameters:
                @display("p=241,138");
        }
        node[0]: Car_ICAR{
            @display("p=146,122");
        }
}
