
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

#define DEBUG_TYPE "bofsan"

// anonymous namespace -> avoid exporting unneeded symbols
namespace {
  struct BOFSan : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    llvm::FunctionCallee StrlenFunc;
    llvm::FunctionCallee ExitFunc;
    llvm::FunctionCallee PutsFunc;
    BOFSan() : FunctionPass(ID) {}

    // Responsible for ensuring the symbol `exit` is available in every module.
    // This function is allowed to modify the global state of the program
    bool doInitialization(Module &M);


	  // This function examines each function individually, it is not
    // allowed to share any state between functions
    bool runOnFunction(Function &F);
  };
}

bool BOFSan::doInitialization(Module &M) {
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
    StrlenFunc = M.getOrInsertFunction(
                                      "strlen",
                                      Type::getInt32Ty(context),        // returns an integer
                                      PointerType::getUnqual(IntegerType::getInt8Ty(context)) // arg: char * (i.e., int8*)
    );
    PutsFunc = M.getOrInsertFunction(
                                      "puts",                           // name of function
                                      Type::getInt32Ty(context),        // returns an integer
                                       PointerType::getUnqual(IntegerType::getInt8Ty(context))         // arg: char* (int8t)
                                    );

    return true;
}

bool BOFSan::runOnFunction(Function &F) {
  errs() << "Visiting function " << F.getName() << "\n";
  SmallVector<Instruction *, 0> allInstructions;
  allInstructions.reserve(F.getInstructionCount());
  // Loop over each block in the function
  for (auto &I : instructions(F)){
    allInstructions.push_back(&I);
  }
  for(Instruction * I : allInstructions){
    // Check if it's a call isntruction
    if (CallInst *CI = dyn_cast<CallInst>(I)) {
      // If we have debug information, record the source location of each call.
      if (DILocation *Loc = I->getDebugLoc()) {
        unsigned Line = Loc->getLine();
        StringRef File = Loc->getFilename();
        StringRef Dir = Loc->getDirectory();
        errs() << "Call instruction at " << Dir << "/" << File << ":" << Line << "\n";
      }
      // If we can get the name of the function being called, examine and possibly modify it
      if (Function *calledFunction = CI->getCalledFunction()) {
        //errs() << "Function: " << calledFunction->getName() << "\n";
        if (calledFunction->getName() == "strcpy") {

          // To get a Value* for the x in puts(x) , we need to examine the operands to the instruction
          // instead of iterating through the Function arguments directly, otherwise LLVM gets mad
          Value* dest_str = I->getOperand(0); // this is the dest string
          Value* src_str  = I->getOperand(1); // this is the source string
          // Create an IR Builder object and use it to insert some function calls just before
          // the current instruction
          IRBuilder<> builder(I);
          Value *strPtr1 = builder.CreateGlobalStringPtr(StringRef("[BoFSan] StrCpy seems fine\n"));
          Value *strPtr2 = builder.CreateGlobalStringPtr(StringRef("[BoFSan] There will be a BoF\n"));
		
		  // get the lengths
          CallInst* dest_len = builder.CreateCall(StrlenFunc, dest_str);
          CallInst* src_len = builder.CreateCall(StrlenFunc, src_str);

          /*
          *  TODO: build calls to the strlen Function with the 2 operands, and get their results
          */

          /*
           *  TODO: comare their lengths with ICmpSGT (results from the TODO above)
           */ 
          //Instruction* len_check = cast<Instruction>(builder.CreateICmpSGT(,));
			Instruction *len_check = cast<Instruction>(builder.CreateICmpSGT(src_len, dest_len, "len_check"));

		

          Instruction* splitBefore = len_check->getNextNode();
          Instruction *argExpressionBlock = SplitBlockAndInsertIfThen(
                    len_check,splitBefore,false);
          
          builder.SetInsertPoint(CI);
          
          // ok 
          builder.CreateCall(PutsFunc, {strPtr1});
          /* 
            TODO: here we are at "everything is fine" basic block (i.e., right before the call to the strlen)
            build a call to puts function to print that everything is all right (i.e., strPtr1)
          */
          builder.SetInsertPoint(argExpressionBlock);
          /*
            TODO: here we are at the error handling basic block, which means, according to the len check,
            a BOF will happen, print the info (i.e., strPtr2) and call to exit function.
          */
          
          // not ok and exit
           builder.CreateCall(PutsFunc, {strPtr2});
		builder.CreateCall(ExitFunc, {ConstantInt::get(Type::getInt32Ty(F.getContext()), 1)});
          
        }
      }
    }
  }

  if(verifyFunction(F, &errs())){
        errs()<<F<<'\n';
        llvm_unreachable("[BoFSan] produced invalid bitcode");
    }
  return false;
}



char BOFSan::ID = 0;
static RegisterPass<BOFSan>
Y("bofsan", "Simple BoF Sanitizer Pass");
