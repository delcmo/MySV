#include "MysvApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"

// kernels
#include "WaterHeightEqu.h"
#include "MomentumEqu.h"
#include "ArtificialDissipativeFlux.h"
#include "PressureBasedViscosityMassMatrix.h"
#include "PressureBasedViscosityLaplace.h"

// auxkernels
#include "Xvelocity.h"
#include "PressureSw.h"

// ics
#include "StepIC.h"

// bcs
#include "SaintVenantSetWaterHeight.h"
#include "SaintVenantSetWaterVelocity.h"
#include "SaintVenantWallBC.h"

// eos
#include "EquationOfState.h"
#include "HydrostaticPressure.h"

// materials
#include "EntropyViscosityCoefficient.h"
#include "LapidusViscosityCoefficient.h"
#include "PressureBasedViscosityCoefficient.h"

// functions
#include "VariableDepthRiverbed.h"

template<>
InputParameters validParams<MysvApp>()
{
  InputParameters params = validParams<MooseApp>();

  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  return params;
}

MysvApp::MysvApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  MysvApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  MysvApp::associateSyntax(_syntax, _action_factory);
}

MysvApp::~MysvApp()
{
}

void
MysvApp::registerApps()
{
  registerApp(MysvApp);
}

void
MysvApp::registerObjects(Factory & factory)
{
  // kernels
  registerKernel(WaterHeightEqu);
  registerKernel(MomentumEqu);
  registerKernel(ArtificialDissipativeFlux);
  registerKernel(PressureBasedViscosityMassMatrix);
  registerKernel(PressureBasedViscosityLaplace);

  // auxkernels
  registerAux(Xvelocity);
  registerAux(PressureSw);

  // ics
  registerInitialCondition(StepIC);

  // bcs
  registerBoundaryCondition(SaintVenantSetWaterHeight);
  registerBoundaryCondition(SaintVenantSetWaterVelocity);
  registerBoundaryCondition(SaintVenantWallBC);

  // eos
  registerUserObject(EquationOfState);
  registerUserObject(HydrostaticPressure);

  // materials
  registerMaterial(EntropyViscosityCoefficient);
  registerMaterial(LapidusViscosityCoefficient);
  registerMaterial(PressureBasedViscosityCoefficient);

  // functions
  registerFunction(VariableDepthRiverbed);
}

void
MysvApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
