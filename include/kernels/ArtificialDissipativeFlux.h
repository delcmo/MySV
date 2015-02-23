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

#ifndef SVDISSIPATIVEFLUX_H
#define SVDISSIPATIVEFLUX_H

#include "Kernel.h"

// Forward Declarations
class SvDissipativeFlux;

template<>
InputParameters validParams<SvDissipativeFlux>();

class SvDissipativeFlux : public Kernel
{
public:

  SvDissipativeFlux(const std::string & name,
             InputParameters parameters);

protected:

  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int _jvar);
    
private:
  
  // Equation type
  enum EquationType
  {
      continuity = 0,
      x_mom = 1,
      y_mom = 2
  };
  MooseEnum _equ_type;

  // Material.
  MaterialProperty<Real> & _kappa;
};

#endif // SVDISSIPATIVEFLUX_H
