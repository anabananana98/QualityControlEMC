#include "QualityControl/MonitorObject.h"
#include "EMCAL/CellOccupancyCheck.h"
#include "QualityControl/QcInfoLogger.h"
#include "QualityControl/Quality.h"

// ROOT
#include <TH1.h>
#include <TH2.h>
#include <TPaveText.h>
#include <TLatex.h>
#include <TList.h>
#include <TRobustEstimator.h>
#include <ROOT/TSeq.hxx>
#include <iostream>
#include <vector>
#include <TCanvas.h>
#include <string>

using namespace std;

namespace o2::quality_control_modules::emcal
{
void CellOccupancyCheck::configure()
{
  // configure threshold-based checkers
  auto nthresholdMaxCounts = mCustomParameters.find("thresholdMaxCounts");
  if (nthresholdMaxCounts != mCustomParameters.end()) {
    try {
      mThresholdMaxCounts = std::stoi(nthresholdMaxCounts->second);
    } catch (std::exception& e) {
      ILOG(Error, Support) << fmt::format("Value {} not an integer", nthresholdMaxCounts->second.data()) << ENDM;
    }
  }
}

Quality CellOccupancyCheck::check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap)
{
  Quality result = Quality::Good;
  for (auto& [moName, mo] : *moMap) {
    if (mo->getName() == "cellOccupancyEMCwThr_PHYS") {
      auto* h = dynamic_cast<TH2*>(mo->getObject());

      // Define bin edge array of the form (BinEdgeX[n],BinEdgeX[n+1]) = (low bin edge of nth SM, up bin edge of nth SM)
      const int BinEdgesX[40] = {
        1, 48, 49, 96, 1, 48, 49, 96,
        1, 48, 49, 96, 1, 48, 49, 96,
        1, 48, 49, 96, 1, 48, 49, 96,
        1, 32, 65, 96, 1, 32, 65, 96,
        1, 32, 65, 96, 1, 48, 49, 96
      };

      const int BinEdgesY[40] = {
        1, 25, 1, 25, 26, 49, 26, 49,
        50, 73, 50, 73, 74, 97, 74, 97,
        98, 121, 98, 121, 122, 129, 122, 129,
        130, 153, 130, 153, 154, 177, 154, 177,
        178, 201, 178, 201, 201, 208, 201, 208
      };

      // Assign an array to hold counts in a supermodule
      double SMcounts[20];

      double counts;
      for (int nsm = 0; nsm < 40; nsm++) {
        if (nsm % 2 == 0) {
          for (int xsm = BinEdgesX[nsm]; xsm <= BinEdgesX[nsm + 1]; xsm++) {
            for (int ysm = BinEdgesY[nsm]; ysm <= BinEdgesY[nsm + 1]; ysm++) {
              counts = h->GetBinContent(xsm, ysm);
              int SM_number = nsm / 2;
              SMcounts[SM_number] += counts;
            }
          }
        }
      }

      // Check if the counts in any supermodule exceed the threshold
      for (int k = 0; k < 20; k++) {
        if (SMcounts[k] > mThresholdMaxCounts) {
          result = Quality::Bad;
        }
      }
    }
  }
  return result;
}

std::string CellOccupancyCheck::getAcceptedType() { return "TH2"; }

void CellOccupancyCheck::beautify(std::shared_ptr<MonitorObject> mo, Quality checkResult)
{
  //    if (mo->getName() == “cellOccupancyEMCwThr_PHYS”)
  //    {
  //        auto* h = dynamic_cast<TH1*>(mo->getObject());
  //        TPaveText* msg = new TPaveText(0.5, 0.5, 0.9, 0.75, "NDC");
  //        h->GetListOfFunctions()->Add(msg);
  //        msg->SetName(Form("%s_msg", mo->GetName()));
  //        if (checkResult == Quality::Good) {
  //            //
  //            msg->Clear();
  //            msg->AddText("NPhysicsTriggs increases with time: OK!!!");
  //            msg->SetFillColor(kGreen);
  //            //
  //            h->SetFillColor(kGreen);
  //        } else if (checkResult == Quality::Bad) {
  //            ILOG(Debug, Devel) << "Quality::Bad, setting to red";
  //            msg->Clear();
  //            msg->AddText("cellOccupancyEMCwThr_PHYS EMPTY");
  //            msg->AddText("If NOT a technical run,");
  //            msg->AddText("call EMCAL on-call.");
  //            h->SetFillColor(kRed);
  //        } else if (checkResult == Quality::Medium) {
  //            ILOG(Debug, Devel) << "Quality::medium, setting to orange";
  //            h->SetFillColor(kOrange);
  //        }
  //        h->SetLineColor(kBlack);
  //    }
}
} // namespace o2::quality_control_modules::emcal
