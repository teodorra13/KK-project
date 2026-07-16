#include "GlobalValueNumbering.h"

// Blok A dominira blokom B ako svaki put od ulaza funkcije do B prolazi kroz A
// Svaki dostizan blok dominira samim sobom
// dominacija nije isto sto i redosled blokova u .ll fajlu : ona zavisi od svih mogucih putanja izvrsavanja



void GlobalValueNumbering::initializeDominators(Function &F, OurCFG &CFG) {

  BasicBlock *Entry = &F.front();

  for (BasicBlock &BB : F) {
        // nedostizni  blokovi se preskacu da sopstvena analiza dominacije ne bi
        // izvlacila zakljucke nad cvorovima koji nisu deo regularno toka izvrsavanja
        if (!CFG.isReachable(&BB)) {
          continue;
        }
        if (&BB == Entry) {
          Dominators[&BB].insert(&BB);
        }
        else {
            for (BasicBlock &Candidate : F) {
                if (CFG.isReachable(&Candidate)) {
                  Dominators[&BB].insert(&Candidate);
                }
            }
        }
  }

}

bool GlobalValueNumbering::areTheSameSets( std::unordered_set<BasicBlock *> &First, std::unordered_set<BasicBlock *> &Second) {
  if (First.size() != Second.size()) {
    return false;
  }

  for (BasicBlock *BB : First) {
    if (Second.find(BB) == Second.end()) {
      return false;
    }
  }

  return true;
}

void GlobalValueNumbering::calculateDominators(Function &F, OurCFG &CFG) {
  BasicBlock *Entry = &F.front();
  bool Changed;

  do {
      Changed = false;

      for (BasicBlock &BB : F) {
          if (&BB == Entry || !CFG.isReachable(&BB)) { //Dom(entry) je konacan,a nedostizne ne analiziramo
            continue;
          }

          std::unordered_set<BasicBlock *> NewDominators;
          bool FirstPredecessor = true;

          for (BasicBlock *Predecessor : CFG.getPredecessors(&BB)) {
              if (!CFG.isReachable(Predecessor)) {
                continue;
              }

              if (FirstPredecessor) { // Kopiramo skup prvog dostiznog prethodnika i to je pocetna vresnost lokalnog preseka
                for (BasicBlock *Dominator : Dominators[Predecessor]) {
                  NewDominators.insert(Dominator);
                }
                FirstPredecessor = false;
              }
              else {
                std::unordered_set<BasicBlock *> Intersection;

                for (BasicBlock *Dominator : NewDominators) {
                  if (Dominators[Predecessor].find(Dominator) !=  Dominators[Predecessor].end()) { // preseci sa skupom svakog sledeceg prethodnika jer dominator mora biti prisutan na svim ulaznim outanjama
                    Intersection.insert(Dominator);
                  }
                }

                NewDominators = Intersection;
              }
          }

          NewDominators.insert(&BB);

          if (!areTheSameSets(Dominators[&BB], NewDominators)) {
            Dominators[&BB] = NewDominators;
            Changed = true;
          }
      }
  } while (Changed);
}

bool GlobalValueNumbering::dominates(Instruction *First, Instruction *Second) {
    if (First->getParent() == Second->getParent()) {
      return true;
    }

    return Dominators[Second->getParent()].find(First->getParent()) !=
      Dominators[Second->getParent()].end();
}

int GlobalValueNumbering::getValueNumber(Value *V) {
  // dve razlicite Value* instane smeju dobiti isti broj tek kada je algoritam dokazao ekvivalentnost
  if (ValueNumbers.find(V) == ValueNumbers.end()) {
    ValueNumbers[V] = NextValueNumber++;
  }

  return ValueNumbers[V];
}

bool GlobalValueNumbering::isInteresting(Instruction *I) {
  return isa<AddOperator>(I) || isa<SubOperator>(I) ||
    isa<MulOperator>(I) || isa<SDivOperator>(I) ||
    isa<UDivOperator>(I) || isa<ICmpInst>(I);
}

bool GlobalValueNumbering::isTheTypeSame(Instruction *First, Instruction *Second) {

    if (First->getOpcode() != Second->getOpcode() || First->getType() != Second->getType()) {
      return false;
    }

    if (isa<ICmpInst>(First) && isa<ICmpInst>(Second)) {
      ICmpInst *FirstCompare = dyn_cast<ICmpInst>(First);
      ICmpInst *SecondCompare = dyn_cast<ICmpInst>(Second);

      return  FirstCompare->getPredicate() == SecondCompare->getPredicate() &&
              First->getOperand(0)->getType() == Second->getOperand(0)->getType() &&
              First->getOperand(1)->getType() == Second->getOperand(1)->getType();
    }

  return true;
}

