#ifndef SAINTVENANTSETWATERHEIGHT_H
#define SAINTVENANTSETWATERHEIGHT_H

#include "IntegratedBC.h"
#include "Function.h"

// Forward Declarations
class SaintVenantSetWaterHeight;
class EquationOfState;

template<>
InputParameters validParams<SaintVenantSetWaterHeight>();


/**
**/
class SaintVenantSetWaterHeight : public IntegratedBC
{
public:
  SaintVenantSetWaterHeight(const std::string & name, InputParameters parameters);
  virtual ~SaintVenantSetWaterHeight(){}

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
  VariableValue & _hu;

  // Constants and parameters
  Real _h_bc;

  // Equation of state
  const EquationOfState & _eos;

  // Integers for jacobian terms
  unsigned _hu_var;
};

#endif // SAINTVENANTSETWATERHEIGHT_H

