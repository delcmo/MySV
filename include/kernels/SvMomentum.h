#ifndef SVMOMENTUM_H
#define SVMOMENTUM_H

#include "Kernel.h"
#include "Function.h"
#include "EquationOfState.h"

//Forward Declarations
class SvMomentum;

template<>
InputParameters validParams<SvMomentum>();

class SvMomentum : public Kernel
{
public:

  SvMomentum(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  VariableValue & _h;
  VariableValue & _hu;
  VariableValue & _hv;

  Real _g;
  unsigned int _component;

  Function & _b;

  const EquationOfState & _eos;

  unsigned int _h_var;
  unsigned int _hu_var;
  unsigned int _hv_var;
};

#endif