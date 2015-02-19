[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 50
[]

[Functions]
  [./ic_func]
    axis = 0
    type = PiecewiseLinear
    x = '0  0.4  0.6  1'
    y = '2  2    4    4'
  [../]
[]

[Variables]
  [./h]
  [./InitialCondition]
    type = FunctionIC
    function = ic_func
  [../]
  [../]
[]

[AuxVariables]
  [./hu]
  [./InitialCondition]
    type = ConstantIC
    value = 2.
  [../]
  [../]
[]

[Kernels]
  [./Time]
    type = TimeDerivative
    variable = h
  [../]

  [./Mass]
    type = SvMass
    variable = h
    hu = hu
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = h
    boundary = left
    value = 2.
  [../]

  [./right]
    type = DirichletBC
    variable = h
    boundary = right
    value = 4.
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'

  nl_rel_tol = 1e-12
  nl_abs_tol = 1e-6
  nl_max_its = 30

  num_steps = 10

[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]