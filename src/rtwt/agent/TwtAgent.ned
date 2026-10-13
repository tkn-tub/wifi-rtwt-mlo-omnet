//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
//
// TWT agent coordinates radio sleep/wake oeprations under the control of U-MAC.
//

package rtwt_mlo.rtwt.agent;

simple TwtAgent
{
    parameters:
        @display("i=block/cogwheel");
        @class(inet::ieee80211::TwtAgent);
        bool isStation = default(true);
        int scheduleBuffer = default(0);
    gates:
        output radioOut;
        
        input macIn;
        output macOut;

        input llcIn;
        output llcOut;       
}
