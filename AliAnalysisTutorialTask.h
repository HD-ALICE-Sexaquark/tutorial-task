#ifndef AliAnalysisTutorialTask_h
#define AliAnalysisTutorialTask_h

#ifndef AliAnalysisTaskSE_h
#include "AliAnalysisTaskSE.h"
#endif

#include "TChain.h"
#include "TH1.h"
#include "TList.h"
#include "TROOT.h"

#include "AliAODEvent.h"
#include "AliAODTrack.h"

class AliAnalysisTutorialTask : public AliAnalysisTaskSE {
   public:
    AliAnalysisTutorialTask();
    AliAnalysisTutorialTask(const char* name);
    virtual ~AliAnalysisTutorialTask();

    virtual void UserCreateOutputObjects();
    virtual void UserExec(Option_t* option);
    virtual void Terminate(Option_t* option) { return; }

   private:
    AliAODEvent* fAOD;   //!<! input event
    TList* fOutputList;  //!<! output list
    TH1F* fHistPt;       //!<! dummy histogram

    /// \cond CLASSDEF
    ClassDef(AliAnalysisTutorialTask, 1);
    /// \endcond
};

#endif
