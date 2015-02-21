#include "MomentumEqu.h"

/** Compute the advection term of the momentum Saint-Venant equation (SV). The pressure P is computed from the material class EquationOfState. The topography is given as a function**/

template<>
InputParameters validParams<MomentumEqu>()
{
  InputParameters params = validParams<Kernel>();

  // Coupled variables
  params.addRequiredCoupledVar("h", "water height");
  params.addRequiredCoupledVar("hu", "x component of h*\vec{u}");
  params.addCoupledVar("hv", "y component of h*\vec{u}");
  // Constants and parameters
  params.addRequiredParam<Real>("gravity", "gravity");
  params.addRequiredParam<unsigned int>("component", "number of component (0 = x, 1 = y, 2 = z)");
  // Equation of state
  params.addRequiredParam<UserObjectName>("eos", "Equation of state");
  // Function
  params.addParam<FunctionName>("topology", " ", "The function to use for the topology");

  return params;
}

MomentumEqu::MomentumEqu(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
    // Coupled variables
    _h(coupledValue("h")),
    _hu(coupledValue("hu")),
    _hv(_mesh.dimension() == 2 ? coupledValue("hv") : _zero),
    // Constants and parameters
    _g(getParam<Real>("gravity")),
    _component(getParam<unsigned int>("component")),
    // Function
    _b(getFunction("topology")),
    // Equation of state:
    _eos(getUserObject<EquationOfState>("eos")),
    // Integer for jacobian terms
    _h_var(coupled("h")),
    _hu_var(coupled("hu")),
    _hv_var(_mesh.dimension() == 2 ? coupled("hv") : 0)
{}

Real
MomentumEqu::computeQpResidual()
{
  // Advection
  RealVectorValue hU(_hu[_qp], _hv[_qp], 0.);
  Real advc = _u[_qp]/_h[_qp]*(hU*_grad_test[_i][_qp]);

  // Pressure
  Real p = _eos.pressure(_h[_qp], hU)*_grad_test[_i][_qp](_component);

  // Topography
  RealVectorValue tplg_grad = _g*_h[_qp]*_b.gradient(_t, _q_point[_qp])*_test[_i][_qp];

  // return value
  return -advc-p+tplg_grad(_component);
}

Real
MomentumEqu::computeQpJacobian()
{
  return 0.;
}

Real
MomentumEqu::computeQpOffDiagJacobian(unsigned int jvar)
{
  return 0.;
}