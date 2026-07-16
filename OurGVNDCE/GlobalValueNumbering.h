#ifndef LLVM_GLOBALVALUENUMBERING_H
#define LLVM_GLOBALVALUENUMBERING_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Operator.h"

#include "OurCFG.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace llvm;

class GlobalValueNumbering {
private:
  std::unordered_map<Value *, int> ValueNumbers;
  std::vector<Instruction *> SavedInstructions;
  std::vector<Instruction *> InstructionsToRemove;
  std::unordered_map<BasicBlock *, std::unordered_set<BasicBlock *>> Dominators;
  int NextValueNumber;

  void initializeDominators(Function &F, OurCFG &CFG);
  void calculateDominators(Function &F, OurCFG &CFG);
  bool areTheSameSets(std::unordered_set<BasicBlock *> &First, std::unordered_set<BasicBlock *> &Second);
  bool dominates(Instruction *First, Instruction *Second);

  int getValueNumber(Value *V);
  bool isInteresting(Instruction *I);
  bool isTheTypeSame(Instruction *First, Instruction *Second);
  bool isConstantInt(Value *V);
  bool areTheSameConstants(Value *First, Value *Second);
  bool isTheSameOperand(Value *First, Value *Second);
  bool haveTheSameOperands(Instruction *First, Instruction *Second);
  bool isCommutative(Instruction *I);
  bool haveTheSameOperandsCommutative(Instruction *First,  Instruction *Second);
  bool isTheSameInstruction(Instruction *First, Instruction *Second);
  Instruction *alreadySaved(Instruction *I);
  void removeRedundantInstructions();

public:
  bool runOnFunction(Function &F);
};

#endif
