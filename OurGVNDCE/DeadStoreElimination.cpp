#include "DeadStoreElimination.h"

void DeadStoreElimination::findLocalVariables(Function &F) {
  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      if (isa<AllocaInst>(&I)) {
        SafeVariables[&I] = true;
      }
    }
  }
}

void DeadStoreElimination::findUnsafeVariables(Function &F) {
  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      for (size_t Index = 0; Index < I.getNumOperands(); Index++) {
        Value *Operand = I.getOperand(Index);

        if (SafeVariables.find(Operand) == SafeVariables.end()) {
          continue;
        }

        bool DirectLoad = isa<LoadInst>(&I) && Index == 0;
        bool DirectStore = isa<StoreInst>(&I) && Index == 1;

        if (!DirectLoad && !DirectStore) {
          SafeVariables[Operand] = false;
        }
      }
    }
  }
}

bool DeadStoreElimination::isSafeVariable(Value *V) {
  return SafeVariables.find(V) != SafeVariables.end() && SafeVariables[V];
}

void DeadStoreElimination::findDeadStores(Function &F) {
  for (BasicBlock &BB : F) {
    std::unordered_map<Value *, StoreInst *> LastStores;

    for (Instruction &I : BB) {
      if (isa<CallInst>(&I)) {
        LastStores.clear();
      }
      else if (StoreInst *Store = dyn_cast<StoreInst>(&I)) {
        Value *Variable = Store->getOperand(1);

        if (!isSafeVariable(Variable)) {
          continue;
        }

        if (LastStores.find(Variable) != LastStores.end() &&
            LastStores[Variable] != nullptr) {
          InstructionsToRemove.push_back(LastStores[Variable]);
        }

        LastStores[Variable] = Store;
      }
      else if (LoadInst *Load = dyn_cast<LoadInst>(&I)) {
        Value *Variable = Load->getOperand(0);

        if (isSafeVariable(Variable)) {
          LastStores[Variable] = nullptr;
        }
      }
    }
  }
}

void DeadStoreElimination::removeDeadStores() {
  for (Instruction *I : InstructionsToRemove) {
    I->eraseFromParent();
  }
}

bool DeadStoreElimination::runOnFunction(Function &F) {
  SafeVariables.clear();
  InstructionsToRemove.clear();

  findLocalVariables(F);
  findUnsafeVariables(F);
  findDeadStores(F);

  bool Changed = !InstructionsToRemove.empty();
  removeDeadStores();
  return Changed;
}
