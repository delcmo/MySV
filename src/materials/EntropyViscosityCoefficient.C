#include "EntropyViscosityCoefficient.h"

template<>
InputParameters validParams<EntropyViscosityCoefficient>()
{
  InputParameters params = validParams<Material>();

  // Parameters
  params.addParam<bool>("is_first_order", false, "if true, use the first-order viscosity coefficient");
  // Coupled variables
  params.addRequiredCoupledVar("h", "high/density");
  params.addRequiredCoupledVar("hu", "x component of h*\vec{u}");
  params.addCoupledVar("hv", "y component of h*\vec{u}");
  // Coupled aux variables
  params.addRequiredCoupledVar("entropy", "entropy function");
  params.addRequiredCoupledVar("F", "x-component of the entropy flux ");
  params.addCoupledVar("G", "y-component of the entropy flux ");
  params.addCoupledVar("b", "topology");  
  // Equation of state
  params.addRequiredParam<UserObjectName>("eos", "Equation of state");

  return params;
}

EntropyViscosityCoefficient::EntropyViscosityCoefficient(const std::string & name, InputParameters parameters) :
    Material(name, parameters),
// Parameters
    _is_first_order(getParam<bool>("is_first_order")),
    // Coupled variables
    _h(coupledValue("h")),
    _hu(coupledValue("hu")),
    _hv(_mesh.dimension() == 2 ? coupledValue("hv") : _zero),
    // Coupled aux variables: entropy
    _E(coupledValue("entropy")),
    _E_old(coupledValueOld("entropy")),
    _E_older(coupledValueOlder("entropy")),
    // Coupled aux variables: entropy flux
    _F_grad(coupledGradient("F")),
    _G_grad(_mesh.dimension() == 2 ? coupledGradient("G") : _grad_zero),
    // Coupled aux variables: topology
    _b(isCoupled("b") ? coupledValue("b") : _zero),
    // Equation of state:
    _eos(getUserObject<EquationOfState>("eos")),
    // Materials
    _kappa(declareProperty<Real>("kappa")),
    _kappa_max(declareProperty<Real>("kappa_max"))
{
}

EntropyViscosityCoefficient::~EntropyViscosityCoefficient()
{}

void
EntropyViscosityCoefficient::computeQpProperties()
{
  // Cell size
  Real h_cell = _current_elem->hmin();
  
  // Momentum vector
  RealVectorValue hU(_hu[_qp], _hv[_qp], 0.);

  // Speed of sound
  Real c2 = _eos.c2(_h[_qp], hU);

  // First-order viscosity coefficient
  _kappa_max[_qp] = 0.5*h_cell*(hU.size()/_h[_qp]+std::sqrt(c2));

  // Weights for BDF2
  Real w0 = _t_step > 1 ? (2.*_dt+_dt_old)/(_dt*(_dt+_dt_old)) : 1. / _dt;
  Real w1 = _t_step > 1 ? -(_dt+_dt_old)/(_dt*_dt_old) : -1. / _dt;
  Real w2 = _t_step > 1 ? _dt/(_dt_old*(_dt+_dt_old)) : 0.;

  // Entropy residual
  Real residual = w0*_E[_qp]+w1*_E_old[_qp]+w2*_E_older[_qp];
  residual += _F_grad[_qp](0)+_G_grad[_qp](1);

  // Normalization parameter
  Real norm = _h[_qp]+_b[_qp];

  // High-order viscosity coefficient
  _kappa[_qp] = _is_first_order ? h_cell*h_cell*std::fabs(residual/norm) : _kappa_max[_qp];

  // Return value of the viscosity coefficient
}