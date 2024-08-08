#include "TChain.h"
#include "TROOT.h"

#include "AliAODInputHandler.h"
#include "AliAnalysisAlien.h"
#include "AliAnalysisManager.h"

#include "AliAnalysisTutorialTask.h"

void runAnalysis() {

    gInterpreter->ProcessLine(".include $ROOTSYS/include");
    gInterpreter->ProcessLine(".include $ALICE_ROOT/include");

    Bool_t local = kFALSE;    // set if you want to run the analysis locally (kTRUE), or on grid (kFALSE)
    Bool_t gridTest = kTRUE;  // if you run on grid, specify test mode (kTRUE) or full grid model (kFALSE)

    AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisManager_TutorialTask");

    AliAnalysisAlien *alienHandler;

    if (!local) {
        alienHandler = new AliAnalysisAlien();
        alienHandler->SetCheckCopy(kFALSE);
        alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
        alienHandler->SetAdditionalLibs("AliAnalysisTutorialTask.cxx AliAnalysisTutorialTask.h");
        alienHandler->SetAnalysisSource("AliAnalysisTutorialTask.cxx");
        alienHandler->SetAliPhysicsVersion("vAN-20240807_O2-1");
        alienHandler->SetExecutableCommand("aliroot -l -q -b");
        alienHandler->SetGridDataDir("/alice/data/2015/LHC15o/");
        alienHandler->SetRunPrefix("000");
        alienHandler->AddRunNumber(246757);
        alienHandler->SetDataPattern("/pass2/AOD252/*/AliAOD.root");
        alienHandler->SetSplitMaxInputFileNumber(40);
        alienHandler->SetTTL(3600);
        alienHandler->SetOutputToRunNo(kTRUE);
        alienHandler->SetKeepLogs(kTRUE);
        alienHandler->SetMergeViaJDL(kFALSE);
        // alienHandler->SetMaxMergeStages(1);
        alienHandler->SetGridWorkingDir("TEST_WorkingDir");
        alienHandler->SetGridOutputDir("TEST_OutputDir");
        alienHandler->SetJDLName("TutorialTask.jdl");
        alienHandler->SetExecutable("TutorialTask.sh");
        mgr->SetGridHandler(alienHandler);
    }

    AliAODInputHandler *aodH = new AliAODInputHandler();
    mgr->SetInputEventHandler(aodH);

    gInterpreter->LoadMacro("AliAnalysisTutorialTask.cxx++g");
    AliAnalysisTutorialTask *task = reinterpret_cast<AliAnalysisTutorialTask *>(gInterpreter->ExecuteMacro("AddTask_TutorialTask.C"));

    if (!mgr->InitAnalysis()) return;

    if (!local) {
        if (gridTest) {
            alienHandler->SetNtestFiles(1);
            alienHandler->SetRunMode("test");
        } else {
            alienHandler->SetRunMode("full");
        }
        mgr->StartAnalysis("grid");
    } else {
        TChain *chain = new TChain("aodTree");
        chain->Add("AliAOD.root");
        mgr->StartAnalysis("local", chain);
    }
}
