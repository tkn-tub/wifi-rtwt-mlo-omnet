//
//  Created on: April 2025
//      Author: Mateusz Zakrzewski, Doganalp Ergenc (TU Berlin)
//
// This is mostly a copy of Ieee80211LInterface, which is the L-MAC interface in the base MLO framework. 
// We have it because it is simply not possible to configure it with the new modules with additional gates that goes beyond the base interface definitions (i.e., for MAC and management modules).
// The solution will be re-factoring MLO framework as the basis with all interface definitions with necessary gates - it is a future work.
//

package rtwt_mlo.lmac;

import inet.networklayer.common.NetworkInterface;
import inet.linklayer.contract.IWirelessInterface;
import inet.linklayer.ieee80211.mib.Ieee80211Mib;
import inet.linklayer.ieee80211.mgmt.IIeee80211Mgmt;
import inet.linklayer.ieee80211.IIeee80211Mac;
import inet.physicallayer.wireless.common.contract.packetlevel.IRadio;

import inet.linklayer.ieee80211.mgmt.IIeee80211Agent;
import inet.linklayer.common.IIeee8021dQosClassifier;
import inet.linklayer.ieee80211.mgmt.Ieee80211MgmtSta;
import inet.linklayer.ieee80211.mgmt.Ieee80211AgentSta;
import inet.networklayer.common.InterfaceTable;
import inet.linklayer.contract.IWirelessInterface;
import inet.linklayer.ieee80211.llc.IIeee80211Llc;

import mlo.lmac.mgmt.IIeee80211LMgmt;
import rtwt_mlo.rtwt.agent.TwtAgent;
import rtwt_mlo.lmac.mac.Ieee80211LMacRtwt;
import rtwt_mlo.lmac.radio.Ieee80211ahTwtRadio;
import rtwt_mlo.lmac.llc.Ieee80211LLlcLpdRtwt;

module Ieee80211LInterfaceRtwt extends NetworkInterface like IWirelessInterface
{
    parameters:
		string interfaceTableModule;
		
        string opMode @enum("a","b","g(erp)","g(mixed)","n(mixed-2.4Ghz)","p","ac") = default("ac");
        string address @mutable = default("auto"); // MAC address as hex string (12 hex digits), or
                                                   // "auto". "auto" values will be replaced by
                                                   // a generated MAC address in init stage 0.
        string protocol = default("ieee80211be"); // ieee80211be
        double bitrate @unit(bps) = default(-1bps);
        
        **.opMode = this.opMode;
        **.bitrate = this.bitrate;
        mac.modeSet = default(this.opMode);
        mac.*.rateSelection.dataFrameBitrate = default(this.bitrate);
        *.macModule = default(absPath(".mac"));
        *.mibModule = default(absPath(".mib"));
		*.interfaceTableModule = default(absPath(this.interfaceTableModule));
		
    gates:
        input upperLayerIn;
        output upperLayerOut;
        
        input radioIn @labels(IWirelessSignal);
    submodules:
        mib: Ieee80211Mib {
            @display("p=125,742");
        }
        agent: <default("Ieee80211AgentSta")> like IIeee80211Agent if typename != "" {
            @display("p=481,300");
        }

        mgmt: <default("Ieee80211LMgmtSta")> like IIeee80211LMgmt  {
            @display("p=389,300");
        }
        
        llc: Ieee80211LLlcLpdRtwt {
            parameters:
                @display("p=300,200");
        }
        mac: Ieee80211LMacRtwt {
            parameters:
                @display("p=300,300");
        }
		radio: Ieee80211ahTwtRadio {
		    parameters:
		        @display("p=300,400");
		}
        twtAgent: TwtAgent {
        	parameters:
        	    @display("p=200,300");
        }
    connections:
        radioIn --> { @display("m=s"); } --> radio.radioIn;
        radio.upperLayerIn <-- mac.lowerLayerOut;
        radio.upperLayerOut --> mac.lowerLayerIn;

        mgmt.agentOut --> agent.mgmtIn if exists(agent);
        mgmt.agentIn <-- agent.mgmtOut if exists(agent);

        mac.mgmtIn <-- mgmt.macOut;
        mac.mgmtOut --> mgmt.macIn;
 
        llc.upperLayerOut --> { @display("m=n"); } --> upperLayerOut;
        llc.upperLayerIn <-- { @display("m=n"); } <-- upperLayerIn;
        
        llc.lowerLayerOut --> mac.upperLayerIn;
        llc.lowerLayerIn <-- mac.upperLayerOut;
        // MODIFICATION BEGIN: New gates are connected.
        mac.twtIn <-- twtAgent.macOut;
        mac.twtOut --> twtAgent.macIn;
        
        llc.twtAgentOut --> twtAgent.llcIn;
        twtAgent.llcOut --> llc.twtAgentIn;
        
        twtAgent.radioOut --> radio.twtAgentIn;
        
        mgmt.llcOut --> llc.mgmtIn;
        // MODIFICATION END
}
