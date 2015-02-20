#include "MomentumSetDensity.h"
#include "EquationOfState.h"

template<>
InputParameters validParams<MomentumSetDensity>()
{
  InputParameters params = validParams<IntegratedBC>();

  // Equation name:
  params.addParam<std::string>("equ_name", "invalid", "Name of the equation.");
  // Coupled variables
  params.addRequiredCoupledVar("hu", "x component of h*\vec{u}");
  params.addCoupledVar("hv", "y component of h*\vec{u}");
  // Constants and parameters
  params.addRequiredParam<Real>("h_bc", "boundary value of the density/water high h");
  // Equation of state
  params.addRequiredParam<UserObjectName>("eos", "The name of equation of state object to use.");

  return params;
}


MomentumSetDensity::MomentumSetDensity(const std::string & name, InputParameters parameters) :
    IntegratedBC(name, parameters),
    // Equation name
    _equ_type("continuity, x_mom, y_mom, invalid", getParam<std::string>("equation_name")),
    // Coupled variables
    _h(coupledValue("h")),
    _hu(coupledValue("hu")),
    _hv(_mesh.dimension() == 2 ? coupledValue("hv") : _zero),
    // Constants and parameters
    _h_bc(getParam<Real>("h_bc")),
    // Equation of state:
    _eos(getUserObject<EquationOfState>("eos")),
    // Integer for jacobian terms
    _h_var(coupled("h")),
    _hu_var(coupled("hu")),
    _hv_var(_mesh.dimension() == 2 ? coupled("hv") : 0)
{
}

Real
MomentumSetDensity::computeQpResidual()
{
  RealVectorValue hU(_hu[_qp], _hv[_qp], 0.);
  Real p = _eos.pressure(_h_bc, hU);
  switch (_equ_type)
  {
    case continuity:
      return hU*_normals[_qp]*_test[_i][_qp];
      break;
    case x_mom:
      return (_u[_qp]/_h_bc*(hU*_normals[_qp])+p*_normals[_qp](0))*_test[_i][_qp];
      break;
    case y_mom:
      return (_u[_qp]/_h_bc*(hU*_normals[_qp])+p*_normals[_qp](1))*_test[_i][_qp];
      break;
    default:
      mooseError("Invalid equation name.");
  }
}

Real
MomentumSetDensity::computeQpJacobian()
{
  return 0.;
}

Real
MomentumSetDensity::computeQpOffDiagJacobian(unsigned jvar)
{
  return 0.;
}

