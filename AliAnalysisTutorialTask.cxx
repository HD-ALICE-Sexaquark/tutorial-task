#include "AliAnalysisTutorialTask.h"

ClassImp(AliAnalysisTutorialTask);

AliAnalysisTutorialTask::AliAnalysisTutorialTask() : AliAnalysisTaskSE(), fAOD{0}, fOutputList{0}, fHistPt{0} {}

AliAnalysisTutorialTask::AliAnalysisTutorialTask(const char* name) : AliAnalysisTaskSE(name), fAOD{0}, fOutputList{0}, fHistPt{0} {
    DefineInput(0, TChain::Class());
    DefineOutput(1, TList::Class());
}

AliAnalysisTutorialTask::~AliAnalysisTutorialTask() {
    if (fOutputList) delete fOutputList;
}

void AliAnalysisTutorialTask::UserCreateOutputObjects() {
    fOutputList = new TList();
    fOutputList->SetOwner(true);

    fHistPt = new TH1F("fHistPt", "fHistPt", 500, 0, 10);
    fOutputList->Add(fHistPt);

    PostData(1, fOutputList);
}

void AliAnalysisTutorialTask::UserExec(Option_t*) {

    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());
    if (!fAOD) ::Fatal("AliAnalysisTutorialTask::UserExec", "No AOD event found, check the event handler.");

    Int_t nTracks = fAOD->GetNumberOfTracks();

    for (Int_t i = 0; i < nTracks; i++) {

        AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));
        if (!track) continue;

        if (!track->TestFilterBit(128)) continue;

        fHistPt->Fill(track->Pt());
    }

    PostData(1, fOutputList);
}
