#include "QualityControl/MonitorObject.h"
#include "EMCAL/NPhysTriggCheck.h"
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
#include <TGraph.h>

using namespace std;

namespace o2::quality_control_modules::emcal
{

Quality NPhysTriggCheck::check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap)
{
  Quality result = Quality::Good;

  for (auto& [moName, mo] : *moMap) {
    if (mo->getName() == "NPhysicsTriggers") {
      auto* c = dynamic_cast<TCanvas*>(mo->getObject());
      TList* list_name = c->GetListOfPrimitives();
      double counts = -1;
      for (auto trendgraph : TRangeDynCast<TGraph>(list_name)) {
        if (!trendgraph) {
          continue;
        }
        double* yValues = trendgraph->GetY();
        double numPoints = trendgraph->GetN();
        for (int x_graph = 0; x_graph < trendgraph->GetN(); ++x_graph) {
          double y = yValues[x_graph];
          if (y == counts || y < counts) {
            result = Quality::Bad;
            break;
          } else {
            counts = y;
          }
        }
      }
    }
  }
  return result;
}

std::string NPhysTriggCheck::getAcceptedType() { return "TGraph"; }

void NPhysTriggCheck::beautify(std::shared_ptr<MonitorObject> mo, Quality checkResult)
{
  //    if (mo->getName() == “NPhysicsTriggers”)
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
  //            msg->AddText("NPhysicsTriggs is EMPTY");
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