bool GlobalValueNumbering::isConstantInt(Value *V) {
  return isa<ConstantInt>(V);
}

bool GlobalValueNumbering::areTheSameConstants(Value *First, Value *Second) { // poredi tip i konkretne vrednosti konstantee
  ConstantInt *FirstConstant = dyn_cast<ConstantInt>(First);
  ConstantInt *SecondConstant = dyn_cast<ConstantInt>(Second);

  return First->getType() == Second->getType() &&
    FirstConstant->getSExtValue() == SecondConstant->getSExtValue();
}

bool GlobalValueNumbering::isTheSameOperand(Value *First, Value *Second) {
  if (isConstantInt(First) && isConstantInt(Second)) {
    return areTheSameConstants(First, Second);
  }

  if (isConstantInt(First) || isConstantInt(Second)) {
    return false;
  }

  return getValueNumber(First) == getValueNumber(Second);
}

bool GlobalValueNumbering::haveTheSameOperands(Instruction *First, Instruction *Second) {


  return isTheSameOperand(First->getOperand(0), Second->getOperand(0)) &&
    isTheSameOperand(First->getOperand(1), Second->getOperand(1));
}


// komutativne su add, mul, icmp eq, icmp ne -> za njih se proverava isti i zemenjeni redosled operanada
// sub sdiv udiv i uredjena porednjenja nisu komutativna -> TU ZAHTEVAMMO ISTI REDOSLED

bool GlobalValueNumbering::isCommutative(Instruction *I) {
  if (isa<ICmpInst>(I)) {
    ICmpInst *Compare = dyn_cast<ICmpInst>(I);
    CmpInst::Predicate Predicate = Compare->getPredicate();

    return Predicate == ICmpInst::ICMP_EQ ||
      Predicate == ICmpInst::ICMP_NE;
  }

  return isa<AddOperator>(I) || isa<MulOperator>(I);
}

bool GlobalValueNumbering::haveTheSameOperandsCommutative( Instruction *First, Instruction *Second) {

  return (isTheSameOperand(First->getOperand(0), Second->getOperand(0)) &&
        isTheSameOperand(First->getOperand(1), Second->getOperand(1))) ||
    (isTheSameOperand(First->getOperand(0), Second->getOperand(1)) &&
     isTheSameOperand(First->getOperand(1), Second->getOperand(0)));
}

bool GlobalValueNumbering::isTheSameInstruction(Instruction *First, Instruction *Second) {
  if (!isTheTypeSame(First, Second)) {
    return false;
  }

  if (isCommutative(First)) {
    return haveTheSameOperandsCommutative(First, Second);
  }

  return haveTheSameOperands(First, Second);
}

Instruction *GlobalValueNumbering::alreadySaved(Instruction *I) {
  for (Instruction *SavedInstruction : SavedInstructions) {
    if (dominates(SavedInstruction, I) &&
        isTheSameInstruction(SavedInstruction, I)) {
      return SavedInstruction;
    }
  }

  return nullptr;
}

void GlobalValueNumbering::removeRedundantInstructions() {
  for (Instruction *I : InstructionsToRemove) {
    I->eraseFromParent();
  }
}

bool GlobalValueNumbering::runOnFunction(Function &F) {
  ValueNumbers.clear();
  SavedInstructions.clear();
  InstructionsToRemove.clear();
  Dominators.clear();
  NextValueNumber = 1;

  OurCFG CFG(F);
  CFG.DFS(&F.front());
  initializeDominators(F, CFG);
  calculateDominators(F, CFG);

  bool Changed = false;

  for (BasicBlock &BB : F) {
    if (!CFG.isReachable(&BB)) {
      continue;
    }

    for (Instruction &I : BB) {
      if (isInteresting(&I)) {
        if (Instruction *SavedInstruction = alreadySaved(&I)) {
          ValueNumbers[&I] = getValueNumber(SavedInstruction);
          I.replaceAllUsesWith(SavedInstruction);
          InstructionsToRemove.push_back(&I);
          Changed = true;
        }
        else {
          getValueNumber(&I);
          SavedInstructions.push_back(&I);
        }
      }
      else {
        getValueNumber(&I);
      }
    }
  }

  removeRedundantInstructions();
  return Changed;
}
