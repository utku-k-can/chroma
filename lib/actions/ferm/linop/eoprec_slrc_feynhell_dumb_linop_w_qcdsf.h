// -*- C++ -*-
// $Id: eoprec_clover_dumb_linop_w.h,v 3.1 2009-04-17 02:05:33 bjoo Exp $
/*! \file
 *  \brief Even-odd preconditioned Clover fermion linear operator
 */

#ifndef __prec_slrc_feynhell_dumb_linop_w_qcdsf_h__
#define __prec_slrc_feynhell_dumb_linop_w_qcdsf_h__

#include "state.h"
#include "fermbc.h"
#include "eoprec_logdet_linop.h"
#include "actions/ferm/fermacts/slrc_feynhell_fermact_params_w.h"
#include "actions/ferm/fermacts/clover_fermact_params_w.h"
#include "actions/ferm/linop/dslash_w.h"
#include "actions/ferm/linop/clover_term_w.h"

#include "actions/ferm/linop/eoprec_slrc_linop_w.h"
#include "actions/ferm/fermstates/simple_fermstate.h"
#include "actions/ferm/fermstates/periodic_fermstate.h"


namespace Chroma 
{ 

#if 1 
  //! Even-odd preconditioned Clover-Dirac operator
  /*!
   * \ingroup linop
   *
   * This routine is specific to Wilson fermions!
   *
   * The kernel for Clover fermions is
   *
   *      M  =  A + (d+M) - (1/2) D'

   * This is a dumb version with only a constructor and an 
   * apply method. It is also fixed to single precision
   */
  class EvenOddPrecDumbSlrcFeynHellFLinOpQCDSF : public LinearOperator<LatticeFermionF>
  {
  public:
    typedef LatticeFermionF T;
    typedef LatticeColorMatrixF U;
    typedef multi1d<U> P;
    typedef multi1d<U> Q;

    typedef LatticeFermion  TR;
    typedef LatticeColorMatrix  UR;
    typedef multi1d<UR> PR;
    typedef multi1d<UR> QR;

    //! Full constructor
    EvenOddPrecDumbSlrccFeynHellFLinOpQCDSF(Handle< FermState<LatticeFermionF,P,Q> > fs,Handle< FermState<LatticeFermionF,P,Q> > tfs,
				   const CloverFermActParams& param_, const SLRCFeynHellFermActParams& fhparam_)
    {create(fs,tfs,param_,fhparam_);}

    //! Destructor is automatic
    ~EvenOddPrecDumbSlrcFeynHellFLinOpQCDSF() {}

    //! Return the fermion BC object for this linear operator
    const FermBC<LatticeFermionF,P,Q>& getFermBC() const {return D.getFermBC();}

    //! Creation routine
    void create(Handle< FermState<LatticeFermionF,P,Q> > fs,Handle< FermState<LatticeFermionF,P,Q> > tfs,
		const CloverFermActParams& param_, const SLRCFeynHellFermActParams& fhparam_) {

      // QDPIO::cout << __PRETTY_FUNCTION__ << ": enter (SP)" << endl;

      param = param_;
      fhparam = fhparam_;
      clov.create(tfs, param);
      invclov.create(tfs,param,clov);  // make a copy
      invclov.choles(0);  // invert the cb=0 part
      D.create(fs, param.anisoParam);

      // ORIGINAL
      //
      // param = param_;
      // clov.create(fs, param);
      // invclov.create(fs,param,clov);  // make a copy
      // invclov.choles(0);  // invert the cb=0 part
      // D.create(fs, param.anisoParam);
      
    }


    // Override inherited one with a few more funkies
    void operator()(LatticeFermionF& chi, const LatticeFermionF& psi, 
		    enum PlusMinus isign) const 
    {
      START_CODE();
      
      LatticeFermionF tmp1; 
      LatticeFermionF tmp2; 
      Real mquarter = -0.25;
      Complex phalf = 0.5 * fhparam.lambda;

      //  tmp1_o  =  D_oe   A^(-1)_ee  D_eo  psi_o
      D.apply(tmp1, psi, isign, 0);
      
      invclov.apply(tmp2, tmp1, isign, 0);
      
      D.apply(tmp1, tmp2, isign, 1);
      
      //  chi_o  =  A_oo  psi_o  -  tmp1_o
      clov.apply(chi, psi, isign, 1);
      
      chi[rb[1]] += mquarter*tmp1;
      
      tmp1[rb[1]] = Gamma(fhparam.op) * psi;
      
      if( isign == PLUS ) {
	chi[rb[1]] += phalf * tmp1;
      }
      else {
	chi[rb[1]] -= phalf * tmp1;
      }

      // Twisted Term?
      if( param.twisted_m_usedP ){ 
	// tmp1 = i mu gamma_5 tmp1
	tmp1[rb[1]] = (Gamma(15) * timesI(psi));
	
	if( isign == PLUS ) {
	  chi[rb[1]] += param.twisted_m * tmp1;
	}
	else {
	  chi[rb[1]] -= param.twisted_m * tmp1;
	}
      }
      
      END_CODE();
      
    }
    

