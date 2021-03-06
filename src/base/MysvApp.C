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
#include "EnergySw.h"
#include "EnergyFluxSw.h"

// ics
#include "StepIC.h"

// bcs
#include "SaintVenantSetWaterHeightInletBC.h"
#include "SaintVenantSetWaterHeightOutletBC.h"
#include "SaintVenantSetWaterVelocityInletBC.h"
#include "SaintVenantSetWaterVelocity.h"
#include "SolidWallBC.h"

// eos
#include "EquationOfState.h"
#include "HydrostaticPressure.h"

// materials
#include "EntropyViscosityCoefficient.h"
#include "LapidusViscosityCoefficient.h"
#include "PressureBasedViscosityCoefficient.h"

// posprocessors
#include "TimeStepCFL.h"

// functions
#include "VariableDepthRiverbed1D.h"
#include "VariableDepthRiverbed2D.h"

// userobjects
#include "JumpInterface.h"

template<>
InputParameters validParams<MysvApp>()
{
  InputParameters params = validParams<MooseApp>();

  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  return params;
}

MysvApp::MysvApp(InputParameters parameters) :
    MooseApp(parameters)
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
  registerAux(EnergySw);
  registerAux(EnergyFluxSw);

  // ics
  registerInitialCondition(StepIC);

  // bcs
  registerBoundaryCondition(SaintVenantSetWaterHeightInletBC);
  registerBoundaryCondition(SaintVenantSetWaterHeightOutletBC);
  registerBoundaryCondition(SaintVenantSetWaterVelocityInletBC);
  registerBoundaryCondition(SaintVenantSetWaterVelocity);
  registerBoundaryCondition(SolidWallBC);

  // eos
  registerUserObject(EquationOfState);
  registerUserObject(HydrostaticPressure);

  // materials
  registerMaterial(EntropyViscosityCoefficient);
  registerMaterial(LapidusViscosityCoefficient);
  registerMaterial(PressureBasedViscosityCoefficient);

  // postprocessors
  registerPostprocessor(TimeStepCFL);

  // functions
  registerFunction(VariableDepthRiverbed1D);
  registerFunction(VariableDepthRiverbed2D);

  // userobjects
  registerUserObject(JumpInterface);
}

void
MysvApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
