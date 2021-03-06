#pragma once

#include <od/extras/LookupTable.h>
#include <od/objects/Object.h>

namespace example {
  class AR : public od::Object {
    public:
      AR();
      virtual ~AR();

#ifndef SWIGLUA
    virtual void process();
    od::Inlet mGateInput{"Gate"};
    od::Inlet mAttack{"Attack"};
    od::Inlet mRelease{"Release"};
    od::Outlet mOutput{"Out"};
#endif

    private:
      float next(float sustain);

      int mStage = 0;

      float mPhase = 0.0f;
      float mCapture = 0.0f;
      float mAttackPhaseChange = 0.0f;
      float mReleasePhaseChange = 0.0f;
      float mCurrentValue = 0.0f;

      od::LookupTable *mAttackStage = NULL;
      od::LookupTable *mReleaseStage = NULL;
  };
}
