#include <example/AR.h>
#include <od/extras/LookupTables.h>
#include <od/constants.h>
#include <od/config.h>
#include <hal/ops.h>

namespace example {

  AR::AR() {
    addInput(mGateInput);
    addInput(mAttack);
    addInput(mRelease);
    addOutput(mOutput);

    mAttackStage  = &od::LookupTables::ConcaveParabolicRise;
    mReleaseStage = &od::LookupTables::ConcaveParabolicFall;
  }

  AR::~AR() { }

  void AR::process() {
    float *gate    = mGateInput.buffer();
    float *attack  = mAttack.buffer();
    float *release = mRelease.buffer();
    float *out     = mOutput.buffer();

    mAttackPhaseChange  = globalConfig.samplePeriod / MAX(0.0001f, attack[0]);
    mReleasePhaseChange = globalConfig.samplePeriod / MAX(0.0001f, release[0]);

    for (int i = 0; i < FRAMELENGTH; i++) {
      out[i] = next(gate[i]);
    }
  }

  inline float AR::next(float currentGate) {
    switch (mStage) {
      case 0:
        if (currentGate > 0.5f) {
          mStage   = 1;
          mPhase   = 0.0f;
          mCapture = mCurrentValue;
        }
        break;

      case 1:
        mPhase += mAttackPhaseChange;
        if (mPhase < 1.0f) {
          mCurrentValue = mCapture + (1.0f - mCapture) * mAttackStage->get(mPhase);
        } else {
          mCapture = mCurrentValue;
          mPhase   = 0.0f;
          mStage   = 2;
        }
        break;

      case 2:
        mCurrentValue = 1.0f;
        if (currentGate < 0.5f) {
          mCapture = mCurrentValue;
          mPhase   = 0.0f;
          mStage   = 3;
        }
        break;

      case 3:
        mPhase += mReleasePhaseChange;
        if (mPhase < 1.0f) {
          mCurrentValue = mCapture * mReleaseStage->get(mPhase);
        } else {
          mCapture      = 0.0f;
          mPhase        = 0.0f;
          mStage        = 0;

          // Force to zero, possible discontinuity
          mCurrentValue = 0.0f;
        }
        break;
    }

    return mCurrentValue;
  }
}