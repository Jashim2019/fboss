/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "fboss/lib/phy/ExternalPhy.h"
#include "fboss/mdio/MdioError.h"

namespace facebook {
namespace fboss {
namespace phy {

bool LaneConfig::operator==(const LaneConfig& rhs) const {
  return (polaritySwap == rhs.polaritySwap) && (tx == rhs.tx);
}

LaneSettings LaneConfig::toLaneSettings() const {
  LaneSettings settings;
  if (!polaritySwap.has_value()) {
    settings.polaritySwap = {};
  } else {
    settings.polaritySwap = polaritySwap.value();
  }
  if (tx.has_value()) {
    settings.tx_ref() = tx.value();
  }
  return settings;
}

LaneConfig LaneConfig::fromLaneSettings(const LaneSettings& settings) {
  LaneConfig config;
  if (auto tx = settings.tx_ref()) {
    config.tx = *tx;
  }
  config.polaritySwap = settings.polaritySwap;
  return config;
}

bool PhySideConfig::operator==(const PhySideConfig& rhs) const {
  return std::equal(
      lanes.begin(), lanes.end(), rhs.lanes.begin(), rhs.lanes.end());
}

bool ExternalPhyConfig::operator==(const ExternalPhyConfig& rhs) const {
  return (system == rhs.system) && (line == rhs.line);
}

bool ExternalPhyProfileConfig::operator==(
    const ExternalPhyProfileConfig& rhs) const {
  return (speed == rhs.speed) && (system == rhs.system) && (line == rhs.line);
}

bool PhyPortConfig::operator==(const PhyPortConfig& rhs) const {
  return config == rhs.config && profile == rhs.profile;
}

bool PhyPortConfig::operator!=(const PhyPortConfig& rhs) const {
  return !(*this == rhs);
}

ExternalPhyProfileConfig ExternalPhyProfileConfig::fromPortProfileConfig(
    const PortProfileConfig& portCfg) {
  if (!portCfg.xphyLine_ref()) {
    throw MdioError(
        "Attempted to create xphy config without xphy line settings");
  }
  ExternalPhyProfileConfig xphyCfg;
  xphyCfg.speed = portCfg.speed;
  xphyCfg.system = portCfg.iphy;
  xphyCfg.line = *portCfg.xphyLine_ref();
  return xphyCfg;
}

PhyPortSettings PhyPortConfig::toPhyPortSettings(int16_t phyID) const {
  PhyPortSettings settings;

  for (auto in : config.system.lanes) {
    settings.system.lanes.insert(
        std::pair<int16_t, LaneSettings>(in.first, in.second.toLaneSettings()));
  }
  for (auto in : config.line.lanes) {
    settings.line.lanes.insert(
        std::pair<int16_t, LaneSettings>(in.first, in.second.toLaneSettings()));
  }

  settings.phyID = phyID;
  settings.speed = profile.speed;
  settings.line.modulation = profile.line.modulation;
  settings.system.modulation = profile.system.modulation;
  settings.line.fec = profile.line.fec;
  settings.system.fec = profile.system.fec;

  return settings;
}

PhyPortConfig PhyPortConfig::fromPhyPortSettings(
    const PhyPortSettings& settings) {
  PhyPortConfig config;
  ProfileSideConfig systemSpeedSettings;
  ProfileSideConfig lineSpeedSettings;
  PhySideConfig systemPhySettings;
  PhySideConfig linePhySettings;

  for (auto in : settings.system.lanes) {
    systemPhySettings.lanes.insert(std::pair<int32_t, LaneConfig>(
        in.first, LaneConfig::fromLaneSettings(in.second)));
  }
  config.config.system = systemPhySettings;

  for (auto in : settings.line.lanes) {
    linePhySettings.lanes.insert(std::pair<int32_t, LaneConfig>(
        in.first, LaneConfig::fromLaneSettings(in.second)));
  }
  config.config.line = linePhySettings;

  lineSpeedSettings.numLanes = settings.line.lanes.size();
  lineSpeedSettings.modulation = settings.line.modulation;
  lineSpeedSettings.fec = settings.line.fec;

  systemSpeedSettings.numLanes = settings.system.lanes.size();
  systemSpeedSettings.modulation = settings.system.modulation;
  systemSpeedSettings.fec = settings.system.fec;

  config.profile.speed = settings.speed;
  config.profile.line = lineSpeedSettings;
  config.profile.system = systemSpeedSettings;

  return config;
}
} // namespace phy
} // namespace fboss
} // namespace facebook
