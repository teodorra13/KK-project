#ifndef LLVM_DEADSTOREELIMINATION_H
#define LLVM_DEADSTOREELIMINATION_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace llvm;


class DeadStoreElimination {

private:

    bool Changed = false;

    std::unordered_map<BasicBlock*, std::unordered_set<Value*>> LiveIn;
    std::unordered_map<BasicBlock*, std::unordered_set<Value*>> LiveOut;

    std::vector<BasicBlock*> WorkList;
    std::unordered_set<BasicBlock*> InWorkList;

    void clear();
    void initialize(Function &F);
    void computeLiveness(Function &F);
    bool processBasicBlock(BasicBlock *BB);
    bool isDeadStoreCandidate(StoreInst *SI);
    bool eliminateDeadStores(Function &F);

public:

    bool runOnFunction(Function &F);
};


#endif