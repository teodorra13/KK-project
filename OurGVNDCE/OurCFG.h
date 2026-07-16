#ifndef LLVM_OURCFG_H
#define LLVM_OURCFG_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace llvm;

class OurCFG {
private:
  std::unordered_map<BasicBlock *, std::vector<BasicBlock *>> AdjacencyList;
  std::unordered_set<BasicBlock *> ReachableBlocks;

  void createCFG(Function &F);

public:
  OurCFG(Function &F);

  void DFS(BasicBlock *Current);
  bool isReachable(BasicBlock *BB);
  std::vector<BasicBlock *> getPredecessors(BasicBlock *BB);
};

#endif
