#include "DeadCodeElimination.h"

bool DeadCodeElimination::isCandidate(Instruction *I) {
  return !I->mayHaveSideEffects() && !I->isTerminator();
  // return isa<AddOperator>(I) || isa<SubOperator>(I) ||
  //   isa<MulOperator>(I) || isa<SDivOperator>(I) ||
  //   isa<UDivOperator>(I) || isa<ICmpInst>(I);
}

void DeadCodeElimination::findAllVariables(Function &F) {
  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      if (isCandidate(&I)) {
        Variables[&I] = false;
      }
    }
  }
}

void DeadCodeElimination::findVariableUsages(Function &F) {
  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      for (size_t Index = 0; Index < I.getNumOperands(); Index++) {
        Value *Operand = I.getOperand(Index);

        if (Variables.find(Operand) != Variables.end()) {
          Variables[Operand] = true;
        }
      }
    }
  }
}

bool DeadCodeElimination::eliminateDeadInstructions(Function &F) {
  std::vector<Instruction *> InstructionsToRemove;

  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      if (Variables.find(&I) != Variables.end() && !Variables[&I]) {
        InstructionsToRemove.push_back(&I);
      }
    }
  }

  for (Instruction *I : InstructionsToRemove) {
    I->eraseFromParent();
  }

  return !InstructionsToRemove.empty();
}

bool DeadCodeElimination::runOnFunction(Function &F) {
  bool Changed = false;
  bool InstructionRemoved;

  do {
    Variables.clear();
    findAllVariables(F);
    findVariableUsages(F);
    InstructionRemoved = eliminateDeadInstructions(F);

    if (InstructionRemoved) {
      Changed = true;
    }
  } while (InstructionRemoved);

  return Changed;
}
