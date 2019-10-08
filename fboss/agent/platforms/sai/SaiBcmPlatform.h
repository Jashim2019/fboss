/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include "fboss/agent/platforms/sai/SaiHwPlatform.h"
namespace facebook::fboss {

class SaiBcmPlatform : public SaiHwPlatform {
 public:
  explicit SaiBcmPlatform(std::unique_ptr<PlatformProductInfo> productInfo)
      : SaiHwPlatform(std::move(productInfo)) {}
  std::string getHwConfig() override;
  HwAsic* getAsic() const override {
    /* TODO: implement this */
    return nullptr;
  }
};

} // namespace facebook::fboss
