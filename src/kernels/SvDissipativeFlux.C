/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "SvDissipativeFlux.h"
/**
This function computes the dissipative terms for all of the equations. It is dimension agnostic.
 */
template<>
InputParameters validParams<SvDissipativeFlux>()
{
  InputParameters params = validParams<Kernel>();

  // Equation name:
  params.addParam<std::string>("equ_name", "invalid", "Name of the equation.");

  return params;
}

SvDissipativeFlux::SvDissipativeFlux(const std::string & name,
                       InputParameters parameters) :
  Kernel(name, parameters),
    // Equation name
    _equ_type("continuity, x_mom, y_mom, invalid", getParam<std::string>("equation_name")),
    // Material
    _kappa(getMaterialProperty<Real>("kappa"))
{}

Real SvDissipativeFlux::computeQpResidual()
{
  if (_mesh.isBoundaryNode(_current_elem->node(_i))==true)
  {
    switch (_equ_type)
    {
      case continuity:
      return _kappa[_qp]*_grad_u[_qp]*_grad_test[_i][_qp];
      break;
    case x_mom:
      return _kappa[_qp]*_grad_u[_qp]*_grad_test[_i][_qp];
      break;
    case y_mom:
      return _kappa[_qp]*_grad_u[_qp]*_grad_test[_i][_qp];
      break;
    default:
      mooseError("Invalid equation name.");
    }
  }
  else
  return 0.;
}

Real SvDissipativeFlux::computeQpJacobian()
{
  return _kappa[_qp]*_grad_phi[_j][_qp]*_grad_test[_i][_qp];
}

Real SvDissipativeFlux::computeQpOffDiagJacobian( unsigned int _jvar)
{
  return 0.;
}
