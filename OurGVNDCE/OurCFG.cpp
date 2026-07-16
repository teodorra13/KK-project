#include "OurCFG.h"

OurCFG::OurCFG(Function &F) {
  createCFG(F);
}

void OurCFG::createCFG(Function &F) {
  for (BasicBlock &BB : F) {
    AdjacencyList[&BB];

    for (Instruction &I : BB) {

      if (BranchInst *Branch = dyn_cast<BranchInst>(&I)) {

          AdjacencyList[&BB].push_back(Branch->getSuccessor(0));
          if (Branch->isConditional()) {
            AdjacencyList[&BB].push_back(Branch->getSuccessor(1));
          }
      }
    }
  }
}

void OurCFG::DFS(BasicBlock *Current) {
  ReachableBlocks.insert(Current);

  for (BasicBlock *Successor : AdjacencyList[Current]) {
    if (ReachableBlocks.find(Successor) == ReachableBlocks.end()) {
      DFS(Successor);
    }
  }
}

bool OurCFG::isReachable(BasicBlock *BB) {
  return ReachableBlocks.find(BB) != ReachableBlocks.end();
}

std::vector<BasicBlock *> OurCFG::getPredecessors(BasicBlock *BB) {

  std::vector<BasicBlock *> Predecessors;
  //svaki blok ciji skup sledbenika sadrzi BB prethodnik je BB
  for (auto &Pair : AdjacencyList) {
    for (BasicBlock *Successor : Pair.second) {
      if (Successor == BB) {
        Predecessors.push_back(Pair.first);
      }
    }
  }

  return Predecessors;
}
