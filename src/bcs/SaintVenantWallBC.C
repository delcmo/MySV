#include "SaintVenantWallBC.h"
#include "EquationOfState.h"

/** Set the fluid velocity at the boundary **/

template<>
InputParameters validParams<SaintVenantWallBC>()
{
  InputParameters params = validParams<IntegratedBC>();

  // Equation name:
  params.addParam<std::string>("equ_name", "invalid", "Name of the equation.");
  // Coupled variables
  params.addRequiredCoupledVar("h", "water height");
  params.addRequiredCoupledVar("hu", "x-mom of h*\vec{u}");  
  // Equation of state
  params.addRequiredParam<UserObjectName>("eos", "The name of equation of state object to use.");

  return params;
}


SaintVenantWallBC::SaintVenantWallBC(const std::string & name, InputParameters parameters) :
    IntegratedBC(name, parameters),
    // Equation name
    _equ_type("continuity, x_mom, invalid", getParam<std::string>("equation_name")),
    // Coupled variables
    _h(coupledValue("h")),
    // Equation of state:
    _eos(getUserObject<EquationOfState>("eos")),
    // Integer for jacobian terms
    _h_var(coupled("h")),
    _hu_var(coupled("hu"))
{
  if (_mesh.dimension() > 1)
    mooseError("'" << this->name() << "' can only be used with 1-D mesh since it is deisnged for the Saint-Venant equations.");
}

Real
SaintVenantWallBC::computeQpResidual()
{
  RealVectorValue hU(0., 0., 0.);
  Real p = _eos.pressure(_h[_qp], hU);
  switch (_equ_type)
  {
    case continuity:
      return 0.;
      break;
    case x_mom:
      return p*_normals[_qp](0)*_test[_i][_qp];
      break;
    default:
      mooseError("'" << this->name() << "' Invalid equation name.");
  }
}

Real
SaintVenantWallBC::computeQpJacobian()
{
  return 0.;
}

Real
SaintVenantWallBC::computeQpOffDiagJacobian(unsigned jvar)
{
  return 0.;
}

