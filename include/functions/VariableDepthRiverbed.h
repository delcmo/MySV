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

#ifndef VARIABLERIVERBED_H
#define VARIABLERIVERBED_H

#include "Function.h"

class VariableDepthRiverbed;

template<>
InputParameters validParams<VariableDepthRiverbed>();

class VariableDepthRiverbed : public Function
{
public:
  VariableDepthRiverbed(const std::string & name, InputParameters parameters);

  virtual Real value(Real t, const Point & p);

  virtual RealVectorValue gradient(Real t, const Point & p);

protected:
  // x-boundaries
  Real _xmin;
  Real _xmax;
};

#endif // VARIABLERIVERBED_H
