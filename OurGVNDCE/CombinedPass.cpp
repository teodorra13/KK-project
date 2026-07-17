#include "DeadCodeElimination.h"
#include "DeadStoreElimination.h"
#include "GlobalValueNumbering.h"

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"

using namespace llvm;

namespace {

struct OurGlobalValueNumberingPass : public FunctionPass {
  static char ID;
  OurGlobalValueNumberingPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    GlobalValueNumbering GVN;
    return GVN.runOnFunction(F);
  }
};

struct OurDeadStoreEliminationPass : public FunctionPass {
  static char ID;
  OurDeadStoreEliminationPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    DeadStoreElimination DSE;
    return DSE.runOnFunction(F);
  }
};

struct OurDeadCodeEliminationPass : public FunctionPass {
  static char  ID;
  OurDeadCodeEliminationPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    DeadCodeElimination DCE;
    return DCE.runOnFunction(F);
  }
};

struct OurCombinedPass : public FunctionPass {
  static char ID;
  OurCombinedPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    bool Changed = false;
    bool IterationChanged;

    do {
      IterationChanged = false;

      GlobalValueNumbering GVN;
      if (GVN.runOnFunction(F)) {
        IterationChanged = true;
        Changed = true;
      }

      DeadStoreElimination DSE;
      if (DSE.runOnFunction(F)) {
        IterationChanged = true;
        Changed = true;
      }

      DeadCodeElimination DCE;
      if (DCE.runOnFunction(F)) {
        IterationChanged = true;
        Changed = true;
      }
    } while (IterationChanged);

    return Changed;
  }
};

} // namespace

char OurGlobalValueNumberingPass::ID = 0;
char OurDeadStoreEliminationPass::ID = 0;
char OurDeadCodeEliminationPass::ID = 0;
char OurCombinedPass::ID = 0;

static RegisterPass<OurGlobalValueNumberingPass>
  X("our-gvn", "Our global value numbering pass");

static RegisterPass<OurDeadStoreEliminationPass>
  Y("our-dse", "Our dead store elimination pass");

static RegisterPass<OurDeadCodeEliminationPass>
  Z("our-dce", "Our dead code elimination pass");

static RegisterPass<OurCombinedPass>
  W("our-gvn-dse-dce", "Our combined GVN, DSE and DCE pass");
