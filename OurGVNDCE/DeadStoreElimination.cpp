#include "DeadStoreElimination.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/CFG.h"

#include <algorithm>

using namespace llvm;


void DeadStoreElimination::clear(){
    Changed = false;

    LiveIn.clear();
    LiveOut.clear();

    WorkList.clear();
    InWorkList.clear();
}


void DeadStoreElimination::initialize(Function &F){
    for (BasicBlock &BB : llvm::reverse(F)){
        WorkList.push_back(&BB);
        InWorkList.insert(&BB);
    }

    std::reverse(WorkList.begin(), WorkList.end());
}


void DeadStoreElimination::computeLiveness(Function &F){
    while (!WorkList.empty()){
        BasicBlock *BB = WorkList.back();
        WorkList.pop_back();

        InWorkList.erase(BB);

        bool ChangedBlock = processBasicBlock(BB);

        if (ChangedBlock){
            for (BasicBlock *Pred : predecessors(BB)){
                if (InWorkList.find(Pred) == InWorkList.end()){
                    WorkList.push_back(Pred);
                    InWorkList.insert(Pred);
                }
            }
        }
    }
}


bool DeadStoreElimination::processBasicBlock(BasicBlock *BB){
    std::unordered_set<Value*> OldLiveIn = LiveIn[BB];

    std::unordered_set<Value*> NewLiveOut;

    for (BasicBlock *Succ : successors(BB)){
        auto It = LiveIn.find(Succ);

        if (It != LiveIn.end()){
            NewLiveOut.insert(
                It->second.begin(),
                It->second.end()
            );
        }
    }

    LiveOut[BB] = NewLiveOut;

    std::unordered_set<Value*> CurrentLive = NewLiveOut;

    for (auto I = BB->rbegin(); I != BB->rend(); ++I){
        Instruction &Inst = *I;

        if (auto *LI = dyn_cast<LoadInst>(&Inst)){
            CurrentLive.insert(LI->getPointerOperand());
        }

        else if (auto *SI = dyn_cast<StoreInst>(&Inst)){
            CurrentLive.erase(SI->getPointerOperand());
        }
    }

    LiveIn[BB] = CurrentLive;

    return OldLiveIn != CurrentLive;
}


bool DeadStoreElimination::isDeadStoreCandidate(StoreInst *SI){
    if (SI == nullptr){
        return false;
    }

    if (SI->isVolatile()){
        return false;
    }


    if (SI->isAtomic()){
        return false;
    }

    if (SI->getPointerOperand() == nullptr){
        return false;
    }

    return true;
}


bool DeadStoreElimination::eliminateDeadStores(Function &F){
    for (BasicBlock &BB : F){
        std::unordered_set<Value*> LiveMemory = LiveOut[&BB];
        std::unordered_set<Value*> DefinedAfterCall;
        bool AllMemoryLive = false;

        std::vector<StoreInst*> StoresToRemove;

        for (auto I = BB.rbegin(); I != BB.rend(); ++I){
            Instruction &Inst = *I;

            if (auto *LI = dyn_cast<LoadInst>(&Inst)){
                LiveMemory.insert(LI->getPointerOperand());
                continue;
            }

            if (auto *SI = dyn_cast<StoreInst>(&Inst)){
                if (!isDeadStoreCandidate(SI)){
                    continue;
                }

                Value *Ptr = SI->getPointerOperand();

                if (!AllMemoryLive){
                    if(LiveMemory.find(Ptr) == LiveMemory.end()){
                        StoresToRemove.push_back(SI);
                    }
                    else {
                        LiveMemory.erase(Ptr);
                    }
                }
                else {
                    if(LiveMemory.find(Ptr) == LiveMemory.end()){
                        if(DefinedAfterCall.find(Ptr) != DefinedAfterCall.end()){
                            StoresToRemove.push_back(SI);
                        }
                        else
                            DefinedAfterCall.insert(Ptr);
                    }
                    else{
                        if(DefinedAfterCall.find(Ptr) == DefinedAfterCall.end())
                             DefinedAfterCall.insert(Ptr);
    
                        LiveMemory.erase(Ptr);
                    }
                }
                continue;
            }

            if (auto *CI = dyn_cast<CallInst>(&Inst)){
                if (!CI->onlyReadsMemory()){
                    AllMemoryLive = true;
                    LiveMemory.clear();
                    DefinedAfterCall.clear();
                }
            }
        }

        for (StoreInst *SI : StoresToRemove){
            SI->eraseFromParent();
            Changed = true;
        }
    }
    return Changed;
}


bool DeadStoreElimination::runOnFunction(Function &F){
    clear();

    initialize(F);
    computeLiveness(F);
    eliminateDeadStores(F);

    return Changed;
}