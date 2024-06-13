#pragma once

#include <Logger/Utils.h>
#include <law/EventPolling/Poll/Poll.h>
#include <elaw/FileWatch/Inotify/Inotify.h>
#include <tpapi/EndPoint/DataTransport/UdsUdpServer.h>

#include "utils/Utils.h"

#include "config/IConfigManager.h"

#include "core/battery/battery.h"
#include "core/battery_charger/battery_charger.h"
#include "core/tla2024/TLA2024.h"

// #include "transport/DataTransportFactory.h"
// #include "core/GnssModule/GnssModuleFactory.h"
// #include "core/NavigationDataSupplier/NavigationDataSupplierFactory.h"

// #include "api/Rtcm/Rtcm3DataServer.h"
// #include "api/Tpapi/TpapiController.h"
// #include "api/Tpapi/EndPoint/TpApiEp.h"
// #include "api/Gnss/RawGnssDataServer.h"
// #include "api/Tpapi/ConfigurationController/TpapiConfigurationController.h"
// #include "api/Tpapi/ConfigurationController/RTKController/TpapiRTKController.h"

class Startup {
private:
    static constexpr std::uint8_t kMaxPollSubs = 30;

private:
    using Poll = law::ep::poll::StaticPoll<kMaxPollSubs>;
    using PollPtr = law::ep::poll::PollObsPtr;

private:
    Poll poll;
    PollPtr pollPtr{&poll};
public:
    Status run(config::ConfigManagerObsPtr configManager) {
        const auto &conf = configManager->getConfig();

        Logger::logger.setCurrentPriority(
                Logger::getMessagePriorityByName(
                        utils::etlStringToStrView(conf.logging.level)));

        auto &configCore = conf.core;

        core::TLA2024 tla {};
        core::Battery battery { tla };
        core::bch::BatteryCharger batteryCharger { battery, configCore, pollPtr };

        // const auto &apiConfig = conf.api;
        
        // const auto &udsUdpServerConf =
        //         std::get<config::tp::UdsUdpServer>(conf.api.tpapi.ep);
        // tpapi::ep::UdsUdpServer udsUdpServer(
        //         utils::etlStringToStrView(udsUdpServerConf.path), pollPtr);
        // api::tp::TpApiEndPoint tpapiEp{tpapi::ep::DataTransportObsPtr(&udsUdpServer), pollPtr};

        // api::TpapiController tpapiController{
        //         api::tp::EndPointObsPtr(&tpapiEp),
        //         api::conf::TpapiConfigurationControllerObsPtr(&tpapiConfigurationController)
        // };

        return startServe();
    }

private:
    Status startServe() noexcept {
        utils::ExitFlag ef(SIGINT, SIGTERM, SIGQUIT);
        while (!ef.get()) {
            BOOST_LEAF_CHECK(poll.poll());
        }

        return {};
    }
};
