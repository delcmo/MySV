#include "SaintVenantSetWaterHeightInletBC.h"
#include "EquationOfState.h"

/** Set the water height at the inlet boundary. This function can only be used with 1-D mesh. 
 This bc function can handle both subsonic and supersonic boundaries. **/

template<>
InputParameters validParams<SaintVenantSetWaterHeightInletBC>()
{
  InputParameters params = validParams<IntegratedBC>();

  // Equation name:
  params.addParam<std::string>("equ_name", "invalid", "Name of the equation.");
  // Coupled variables
  params.addRequiredCoupledVar("hu", "x component of h*vec{u}");
  // Constants and parameters
  params.addRequiredParam<Real>("h_bc", "boundary value of the density/water high h");
  params.addParam<Real>("u_bc", "boundary value of the fluid velocity (only used if fluid becomes supersonic at the inlet.");
  // Equation of state
  params.addRequiredParam<UserObjectName>("eos", "The name of equation of state object to use.");

  return params;
}


SaintVenantSetWaterHeightInletBC::SaintVenantSetWaterHeightInletBC(const InputParameters & parameters) :
    IntegratedBC(parameters),
    // Equation name
    _equ_type("continuity x_mom invalid", getParam<std::string>("equ_name")),
    // Coupled variables
    _hu(coupledValue("hu")),
    // Constants and parameters
    _h_bc(getParam<Real>("h_bc")),
    _u_bc(isCoupled("u_bc") ? getParam<Real>("u_bc") : 0.),
    // Equation of state:
    _eos(getUserObject<EquationOfState>("eos")),
    // Integer for jacobian terms
    _hu_var(coupled("hu"))
{
  // Assert mesh dimension
  if (_mesh.dimension() >= 2)
    mooseError("'" << this->name() << "' can only be used with 1-D mesh since it is designed for the Saint-Venant equations.");
  // Determine whether or not u_bc is specified in the input file
  _u_bc_specified = isCoupled("u_bc") ? true : false;
}

Real
SaintVenantSetWaterHeightInletBC::computeQpResidual()
{
  // Check that the bc is an inlet bc
  Real vel = _hu[_qp]/_h_bc;
  if (vel*_normals[_qp](0)>0)
    mooseError("'" << this->name() << "' is not/no longer an inlet bc: 'vec{u} dot vec{normal}' is greater than zero");

  // Current bc values of the momentum, sound speed and pressure
  RealVectorValue hU(_hu[_qp], 0., 0.);
  Real c2 = _eos.c2(_h_bc, hU);
  Real Mach = std::fabs(vel)/std::sqrt(c2);

  // If the fluid is supersonic u_bc is used to evaluate hU at the boundary
  if (Mach>1)
  {
    if (!_u_bc_specified)
      mooseError("'" << this->name() << "': the fluid becomes supersonic but you did not sepcify an inlet fluid velocity value in the input file.");
    hU(0) = _h_bc*_u_bc;
  }
  Real p = _eos.pressure(_h_bc, hU);

  // Return the value of the bc flux for each equation
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
SaintVenantSetWaterHeightInletBC::computeQpJacobian()
{
  // Precompute some values
  Real vel = _hu[_qp]/_h_bc;
  RealVectorValue hU(_hu[_qp], 0., 0.);
  Real c2 = _eos.c2(_h_bc, hU);
  Real Mach = std::fabs(vel)/std::sqrt(c2);
  Real dpdu;

  // Return jacobian terms
  if (Mach<1)
  {
    switch (_equ_type)
    {
      case x_mom:
        dpdu = _eos.dp_dhu(_h_bc, hU);
        return _phi[_j][_qp]*(2.*_u[_qp]/_h_bc+dpdu)*_normals[_qp](0)*_test[_i][_qp];
        break;
      default:
        return 0.;
    }
  }
  else
    return 0.;
}

Real
SaintVenantSetWaterHeightInletBC::computeQpOffDiagJacobian(unsigned jvar)
{
  // Precompute some values
  Real vel = _hu[_qp]/_h_bc;
  RealVectorValue hU(_hu[_qp], 0., 0.);
  Real c2 = _eos.c2(_h_bc, hU);
  Real Mach = std::fabs(vel)/std::sqrt(c2);

  // Non-zero jacobian term only if subsonic fluid
  if (jvar == _hu_var && Mach < 1)
  {
    switch (_equ_type)
    {
      case continuity:
        return _phi[_j][_qp]*_normals[_qp](0)*_test[_i][_qp];
        break;
      default:
        return 0.;
        break;
    }
  }
  else
    return 0.;
}

