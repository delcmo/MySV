#ifndef ENTROPYVISCOSITYCOEFFICIENT_H
#define ENTROPYVISCOSITYCOEFFICIENT_H

#include "Material.h"
#include "EquationOfState.h"

class EntropyViscosityCoefficient;

template<>
InputParameters validParams<EntropyViscosityCoefficient>();

/**
 * Computes dissipative fluxes for entropy viscosity method
 */
class EntropyViscosityCoefficient : public Material
{
public:
  EntropyViscosityCoefficient(const std::string & name, InputParameters parameters);
  virtual ~EntropyViscosityCoefficient();

protected:
  virtual void computeQpProperties();

  // Coupled variables
  VariableValue & _h;
  VariableValue & _hu;
  VariableValue & _hv;

  // Coupled gradient
  VariableGradient & _h_grad;
  VariableGradient & _hu_grad;
  VariableGradient & _hv_grad;

  // Equation of state
  const EquationOfState & _eos;

  // material to compute
  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _kappa_max;
};

#endif /* ENTROPYVISCOSITYCOEFFICIENT_H */
