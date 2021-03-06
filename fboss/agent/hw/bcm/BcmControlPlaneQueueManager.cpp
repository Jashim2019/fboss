/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include "fboss/agent/hw/bcm/BcmControlPlaneQueueManager.h"

#include "fboss/agent/hw/StatsConstants.h"
#include "fboss/agent/hw/bcm/BcmError.h"
#include "fboss/agent/hw/bcm/BcmPlatform.h"
#include "fboss/agent/hw/bcm/BcmSwitch.h"

namespace facebook::fboss {

const std::vector<BcmCosQueueCounterType>&
BcmControlPlaneQueueManager::getQueueCounterTypes() const {
  static const std::vector<BcmCosQueueCounterType> types = {
      {cfg::StreamType::MULTICAST,
       BcmCosQueueStatType::DROPPED_PACKETS,
       BcmCosQueueCounterScope::QUEUES,
       "in_dropped_pkts"},
      {cfg::StreamType::MULTICAST,
       BcmCosQueueStatType::OUT_PACKETS,
       BcmCosQueueCounterScope::QUEUES,
       "in_pkts"}};
  return types;
}

BcmPortQueueConfig BcmControlPlaneQueueManager::getCurrentQueueSettings()
    const {
  QueueConfig multicastQueues;
  for (int i = 0; i < getNumQueues(cfg::StreamType::MULTICAST); i++) {
    multicastQueues.push_back(
        getCurrentQueueSettings(cfg::StreamType::MULTICAST, i));
  }
  return BcmPortQueueConfig({}, std::move(multicastQueues));
}

opennsl_gport_t BcmControlPlaneQueueManager::getQueueGPort(
    cfg::StreamType streamType,
    opennsl_cos_queue_t cosQ) const {
  if (!hw_->getPlatform()->isCosSupported()) {
    throw FbossError(
        "Failed to retrieve queue gport because platform doesn't support cosq");
  }
  // cpu only have multicast
  if (streamType == cfg::StreamType::MULTICAST) {
    return cosQueueGports_.multicast.at(cosQ);
  }
  throw FbossError(
      "Failed to retrieve queue gport because unsupported StreamType: ",
      cfg::_StreamType_VALUES_TO_NAMES.find(streamType)->second);
}

const PortQueue& BcmControlPlaneQueueManager::getDefaultQueueSettings(
    cfg::StreamType streamType) const {
  return hw_->getPlatform()->getDefaultControlPlaneQueueSettings(streamType);
}

} // namespace facebook::fboss
