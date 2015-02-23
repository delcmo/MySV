#include "EntropyViscosityCoefficient.h"

template<>
InputParameters validParams<EntropyViscosityCoefficient>()
{
  InputParameters params = validParams<Material>();
  
  // Coupled variables
  params.addRequiredCoupledVar("h", "high/density");
  params.addRequiredCoupledVar("hu", "x component of h*\vec{u}");
  params.addCoupledVar("hv", "y component of h*\vec{u}");
  // Equation of state
  params.addRequiredParam<UserObjectName>("eos", "Equation of state");

  return params;
}

EntropyViscosityCoefficient::EntropyViscosityCoefficient(const std::string & name, InputParameters parameters) :
    Material(name, parameters),
    // Coupled variables
    _h(coupledValue("h")),
    _hu(coupledValue("hu")),
    _hv(_mesh.dimension() == 2 ? coupledValue("hv") : _zero),
    // Coupled gradient
    _h_grad(coupledGradient("h")),
    _hu_grad(coupledGradient("hu")),
    _hv_grad(_mesh.dimension() == 2 ? coupledGradient("hv") : _grad_zero),
    // Equation of state:
    _eos(getUserObject<EquationOfState>("eos")),
    // Materials
    _kappa(declareProperty<Real>("kappa")),
    _kappa_max(declareProperty<Real>("kappa_max"))
{
}

EntropyViscosityCoefficient::~EntropyViscosityCoefficient()
{
}

void
EntropyViscosityCoefficient::computeQpProperties()
{
  // Cell size
  Real h_cell = _current_elem->hmin();

  // Momentum vector
  RealVectorValue hU(_hu[_qp], _hv[_qp], 0.);

  // Speed of sound
  Real c2 = _eos.c2(_h[_qp], hU);

  // Set material viscosity coefficient
  _kappa_max[_qp] = 0.5*h_cell*(hU.size()/_h[_qp]+std::sqrt(c2));
  _kappa[_qp] = _kappa_max[_qp];
}