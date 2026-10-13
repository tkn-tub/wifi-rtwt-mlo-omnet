
package rtwt_mlo.simulations;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.common.IntegratedVisualizer;
import inet.visualizer.canvas.integrated.IntegratedMultiCanvasVisualizer;
import inet.common.lifecycle.NodeStatus;
import inet.common.scenario.ScenarioManager;

import rtwt_mlo.node.RtwtStation;
import rtwt_mlo.node.RtwtAccessPoint;

network MLONetwork
{
    parameters:
        @display("bgb=761,421");

        @statistic[throughput](source=liveThroughput(host2.app[0].packetReceived)/1000000; record=figure; targetFigure=panel.throughput; checkSignals=false);
        @statistic[numRcvdPk](source=count(host2.app[0].packetReceived); record=figure; targetFigure=panel.numRcvdPkCounter; checkSignals=false);
        @figure[panel](type=panel; pos=1.5,0.1);
        //    @figure[panel.throughput](type=gauge; pos=0,0; size=100,100; minValue=0; maxValue=40; tickSize=5; label="App level throughput [Mbps]");
        @figure[panel.throughput](type=linearGauge; pos=250,50; size=250,30; minValue=0; maxValue=54; tickSize=6; label="Application level throughput [Mbps]");
        @figure[panel.numRcvdPkCounter](type=counter; pos=50,50; label="Packets received"; decimalPlaces=6);
    submodules:
        visualizer: IntegratedMultiCanvasVisualizer {
            parameters:
                @display("p=50,36");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=50,100");
        }
        // separate medium for the 5 GHz link: links on different bands do not interact,
        // and the medium's same-start-time check must not fire across bands
        radioMedium5GHz: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=50,230");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=50,167");
        }
        scenarioManager: ScenarioManager {
            parameters:
                @display("p=50,300");
        }
        host1: RtwtStation {
            parameters:
                @display("p=288,177");
        }
        host2: RtwtStation {
            parameters:
                @display("p=367,228");
        }
        host3: RtwtStation {
            parameters:
                @display("p=445,228");
        }
        host4: RtwtStation {
            parameters:
                @display("p=519,167");
        }
        accessPoint: RtwtAccessPoint {
            parameters:
                @display("p=400,200");
        }
}


