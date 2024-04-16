// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file   CellOccupancyCheck.h
/// \author Ananya Rai
///

#ifndef QC_MODULE_EMCAL_EMCALCELLOCCUPANCYCHECK_H
#define QC_MODULE_EMCAL_EMCALCELLOCCUPANCYCHECK_H

#include "QualityControl/CheckInterface.h"

namespace o2::quality_control_modules::emcal
{

/// \brief  Check whether cell occupancy is greater than threshold value or not
///
/// \author Ananya Rai
class CellOccupancyCheck : public o2::quality_control::checker::CheckInterface
{
 public:
  /// Default constructor
  CellOccupancyCheck() = default;
  /// Destructor
  ~CellOccupancyCheck() override = default;

  // Override interface
  void configure() override;
  Quality check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap) override;
  void beautify(std::shared_ptr<MonitorObject> mo, Quality checkResult = Quality::Null) override;
  std::string getAcceptedType() override;

 private:
  /************************************************
   * threshold cuts                               *
   ************************************************/
  int mThresholdMaxCounts = 10000; ///< Max number of counts in supermodule

  ClassDefOverride(CellOccupancyCheck, 3);
};

} // namespace o2::quality_control_modules::emcal

#endif // QC_MODULE_EMCAL_EMCALCELLOCCUPANCYCHECK_H
