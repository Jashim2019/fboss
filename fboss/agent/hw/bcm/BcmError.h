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

#include "fboss/agent/FbossError.h"
#include "fboss/agent/hw/bcm/BcmSwitch.h"

#include <folly/Format.h>
#include <folly/logging/xlog.h>

extern "C" {
#include <opennsl/error.h>
#include <opennsl/pkt.h>
#include <shared/error.h>
}

namespace facebook::fboss {

/**
 * A class for errors from the Broadcom SDK.
 */
class BcmError : public FbossError {
 public:
  template <typename... Args>
  BcmError(int err, Args&&... args)
      : FbossError(std::forward<Args>(args)..., ": ", opennsl_errmsg(err)),
        err_(static_cast<opennsl_error_t>(err)) {}

  template <typename... Args>
  BcmError(opennsl_error_t err, Args&&... args)
      : FbossError(std::forward<Args>(args)..., ": ", opennsl_errmsg(err)),
        err_(err) {}

  ~BcmError() throw() override {}

  opennsl_error_t getBcmError() const {
    return err_;
  }

 private:
  opennsl_error_t err_;
};

template <typename... Args>
void bcmCheckError(int err, Args&&... msgArgs) {
  if (OPENNSL_FAILURE(err)) {
    XLOG(ERR) << folly::to<std::string>(std::forward<Args>(msgArgs)...) << ": "
              << opennsl_errmsg(err) << ", " << err;
    throw BcmError(err, std::forward<Args>(msgArgs)...);
  }
}

template <typename... Args>
void bcmLogError(int err, Args&&... msgArgs) {
  if (OPENNSL_FAILURE(err)) {
    XLOG(ERR) << folly::to<std::string>(std::forward<Args>(msgArgs)...) << ": "
              << opennsl_errmsg(err) << ", " << err;
  }
}

template <typename... Args>
void bcmLogFatal(int err, const BcmSwitchIf* hw, Args&&... msgArgs) {
  if (OPENNSL_FAILURE(err)) {
    auto errMsg = folly::sformat(
        "{}: {}, {}",
        folly::to<std::string>(std::forward<Args>(msgArgs)...),
        opennsl_errmsg(err),
        err);

    // Make sure we log the error message, in case hw->exitFatal throws.
    XLOG(ERR) << errMsg;
    hw->exitFatal();
    XLOG(FATAL) << errMsg;
  }
}

} // namespace facebook::fboss
