/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include "fboss/agent/hw/bcm/BcmSwitchEventCallback.h"

#include <folly/logging/xlog.h>
#include <glog/logging.h>
#include "fboss/agent/hw/bcm/BcmSwitchEventUtils.h"

namespace facebook::fboss {

void BcmSwitchEventUnitNonFatalErrorCallback::callback(
    const int unit,
    const opennsl_switch_event_t eventID,
    const uint32_t arg1,
    const uint32_t arg2,
    const uint32_t arg3) {
  BcmSwitchEventUtils::exportEventCounters(eventID, 1);

  auto alarm = BcmSwitchEventUtils::getAlarmName(eventID);

  logNonFatalError(unit, alarm, eventID, arg1, arg2, arg3);
}

void BcmSwitchEventUnitNonFatalErrorCallback::logNonFatalError(
    int unit,
    const std::string& alarm,
    opennsl_switch_event_t eventID,
    uint32_t arg1,
    uint32_t arg2,
    uint32_t arg3) {
  XLOG(ERR) << "BCM non-fatal error on unit " << unit << ": " << alarm << " ("
            << eventID << ") with params " << arg1 << ", " << arg2 << ", "
            << arg3;
}
void BcmSwitchEventUnitFatalErrorCallback::callback(
    const int unit,
    const opennsl_switch_event_t eventID,
    const uint32_t arg1,
    const uint32_t arg2,
    const uint32_t arg3) {
  auto alarm = BcmSwitchEventUtils::getAlarmName(eventID);
  XLOG(FATAL) << "BCM Fatal error on unit " << unit << ": " << alarm << " ("
              << eventID << ") with params " << arg1 << ", " << arg2 << ", "
              << arg3;
}

} // namespace facebook::fboss
