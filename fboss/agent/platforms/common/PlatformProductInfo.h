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

#include <folly/Range.h>
#include "fboss/agent/if/gen-cpp2/ctrl_types.h"
#include "fboss/agent/platforms/common/PlatformMode.h"

DECLARE_string(fruid_filepath);

namespace facebook::fboss {

class PlatformProductInfo {
 public:
  explicit PlatformProductInfo(folly::StringPiece path);

  void getInfo(ProductInfo& info);
  PlatformMode getMode() const;
  void initialize();
  std::string getFabricLocation();
  std::string getProductName();

 private:
  // Forbidden copy constructor and assignment operator
  PlatformProductInfo(PlatformProductInfo const&) = delete;
  PlatformProductInfo& operator=(PlatformProductInfo const&) = delete;

  void setFBSerial();
  void initFromFbWhoAmI();
  void initMode();
  void parse(std::string data);

  ProductInfo productInfo_;
  folly::StringPiece path_;
  PlatformMode mode_;
};

} // namespace facebook::fboss
