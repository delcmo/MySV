#ifndef SAINTVENANTWALLBC_H
#define SAINTVENANTWALLBC_H

#include "IntegratedBC.h"
#include "Function.h"

// Forward Declarations
class SaintVenantWallBC;
class EquationOfState;

template<>
InputParameters validParams<SaintVenantWallBC>();

class SaintVenantWallBC : public IntegratedBC
{
public:
  SaintVenantWallBC(const std::string & name, InputParameters parameters);
  virtual ~SaintVenantWallBC(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Equation type
  enum EquationType
  {
    continuity = 0,
    x_mom = 1
  };
  MooseEnum _equ_type;

  // Coupled variables
  VariableValue & _h;

  // Equation of state
  const EquationOfState & _eos;

  // Integers for jacobian terms
  unsigned _h_var;
  unsigned _hu_var;
};

#endif // SAINTVENANTWALLBC_H

