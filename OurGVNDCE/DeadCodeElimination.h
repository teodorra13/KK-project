#ifndef LLVM_DEADCODEELIMINATION_H
#define LLVM_DEADCODEELIMINATION_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Operator.h"

#include <unordered_map>
#include <vector>

using namespace llvm;

class DeadCodeElimination {
private:
  std::unordered_map<Value *, bool> Variables;

  bool isCandidate(Instruction *I);
  void findAllVariables(Function &F);
  void findVariableUsages(Function &F);
  bool eliminateDeadInstructions(Function &F);

public:
  bool runOnFunction(Function &F);
};

#endif