    //! Return flops performed by the operator()
    unsigned long nFlops() const 
    {
      unsigned long cbsite_flops = 2*D.nFlops()+2*clov.nFlops()+4*Nc*Ns;
      if(  param.twisted_m_usedP ) { 
	cbsite_flops += 4*Nc*Ns; // a + mu*b : a = chi, b = g_5 I psi
      }
      return cbsite_flops*(Layout::sitesOnNode()/2);
    }

    const Subset& subset() const 
    {
      return rb[1];
    }

  private:
    CloverFermActParams param;
    SLRCFeynHellFermActParams fhparam;
    WilsonDslashF D;
    CloverTermF   clov;
    CloverTermF   invclov;  // uggh, only needed for evenEvenLinOp
  };

#endif
#if 1
  //! Even-odd preconditioned Clover-Dirac operator
  /*!
   * \ingroup linop
   *
   * This routine is specific to Wilson fermions!
   *
   * The kernel for Clover fermions is
   *
   *      M  =  A + (d+M) - (1/2) D'

   * This is a dumb version with only a constructor and an 
   * apply method. It is also fixed to double precision
   */
  class EvenOddPrecDumbSlrcFeynHellDLinOpQCDSF : public LinearOperator<LatticeFermionD>
  {
  public:
    typedef LatticeFermionD T;
    typedef LatticeColorMatrixD U;
    typedef multi1d<U> P;
    typedef multi1d<U> Q;

    //! Full constructor
    EvenOddPrecDumbSlrcFeynHellDLinOpQCDSF(Handle< FermState<T,P,Q> > fs,Handle< FermState<T,P,Q> > tfs,
				const CloverFermActParams& param_, const SLRCFeynHellFermActParams& fhparam_)
    {create(fs,tfs,param_,fhparam_);}

    //! Destructor is automatic
    ~EvenOddPrecDumbSlrcFeynHellDLinOpQCDSF() {}

    //! Return the fermion BC object for this linear operator
    const FermBC<T,P,Q>& getFermBC() const {return D.getFermBC();}

    //! Creation routine
    void create(Handle< FermState<T,P,Q> > fs,Handle< FermState<T,P,Q> > tfs,
		const CloverFermActParams& param_, const SLRCFeynHellFermActParams& fhparam_) {
      START_CODE();
      
      // QDPIO::cout << __PRETTY_FUNCTION__ << ": enter (DP)" << endl;
      
      param = param_;
      fhparam = fhparam_;
      clov.create(tfs, param);
      invclov.create(tfs,param,clov);  // make a copy
      invclov.choles(0);  // invert the cb=0 part
      D.create(fs, param.anisoParam);
      
      // QDPIO::cout << __PRETTY_FUNCTION__ << ": exit" << endl;
      END_CODE();
      
    }

 
    // Override inherited one with a few more funkies
    void operator()(T& chi, const T& psi, 
		    enum PlusMinus isign) const 
    {

      START_CODE();
      
      T tmp1; 
      T tmp2; 
      Real mquarter = -0.25;
      Complex phalf = 0.5 * fhparam.lambda;

      //  tmp1_o  =  D_oe   A^(-1)_ee  D_eo  psi_o
      D.apply(tmp1, psi, isign, 0);
      
      invclov.apply(tmp2, tmp1, isign, 0);
      
      D.apply(tmp1, tmp2, isign, 1);
      
      //  chi_o  =  A_oo  psi_o  -  tmp1_o
      clov.apply(chi, psi, isign, 1);
      
      chi[rb[1]] += mquarter*tmp1;
      
      tmp1[rb[1]] = Gamma(fhparam.op) * psi;
      
      if( isign == PLUS ) {
	chi[rb[1]] += phalf * tmp1;
      }
      else {
	chi[rb[1]] -= phalf * tmp1;
      }

      // Twisted Term?
      if( param.twisted_m_usedP ){ 
	// tmp1 = i mu gamma_5 tmp1
	tmp1[rb[1]] = (Gamma(15) * timesI(psi));
	
	if( isign == PLUS ) {
	  chi[rb[1]] += param.twisted_m * tmp1;
	}
	else {
	  chi[rb[1]] -= param.twisted_m * tmp1;
	}
      }
      
      END_CODE();
      
    }


    //! Return flops performed by the operator()
    unsigned long nFlops() const
    {
      unsigned long cbsite_flops = 2*D.nFlops()+2*clov.nFlops()+4*Nc*Ns;
      cbsite_flops += 4*Nc*Ns;
      if(  param.twisted_m_usedP ) { 
	cbsite_flops += 4*Nc*Ns; // a + mu*b : a = chi, b = g_5 I psi
      }
      return cbsite_flops*(Layout::sitesOnNode()/2);
    }

    const Subset& subset() const 
    {
      return rb[1];
    }

  private:
    CloverFermActParams param;
    SLRCFeynHellFermActParams fhparam;
    WilsonDslashD D;
    CloverTermD   clov;
    CloverTermD   invclov;  // uggh, only needed for evenEvenLinOp
  };

#endif

} // End Namespace Chroma


#endif
