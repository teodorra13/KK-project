#ifndef LLVM_DEADSTOREELIMINATION_H
#define LLVM_DEADSTOREELIMINATION_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

#include <unordered_map>
#include <vector>

using namespace llvm;

class DeadStoreElimination {
private:
  std::unordered_map<Value *, bool> SafeVariables;
  std::vector<Instruction *> InstructionsToRemove;

  void findLocalVariables(Function &F);
  void findUnsafeVariables(Function &F);
  bool isSafeVariable(Value *V);
  void findDeadStores(Function &F);
  void removeDeadStores();

public:
  bool runOnFunction(Function &F);
};

#endif
