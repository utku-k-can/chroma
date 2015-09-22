// -*- C++ -*-
/*! \file
 *  \brief Unpreconditioned Clover fermion action
 */

#ifndef __unprec_clover_back_fermact_w_h__
#define __unprec_clover_back_fermact_w_h__

#include "unprec_wilstype_fermact_w.h"
#include "actions/ferm/linop/lgherm_w.h"
#include "actions/ferm/fermacts/clover_back_fermact_params_w.h"

namespace Chroma
{
  //! Name and registration
  /*! \ingroup fermacts */
  namespace UnprecCloverBackFermActEnv
  {
    extern const std::string name;
    bool registerAll();
  }

  //! Unpreconditioned Clover fermion action
  /*! \ingroup fermacts
   *
   * Unpreconditioned clover fermion action
   */
  class UnprecCloverBackFermAct : public UnprecWilsonTypeFermAct<LatticeFermion, 
			      multi1d<LatticeColorMatrix>, multi1d<LatticeColorMatrix> >
  {
  public:
    // Typedefs to save typing
    typedef LatticeFermion               T;
    typedef multi1d<LatticeColorMatrix>  P;
    typedef multi1d<LatticeColorMatrix>  Q;

    //! General FermBC
    /*! Isotropic action */
    UnprecCloverBackFermAct(Handle< CreateFermState<T,P,Q> > cfs_,
			const CloverBackFermActParams& param_) : 
      cfs(cfs_), param(param_) {}

    //! Copy constructor
    UnprecCloverBackFermAct(const UnprecCloverBackFermAct& a) : 
      cfs(a.cfs), param(a.param) {}

    //! Produce a linear operator for this action
    UnprecLinearOperator<T,P,Q>* linOp(Handle< FermState<T,P,Q> > state) const;

    LinearOperator<T>* hermitianLinOp(Handle< FermState<T,P,Q> > state) const 
      { 
	return new lgherm<T>(linOp(state));
      }

    //! Destructor is automatic
    ~UnprecCloverBackFermAct() {}

  protected:
    //! Return the fermion BC object for this action
    const CreateFermState<T,P,Q>& getCreateState() const {return *cfs;}

    // Hide partial constructor
    UnprecCloverBackFermAct() {}

    //! Assignment
    void operator=(const UnprecCloverBackFermAct& a) {}

  private:
    Handle< CreateFermState<T,P,Q> > cfs;
    CloverBackFermActParams param;
  };

}

#endif
