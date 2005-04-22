#ifndef ovext_const_div_by_resp_h
#define ovext_const_div_by_resp_h

#include "chromabase.h"
#include "ovext_tuning_strategy.h"

namespace Chroma {
    
  namespace OvExtConstDivByResPStrategyEnv { 
    extern const std::string name;
    extern const bool registered;
  };
  
  
  class OvExtConstDivByResPStrategy : public AbsOvExtTuningStrategy {
  public: 
    // Destructor is automatic
    ~OvExtConstDivByResPStrategy(void) {}
    
    // Construct from program
    OvExtConstDivByResPStrategy(const Real& tuningConstant_ ): tuningConstant(tuningConstant_) {}
    
    
    void operator()( multi1d<Real>& beta,
		     const Real& coeffP,
		     const multi1d<Real>& resP,
		     const multi1d<Real>& resQ) const {
      int size=resP.size();
      beta.resize(size);
      for(int i=0; i < size; i++) { 
	beta[i] = Real(1)/ (tuningConstant*resP[i]);
      }
    }
  private:
    const Real tuningConstant;
  };
  
};

#endif
