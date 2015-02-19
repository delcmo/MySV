#ifndef SVMASS_H
#define SVMASS_H

#include "Kernel.h"

//Forward Declarations
class SvMass;

template<>
InputParameters validParams<SvMass>();

class SvMass : public Kernel
{
public:

  SvMass(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  VariableValue & _hu;
  VariableValue & _hv;

  unsigned int _hu_var;
  unsigned int _hv_var;
};

#endif
