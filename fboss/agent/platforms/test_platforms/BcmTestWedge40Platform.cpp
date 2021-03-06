/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include "fboss/agent/platforms/test_platforms/BcmTestWedge40Platform.h"
#include "fboss/agent/platforms/common/PlatformProductInfo.h"
#include "fboss/agent/platforms/test_platforms/BcmTestWedge40Port.h"

#include "fboss/agent/hw/switch_asics/Trident2Asic.h"

namespace {
static const std::array<int, 16> kMasterLogicalPortIds =
    {1, 5, 9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61};

constexpr uint8_t kNumPortsPerTransceiver = 4;
} // namespace

namespace facebook::fboss {

BcmTestWedge40Platform::BcmTestWedge40Platform(
    std::unique_ptr<PlatformProductInfo> productInfo)
    : BcmTestWedgePlatform(
          std::move(productInfo),
          std::vector<PortID>(
              kMasterLogicalPortIds.begin(),
              kMasterLogicalPortIds.end()),
          kNumPortsPerTransceiver) {
  asic_ = std::make_unique<Trident2Asic>();
}

std::unique_ptr<BcmTestPort> BcmTestWedge40Platform::createTestPort(PortID id) {
  return std::make_unique<BcmTestWedge40Port>(id, this);
}

HwAsic* BcmTestWedge40Platform::getAsic() const {
  return asic_.get();
}

BcmTestWedge40Platform::~BcmTestWedge40Platform() {}

} // namespace facebook::fboss
