
#ifdef NS3_MODULE_COMPILATION
# error "Do not include ns3 module aggregator headers from other modules; these are meant only for end user scripts."
#endif

#ifndef NS3_MODULE_WIFI
    

// Module headers:
#include "aarf-wifi-manager.h"
#include "aarfcd-wifi-manager.h"
#include "adhoc-wifi-mac-jason.h"
#include "adhoc-wifi-mac.h"
#include "amrr-wifi-manager.h"
#include "amsdu-subframe-header.h"
#include "ap-wifi-mac.h"
#include "arf-wifi-manager.h"
#include "athstats-helper.h"
#include "block-ack-agreement.h"
#include "block-ack-cache.h"
#include "block-ack-manager.h"
#include "capability-information.h"
#include "cara-wifi-manager.h"
#include "constant-rate-wifi-manager.h"
#include "ctrl-headers.h"
#include "dca-txop.h"
#include "dcf-manager.h"
#include "dcf.h"
#include "dsss-error-rate-model.h"
#include "edca-txop-n.h"
#include "error-rate-model.h"
#include "ht-capabilities.h"
#include "ht-wifi-mac-helper.h"
#include "ideal-wifi-manager.h"
#include "interference-helper.h"
#include "mac-low.h"
#include "mac-rx-middle.h"
#include "mgt-headers.h"
#include "minstrel-wifi-manager.h"
#include "msdu-aggregator.h"
#include "nist-error-rate-model.h"
#include "nqos-wifi-mac-helper.h"
#include "onoe-wifi-manager.h"
#include "originator-block-ack-agreement.h"
#include "qos-tag.h"
#include "qos-utils.h"
#include "qos-wifi-mac-helper.h"
#include "regular-wifi-mac.h"
#include "rraa-wifi-manager.h"
#include "snr-tag.h"
#include "ssid.h"
#include "sta-wifi-mac.h"
#include "status-code.h"
#include "supported-rates.h"
#include "wifi-channel.h"
#include "wifi-helper.h"
#include "wifi-information-element-vector.h"
#include "wifi-information-element.h"
#include "wifi-mac-header.h"
#include "wifi-mac-queue.h"
#include "wifi-mac-trailer.h"
#include "wifi-mac.h"
#include "wifi-mode.h"
#include "wifi-net-device.h"
#include "wifi-phy-standard.h"
#include "wifi-phy-state-helper.h"
#include "wifi-phy.h"
#include "wifi-preamble.h"
#include "wifi-remote-station-manager.h"
#include "wifi-tx-vector.h"
#include "yans-error-rate-model.h"
#include "yans-wifi-channel.h"
#include "yans-wifi-helper.h"
#include "yans-wifi-phy.h"
#endif
