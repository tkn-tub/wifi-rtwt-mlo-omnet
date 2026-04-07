# R-TWT MLO — Restricted Target Wake Time for Wi-Fi 7 Multi-Link Operation

An OMNeT++ simulation framework implementing **Restricted Target Wake Time (R-TWT)** for **IEEE 802.11be (Wi-Fi 7)** with **Multi-Link Operation (MLO)**. 

---

## Dependencies

| Dependency | Version |
|---|---|
| [OMNeT++](https://omnetpp.org/) | **6.0.3** |
| [INET Framework](https://inet.omnetpp.org/) | **4.5** |
| [wifi-mlo-omnet](https://github.com/tkn-tub/wifi-mlo-omnet) | (see below) |

### MLO Base Framework

This project **requires** the MLO base framework as a linked OMNeT++ project:

> https://github.com/tkn-tub/wifi-mlo-omnet

The `RtwtStation`, `RtwtAccessPoint`, `RtwtUMac`, and `RtwtUMacAccessPoint` modules all extend base classes (`MLOStation`, `MLOAccessPoint`, `UMac`) defined in that framework. Both projects must reside in the same OMNeT++ workspace and the MLO project must be set as a referenced project in the build settings.

## Building

This project is built using the **OMNeT++ IDE** (Eclipse-based):

1. Place this project folder, `inet4.5/`, and `wifi-mlo-omnet/` side by side in your OMNeT++ workspace.
2. Import all three projects into the workspace.
3. Set project Makemake properties for this project:
   - Root: **build** mode (makemake), source location
   - `simulations/` and `src/`: **No makefile**, C++ Source Folder
4. Add `wifi-mlo-omnet` as a referenced project under **Project > Properties > Project References**.
5. Build via **Project > Build** in the OMNeT++ IDE.

You can safely ignore the linker warnings. Some of the extended modules have the exact same functions (e.g., overwritten but copied) to avoid segmentation faults and these warnings mainly stem from such duplicates. 

---

## Architecture

### Protocol Stack

The R-TWT-enabled STAs and APs are defined as `RtwtStation` and `RtwtAccessPoint`. The MLO operation is assumed to be over two links. They have the following protocol stack and modules:

```
Application (UdpBasicApp / UdpSink)
        ↕
    RtwtUMac  (Multi-Link aggregation — src/umac/)
        ↕              ↕
   lmac[0]         lmac[1]        (per-link IEEE 802.11be — src/lmac/)
   2.4 GHz          5 GHz
```

### Modules

#### U-MAC (`src/umac/`)

- **`RtwtUMac`** — U-MAC for R-TWT-enabled STAs. Extends the base `UMac` from the MLO framework. STAs advertise their link requirements (configured in `omnetpp.ini`) to the AP. SLO mode is also supported.
- **`RtwtUMacAccessPoint`** — U-MAC for R-TWT-enabled AP. Extends `RtwtUMac`. Provides a special per-STA queueing facility that gates transmissions to the wake-time windows of each destination STA. Drives one of the three available schedulers.

#### R-TWT Schedulers (`src/rtwt/scheduler/`)

All schedulers inherit from **`TwtSchedulerBase`**, which collects link requirements from all STAs, applies max-min fairness to compute the required number of time slots (slot unit = 256 µs), and produces TWT schedule results. The subclasses differ in how they distribute service periods (SPs) across the two links:

| Scheduler | Strategy |
|---|---|
| `SymmetricalScheduler` | Splits the SP equally across both links (link aggregation style — parallel TX/RX). |
| `AsymmetricalScheduler` | Splits the SP proportionally to the data rate of each link; also enables simultaneous TX and RX. |
| `CrossSymmetricalScheduler` | Splits the SP equally but assigns TX to one link and RX to the other (simultaneous TX/RX without link aggregation). Note: this is not STR in the MLO sense — the TX/RX occur at the sender and receiver MLDs respectively. All MLO operations in this implementation are STR-capable. |

#### R-TWT Agent (`src/rtwt/agent/`)

- **`TwtAgent`** — Runs inside each L-MAC instance. Coordinates radio sleep/wake operations under the control of U-MAC. Maintains TWT agreements and schedules self-messages for SP start/end transitions.

#### L-MAC (`src/lmac/`)

Per-link IEEE 802.11be interface, extending `Ieee80211LMac` from the MLO base framework:

- **`Ieee80211LMacRtwt`** — L-MAC with R-TWT support: integrates `TwtAgent` and uses the R-TWT-aware HCF. Note: this module is largely a copy of `Ieee80211LMac` from the MLO framework because the base interface definitions cannot be extended with additional gates without refactoring the MLO framework (identified as future work).
- **`LHcfRtwt`** — Extended HCF that coordinates channel access within SPs. Talks to `TwtAgent` and pulls packets from U-MAC according to receiver STA wake times. Uses `LEdcaRtwt`.
- **`LEdcaRtwt`** / **`LEdcafRtwt`** — Extended EDCA/EDCAF with SP-aware ACK handling. Structurally identical to the MLO base versions except for the EDCAF module and ACK policy, which could not be configured flexibly through the base interface.
- **`OriginatorQosTwtAckPolicy`** / **`QosTwtAckHandler`** — Extended ACK management: tracks block ACKs per destination STA and sends them at the end of each STA's SP.
- **`Ieee80211LMgmtApRtwt`** — AP management module that ensures beacons are sent at the start of each STA's SP (so all STAs receive beacons while awake).
- **`Ieee80211ahTwtRadio`** — TWT-aware radio that executes periodic sleep/wake cycles as commanded by `TwtAgent`.

#### Nodes (`src/node/`)

- **`RtwtStation`** — R-TWT-enabled STA. Extends `MLOStation` from the MLO framework with `Ieee80211LMacRtwt` and `RtwtUMac`.
- **`RtwtAccessPoint`** — R-TWT-enabled AP. Extends `MLOAccessPoint` from the MLO framework with `Ieee80211LMacRtwt`, `RtwtUMacAccessPoint`, and a scheduler submodule.

#### Message Types (`src/rtwt/msg/`)

Custom OMNeT++ message definitions and their serializers:

- **`twtFrame`** — TWT setup request/response frames exchanged between STAs and AP.
- **`twtLinkRequirement`** — STA→AP advertisement of link requirements.
- **`twtScheduleBeacon`** — AP→STA broadcast of the computed TWT schedule.

---

## Key Parameters

| Parameter | Location | Description |
|---|---|---|
| `umac.typename` | per-node | `RtwtUMac` (STA) or `RtwtUMacAccessPoint` (AP) |
| `twtScheduler.typename` | AP umac | `SymmetricalScheduler`, `AsymmetricalScheduler`, or `CrossSymmetricalScheduler` |
| `twtScheduler.cycleDuration` | AP umac | TWT cycle length |
| `twtScheduler.practicalBitrate` | AP umac | Effective bitrate used for slot count calculation |
| `umac.linkRequirements` | STA umac | JSON-style array of flow requirements: `[{flowId, restriction, dataRate, latency, wakeTime, receiver}]` |
| `umac.sloLink` | umac | Force single-link operation on link 0 or 1 |
| `twtEnable` | lmac mac | Enable R-TWT at the MAC level |

---

## Evalution

Under the `result-processing` folder, you can find two Jupiter notebooks to (1) process the OMNeT++ results files (.sca and .vec) and (2) create plots for the selected metrics. You can run them in the given order. (2) also has some validation experiments, which illustrates the packet transmission timeline for the consequent SPs. 

## Citation

If you use this framework in your research, please cite our paper:

```bibtex
@inproceedings{ergenc2026multi-link,
  author = {Ergen{\c{c}}, Do{\u{g}}analp and Zakrzewski, Mateusz and Dressler, Falko},
  title = {{Multi-Link Scheduling with Restricted Target Wake Time in Wi-Fi 7}},
  publisher = {IEEE},
  address = {Crans-Montana, Switzerland},
  booktitle = {21th IEEE/IFIP Wireless On-demand Network systems and Services Conference (WONS 2026)},
  month = {3},
  year = {2026},
}
```

## License

This project is licensed under the **GNU Lesser General Public License v3.0 (LGPL-3.0)**. See [LICENSE](LICENSE) for details.
