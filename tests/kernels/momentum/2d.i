[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  xmin = 0.
  xmax = 1.
  ymin = 0.
  ymax = 1.
[]

[Functions]
  [./topology]
    type = ConstantFunction
    value = 0.
  [../]
[]

[UserObjects]
  [./hydro]
    type = HydrostaticPressure
    gravity = 9.8
  [../]
[]

[Variables]
  [./h]
    family = LAGRANGE
    order = first
    [./InitialCondition]
      type = StepIC
      h_left=2.
      h_right=1.
      radius=0.1
      x_source=0.5
      y_source=0.5      
    [../]
  [../]

  [./hu]
    family = LAGRANGE
    order = first  
    [./InitialCondition]
    type = ConstantIC
    value = 0.
    [../]
  [../]
  
  [./hv]
    family = LAGRANGE
    order = first  
    [./InitialCondition]
    type = ConstantIC
    value = 0.
    [../]
  [../]
[]

[Kernels]
  # Water height equation
  [./TimeMass]
    type = TimeDerivative
    variable = h
  [../]

  [./Mass]
    type = WaterHeightEqu
    variable = h
    hu = hu
    hv = hv
  [../]
  
  [./MassDissip]
    type = ArtificialDissipativeFlux
    variable = h
    equ_name = continuity
  [../]

  # x-momentum equation
  [./TimeXMom]
    type = TimeDerivative
    variable = hu
  [../]

  [./XMomentum]
  type = MomentumEqu
  variable = hu
  h = h
  hu = hu
  hv = hv  
  gravity = 9.8
  component = 0
  topology = topology
  eos = hydro
  [../]

  [./XMomDissip]
    type = ArtificialDissipativeFlux
    variable = hu
    equ_name = x_mom
  [../]

  # y-momentum equation  
  [./TimeYMom]
    type = TimeDerivative
    variable = hv
  [../]

  [./YMomentum]
  type = MomentumEqu
  variable = hv
  h = h
  hu = hu
  hv = hv
  gravity = 9.8
  component = 1
  topology = topology
  eos = hydro
  [../]
  
  [./YMomDissip]
    type = ArtificialDissipativeFlux
    variable = hv
    equ_name = y_mom
  [../]
[]

[AuxVariables]
  [./kappa_max_aux]
    family = MONOMIAL
    order = CONSTANT
  [../]
[]

[AuxKernels]
  [./kappa_max_ak]
    type = MaterialRealAux
    variable = kappa_max_aux
    property = kappa_max
  [../]
[]

[Materials]
  [./EntropyViscosityCoeff]
    type = EntropyViscosityCoefficient
    block = 0
    h = h
    hu = hu
    hv = hv    
    eos = hydro
  [../]
[]

[BCs]
  [./bc_h]
    type = DirichletBC
    variable = h
    boundary = 'left right top bottom'
    value = 1.
  [../]

  [./bc_hu]
    type = DirichletBC
    variable = hu
    boundary = 'left right top bottom'
    value = 0.
  [../]
  
  [./bc_hv]
    type = DirichletBC
    variable = hv
    boundary = 'left right top bottom'
    value = 0.
  [../]
[]

[Preconditioning]
  [./FDP]
    type = SMP # FDP
    full = true
    solve_type = 'PJFNK'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  
  dt = 1.e-4

  nl_rel_tol = 1e-12
  nl_abs_tol = 1e-6
  nl_max_its = 30

  num_steps = 10

[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = false
  print_perf_log = true
[]
