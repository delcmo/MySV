#include "MysvApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"

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
}

void
MysvApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
