#ifndef MOMENTUMSETDENSITY_H
#define MOMENTUMSETDENSITY_H

#include "IntegratedBC.h"
#include "Function.h"

// Forward Declarations
class MomentumSetDensity;
class EquationOfState;

template<>
InputParameters validParams<MomentumSetDensity>();


/**
 * A momentum BC with specified static pressure and temperature.
 * This is suitable for use at a subsonic inflow boundary.
 */
class MomentumSetDensity : public IntegratedBC
{
public:
  MomentumSetDensity(const std::string & name, InputParameters parameters);
  virtual ~MomentumSetDensity(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Equation type
  enum EquationType
  {
    continuity = 0,
    x_mom = 1,
    y_mom = 2
  };
  MooseEnum _equ_type;

  // Coupled variables
  VariableValue & _h;
  VariableValue & _hu;
  VariableValue & _hv;

  // Constants and parameters
  Real _h_bc;

  // Equation of state
  const EquationOfState & _eos;

  // Integers for jacobian terms
  unsigned _h_var;
  unsigned _hu_var;
  unsigned _hv_var;
};

#endif // MOMENTUMSETDENSITY_H

