add_llvm_library(LLVMOurGVNDCE MODULE
  OurCFG.cpp
  GlobalValueNumbering.cpp
  DeadStoreElimination.cpp
  DeadCodeElimination.cpp
  CombinedPass.cpp

  PLUGIN_TOOL
  opt
)
