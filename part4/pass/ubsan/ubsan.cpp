
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Verifier.h"
using namespace llvm;

#define DEBUG_TYPE "ubsan"

// anonymous namespace -> avoid exporting unneeded symbols
namespace {
  struct UBSan : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    llvm::FunctionCallee StrlenFunc;
    llvm::FunctionCallee ExitFunc;
    llvm::FunctionCallee PutsFunc;
    UBSan() : FunctionPass(ID) {}

    // Responsible for ensuring the symbol `exit` is available in every module.
    // This function is allowed to modify the global state of the program
    bool doInitialization(Module &M);


	  // This function examines each function individually, it is not
    // allowed to share any state between functions
    bool runOnFunction(Function &F);
  };
}

bool UBSan::doInitialization(Module &M) {
    // https://llvm.org/docs/WritingAnLLVMPass.html#the-doinitialization-module-method
    errs() << "Initialize our pass for the current module\n";
    // These messages printed to errs() are printed when our pass runs, *not* when the
    // resulting binary is run

    LLVMContext &context = M.getContext();
    ExitFunc = M.getOrInsertFunction(
                                      "exit",                         // name of function
                                      Type::getVoidTy(context),       // return type
                                      Type::getInt32Ty(context)       // first arg: int32
                                    );

    PutsFunc = M.getOrInsertFunction(
                                      "puts",                           // name of function
                                      Type::getInt32Ty(context),        // returns an integer
                                       PointerType::getUnqual(IntegerType::getInt8Ty(context))         // arg: char* (int8t)
                                    );

    return true;
}

bool UBSan::runOnFunction(Function &F) {
  errs() << "Visiting function " << F.getName() << "\n";
  SmallVector<Instruction *, 0> allInstructions;
  allInstructions.reserve(F.getInstructionCount());
  // Loop over each block in the function
  for (auto &I : instructions(F)){
    allInstructions.push_back(&I);
  }
  for(Instruction * I : allInstructions){
    // Check if it's a call isntruction
    if(BinaryOperator* bI = dyn_cast<BinaryOperator>(I)){
        if(bI->getOpcode() == Instruction::Add){
            if (DILocation *Loc = bI->getDebugLoc()) {
                unsigned Line = Loc->getLine();
                StringRef File = Loc->getFilename();
                StringRef Dir = Loc->getDirectory();
                errs() << "Add instruction at " << Dir << "/" << File << ":" << Line << "\n";
            }

            Value* op1 = I->getOperand(0); // left operand of the +
            Value* op2  = I->getOperand(1); // right operand of the + 
            IRBuilder<> builder(I);
            Value *strPtr1 = builder.CreateGlobalStringPtr(StringRef("[UBSan] add instruction seems fine\n"));
            Value *strPtr2 = builder.CreateGlobalStringPtr(StringRef("[UBSan] There will be a UB\n"));
            assert(op1->getType() == builder.getInt32Ty()); // make sure it's 32-bit integer (you can take this for granted)
            assert(op2->getType() == builder.getInt32Ty()); // make sure it's 32-bit integer (you can take this for granted)

            /*
              TODO: sign extent op1 and op2 to 64 bit
              and add the extented op together
            */
            // this is the max value of 32-bit signed integer, as 64-bit signed integer.
            Value* cap = ConstantInt::get(builder.getInt64Ty(), maxIntN(32));

            /*
              TODO: compare if result is greater than the cap
            */
            Instruction* check = cast<Instruction>(builder.CreateICmpSGT(,));

            Instruction* splitBefore = check->getNextNode();
            Instruction *argExpressionBlock = SplitBlockAndInsertIfThen(
                    check,splitBefore,false);

            builder.SetInsertPoint(splitBefore);
            /* 
              TODO: here we are at "everything is fine" basic block (i.e., right before the call to the add operat ion)
              build a call to puts function to print that everything is all right (i.e., strPtr1)
             */
            builder.SetInsertPoint(argExpressionBlock);
            /*
            TODO: here we are at the error handling basic block, which means, according to the comparison check,
            a undefined behavior will happen, print the info (i.e., strPtr2) and call to exit function.
          */
        }
    }
  }

  if(verifyFunction(F, &errs())){
        errs()<<F<<'\n';
        llvm_unreachable("[UBSan] produced invalid bitcode");
    }
  return false;
}



char UBSan::ID = 0;
static RegisterPass<UBSan>
Y("ubsan", "Simple Undefined Behavior Sanitizer Pass");
