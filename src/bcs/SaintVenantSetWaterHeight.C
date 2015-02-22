#include "SaintVenantSetWaterHeight.h"
#include "EquationOfState.h"

/** Set the water height at the boundary **/

template<>
InputParameters validParams<SaintVenantSetWaterHeight>()
{
  InputParameters params = validParams<IntegratedBC>();

  // Equation name:
  params.addParam<std::string>("equ_name", "invalid", "Name of the equation.");
  // Coupled variables
  params.addRequiredCoupledVar("hu", "x component of h*\vec{u}");
  // Constants and parameters
  params.addRequiredParam<Real>("h_bc", "boundary value of the density/water high h");
  // Equation of state
  params.addRequiredParam<UserObjectName>("eos", "The name of equation of state object to use.");

  return params;
}


SaintVenantSetWaterHeight::SaintVenantSetWaterHeight(const std::string & name, InputParameters parameters) :
    IntegratedBC(name, parameters),
    // Equation name
    _equ_type("continuity x_mom invalid", getParam<std::string>("equ_name")),
    // Coupled variables
    _hu(coupledValue("hu")),
    // Constants and parameters
    _h_bc(getParam<Real>("h_bc")),
    // Equation of state:
    _eos(getUserObject<EquationOfState>("eos")),
    // Integer for jacobian terms
    _hu_var(coupled("hu"))
{
  if (_mesh.dimension() == 2)
    mooseError("'" << this->name() << "' can only be used with 1-D mesh since it is deisnged for the Saint-Venant equations.");
}

Real
SaintVenantSetWaterHeight::computeQpResidual()
{
  RealVectorValue hU(_hu[_qp], 0., 0.);
  Real p = _eos.pressure(_h_bc, hU);
  switch (_equ_type)
  {
    case continuity:
      return hU*_normals[_qp]*_test[_i][_qp];
      break;
    case x_mom:
      return (_u[_qp]*_u[_qp]/_h_bc+p)*_normals[_qp](0)*_test[_i][_qp];
      break;
    default:
      mooseError("'" << this->name() << "' Invalid equation name.");
  }
}

Real
SaintVenantSetWaterHeight::computeQpJacobian()
{
  return 0.;
}

Real
SaintVenantSetWaterHeight::computeQpOffDiagJacobian(unsigned jvar)
{
  return 0.;
}

