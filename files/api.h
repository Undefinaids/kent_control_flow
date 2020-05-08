// Usage notes
// This API is still somewhat work in progress, so things may be broken.
// TODO: revise if we feel more certain later?
// Lots of the information here is derived from reading QEMU documentation, comments, and source code.
// As a result, it's possible there are misunderstandings in it.
//
// These data-structures are designed to hold the disassembly of some machine code given as input.
// The disasembled code is represented using QEMU's TCG intermediate language.
// A disasembled program will be represented as an array of libTCGOp.
//
// Sequences of ops are divided up by two types of markers.
// - libTCG_OP_SET_LABEL explicitly labelled jump destinations for conditionals (see below)
// - libTCG_OP_INSTRUCTION_START at the start of each instruction that was disassembled
// - libTCG_OP_BLOCK_START at the start of each "translation block"
//
// Translation blocks are marked to allow jumping to specific addresses.
// Each translation block marker includes the address in the input binary that it corresponds to.
// A translation block will end when a (conditional) jump instruction is disasembled.
// libTCG_OP_EXIT_BLOCK will appear to signal that execution should continue elsewhere.
// The destination of a jump can be determined from the contents of the program counter register.
// Beware that translation blocks may begin with an architecture specific pre-amble.
// These pre-ambles are used to handle interrupts in ARM, and don't correspond to actual input instructions.
//
// Conditionals are implemented by jumping within translation blocks (see libTCG_OP_BRANCH_CONDITIONAL).
// These jumps are always to labels, the ID numbers of which are unique within each translation block.
//
// Data is stored by variables (see libTCGArg), which have one of three scopes:
// - temporary
// - local
// - global
// Temporary and local exist only within a translation block, and are identified by unique ID numbers.
// QEMU's documentation doesn't make it very clear how the two are different.
// TODO: sanity check following statement?
// Allowing their scopes to be larger than usual shouldn't change the behaviour of TCG programs.
// Global variables are never deleted, and are used to represent CPU registers and state.
// Each global has a name, usually the name of the register it is equivalent to.
//
// Some TCG ops access memory.
// These assume that two address spaces exist:
// - guest, the emulated machine
// - host, the machine doing the emulating
// TCG variables exist in host memory, and may be accessed that way (which can cause aliasing issues).
// TODO: sanity check two following statements?
// Host accesses are used to access some QEMU datastructures (like the CPU state).
// Guest accesses are used to do normal computations, like accessing the heap and stack.

#define libTCG_NAME_LEN 32

// Unique ID of a label, used for short-range branches.
typedef unsigned libTCGLabelID;

// libTCGArg describes a TCG variable.
// These are used where an op takes a variable as an input, or returns it as an output.
// libTCGArgType is used to show which member of the union is valid.
typedef enum {
	libTCG_ARG_GLOBAL,
	libTCG_ARG_LOCAL,
	libTCG_ARG_TEMPORARY,
} libTCGArgType;
typedef union {
	libTCGArgType type;
	struct {
		libTCGArgType type;
		char name[libTCG_NAME_LEN]; // these include null terminators
		bool is_pc; // true if this variable is the program counter register
	} global;
	struct {
		libTCGArgType type;
		int id;
	} local;
	struct {
		libTCGArgType type;
		int id;
	} temporary;
} libTCGArg;

// Gives the bit-width of the variables a TCG op expects.
typedef enum {
	libTCG_WIDTH_32BIT,
	libTCG_WIDTH_64BIT,
} libTCGWidth;

// Special arguments for various types of op follow...

// Gives the width of data in host-memory 
typedef enum {
	libTCG_HOST_ACCESS_8BIT,
	libTCG_HOST_ACCESS_16BIT,
	libTCG_HOST_ACCESS_32BIT,
	libTCG_HOST_ACCESS_64BIT,
} libTCGHostAccessWidth;

// Tpyes of comparison used by conditional operations.
typedef enum {
	libTCG_CONDITION_NEVER,
	libTCG_CONDITION_ALWAYS,
	// equal, not-equal
	libTCG_CONDITION_EQ, // equal
	libTCG_CONDITION_NE, // not equal
	// signed comparisons:
	libTCG_CONDITION_LTS, // less-than, signed
	libTCG_CONDITION_GES, // greater-equal, signed
	libTCG_CONDITION_LES, // less-equal, signed
	libTCG_CONDITION_GTS, // greater-then, signed
	// unsigned comparisons:
	libTCG_CONDITION_LTU, // less-than, unsigned
	libTCG_CONDITION_GEU, // greater-equal, unsigned
	libTCG_CONDITION_LEU, // less-equal, unsigned
	libTCG_CONDITION_GTU, // greater-than, unsigned
} libTCGCondition;

// Arithmetic operators that take two arguments (see libTCG_OP_BINARY_ARITHMETIC)
typedef enum {
	libTCG_BINARY_ADD,
	libTCG_BINARY_SUB,
	libTCG_BINARY_MUL,
	libTCG_BINARY_MULUH,
	libTCG_BINARY_MULSH,
	libTCG_BINARY_DIV,
	libTCG_BINARY_DIVU,
	libTCG_BINARY_REM,
	libTCG_BINARY_REMU,
	libTCG_BINARY_AND,
	libTCG_BINARY_OR,
	libTCG_BINARY_XOR,
	libTCG_BINARY_SHL,
	libTCG_BINARY_SHR,
	libTCG_BINARY_SAR,
	libTCG_BINARY_ROTL,
	libTCG_BINARY_ROTR,
	libTCG_BINARY_ANDC,
	libTCG_BINARY_ORC,
	libTCG_BINARY_EQV,
	libTCG_BINARY_NAND,
	libTCG_BINARY_NOR,
	libTCG_BINARY_COUNT_LEADING_ZEROES,
	libTCG_BINARY_COUNT_TRAILING_ZEROES,
} libTCGBinaryOp;

typedef enum {
	libTCG_BINARY2_ADD,
	libTCG_BINARY2_SUB,
} libTCGBinary2Op;

// Arithmetic operators that take one argument (see libTCG_OP_UNARY_ARITHMETIC)
typedef enum {
	libTCG_UNARY_NOT,
	libTCG_UNARY_NEG,
} libTCGUnaryOp;

typedef enum {
	libTCG_EXTEND_8BIT,
	libTCG_EXTEND_16BIT,
	libTCG_EXTEND_32BIT,
} libTCGExtendBits;

typedef enum {
	libTCG_BYTE_SWAP_16BIT,
	libTCG_BYTE_SWAP_32BIT,
	libTCG_BYTE_SWAP_64BIT,
} libTCGByteSwapBits;

// Within QEMU this is used to optimise jumps between translation blocks.
// It's unlikely to be meaningful for our use.
// In case this is needed:
// Translation blocks are expected two jump to no more than two destinations.
// The destinations are each allocated one of these IDs.
// libTCG_OP_GOTO_BLOCK can then be used to jump directly to the address of the corresponding block.
typedef enum {
	libTCG_BLOCK_ZERO,
	libTCG_BLOCK_ONE,
} libTCGBlockID;

// Similar to libTCGBlockID, but used at exit points of blocks.
// Also unlikely to be useful.
typedef enum {
	libTCG_EXIT_ZERO,
	libTCG_EXIT_ONE,
	libTCG_EXIT_INTERRUPT,
} libTCGExitBlockID;

typedef enum {
	libTCG_GUEST_ACCESS_8BIT,
	libTCG_GUEST_ACCESS_16BIT,
	libTCG_GUEST_ACCESS_32BIT,
	libTCG_GUEST_ACCESS_64BIT,
} libTCGGuestAccessWidth;

typedef enum {
	libTCG_GUEST_ALIGN_NONE,
	libTCG_GUEST_ALIGN_2BYTE,
	libTCG_GUEST_ALIGN_4BYTE,
	libTCG_GUEST_ALIGN_8BYTE,
	libTCG_GUEST_ALIGN_16BYTE,
	libTCG_GUEST_ALIGN_32BYTE,
	libTCG_GUEST_ALIGN_64BYTE,
} libTCGGuestAccessAlign;

// Names of every op.
// Used in libTCGOp to show which memory of the union is valid.
typedef enum {
	libTCG_OP_DISCARD, // says explicity that a variable will not be used, and should be deleted
	libTCG_OP_SET_LABEL, // marks a short-range branch destination
	libTCG_OP_CALL, // call to a helper function
	libTCG_OP_BRANCH, // short-range branch to a label
	libTCG_OP_MEMORY_BARRIER, // used to ensure synchronisation
	libTCG_OP_MOVE, // copies data from variable to another
	libTCG_OP_MOVE_IMMEDIATE, // sets a variable to a fixed value
	libTCG_OP_SET_CONDITIONAL, // set variable to 1 if condition is true, else 0
	libTCG_OP_MOVE_CONDITIONAL,
	libTCG_OP_HOST_LOAD, // read from host memory
	libTCG_OP_HOST_STORE, // write to host memory
	libTCG_OP_BINARY_ARITHMETIC, // arithmetic with one output and two inputs
	libTCG_OP_DEPOSIT, // TODO: figure out details from qemu/tcg/README
	libTCG_OP_EXTRACT, // TODO: figure out details from qemu/tcg/README
	libTCG_OP_EXTRACT2, // TODO: figure out details from qemu/tcg/README
	libTCG_OP_BRANCH_CONDITIONAL, // short-range conditional branch to a label
	libTCG_OP_BINARY2_ARITHMETIC, // BINARY_ARITHMETIC with pairs of variables to allow longer numbers
	libTCG_OP_MULTIPLY2, // multiply with pairs of variables to allow longer numbers
	libTCG_OP_BRANCH_CONDITIONAL2, // BRANCH_CONDITIONAL, but pairs of variable to allow longer numbers
	libTCG_OP_SET_CONDITIONAL2, // SET_CONDITIONAL, but pairs of variable to allow longer numbers
	libTCG_OP_BYTE_SWAP,
	libTCG_OP_EXTEND, // TODO: figure out details from qemu/tcg/README
	libTCG_OP_UNARY_ARITHMETIC, // arithmetic with one output and one input
	libTCG_OP_DIV2, // divide with pairs of variables to allow longer numbers
	libTCG_OP_CTPOP, // TODO: undocumented, reverse engineer details from source
	libTCG_OP_CAST_EXTEND, // TODO: figure out details from qemu/tcg/README
	libTCG_OP_CAST_EXTRACT, // TODO: figure out details from qemu/tcg/README
	libTCG_OP_INSTRUCTION_START, // marker that shows where disassembled instructions started
	libTCG_OP_EXIT_BLOCK, // end of translation block, continue executing from program counter
	libTCG_OP_GOTO_BLOCK, // optimisation for jumping to a block, allowed to be a no-op
	libTCG_OP_GOTO_ADDRESS, // jump to an arbitrary address
	libTCG_OP_GUEST_LOAD, // read from guest memory
	libTCG_OP_GUEST_STORE, // write to guest memory
	libTCG_OP_BLOCK_START, // marker that shows where a new translation block starts (not a QEMU TCG op)
} libTCGOpType;

// Structures for each op that has data follow...
// These are combined into the libTCGOp union so that arrays of ops can be built easily.
// Their names correspond to ops in libTCGOpType.

typedef struct {
	libTCGOpType op;
	libTCGArg target;
} libTCGDiscard;

typedef struct {
	libTCGOpType op;
	libTCGLabelID label; // unique with each translation block
} libTCGSetLabel;

typedef struct {
	libTCGOpType op;
	char name[libTCG_NAME_LEN]; // name of the helper function to call
	size_t outputs_len;
	libTCGArg outputs[1];
	size_t inputs_len;
	libTCGArg inputs[6];
} libTCGCall;

typedef struct {
	libTCGOpType op;
	libTCGLabelID to;
} libTCGBranch;

typedef struct {
	libTCGOpType op;
	// Has one argument, the meaning of which I couldn't find, so it's left out for now
} libTCGMemoryBarrier;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGArg from;
	libTCGArg to;
} libTCGMove;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGArg to;
	uint64_t value;
} libTCGMoveImmediate;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGArg to;
	libTCGArg test_a;
	libTCGArg test_b;
	libTCGCondition condition;
} libTCGSetConditional;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGArg to;
	libTCGArg test_a;
	libTCGArg test_b;
	libTCGArg from_a;
	libTCGArg from_b;
	libTCGCondition condition;
} libTCGMoveConditional;

typedef struct {
	libTCGOpType op;
	libTCGHostAccessWidth from_width;
	libTCGWidth to_width;
	libTCGArg from;
	libTCGArg to;
	uint64_t offset;
	bool sign_extend;
} libTCGHostLoad;

typedef struct {
	libTCGOpType op;
	libTCGWidth from_width;
	libTCGGuestAccessWidth to_width;
	libTCGArg from;
	libTCGArg to;
	uint64_t offset;
} libTCGHostStore;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGArg to;
	libTCGArg a;
	libTCGArg b;
	libTCGBinaryOp binary_op;
} libTCGBinaryArithmetic;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGArg to;
	libTCGArg from;
	libTCGArg bits;
	uint64_t position;
	uint64_t length;
} libTCGDeposit;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	bool sign_extend;
	libTCGArg to;
	libTCGArg from;
	uint64_t position;
	uint64_t length;
} libTCGExtract;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGArg to;
	libTCGArg a;
	libTCGArg b;
	uint64_t position;
} libTCGExtract2;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGArg a;
	libTCGArg b;
	libTCGCondition condition;
	libTCGLabelID label;
} libTCGBranchConditional;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGBinary2Op binary2_op;
	libTCGArg to_low;
	libTCGArg to_high;
	libTCGArg a_low;
	libTCGArg a_high;
	libTCGArg b_low;
	libTCGArg b_high;
} libTCGBinary2Arithmetic;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	bool is_signed;
	libTCGArg to_low;
	libTCGArg to_high;
	libTCGArg a;
	libTCGArg b;
} libTCGMultiply2;

typedef struct {
	libTCGOpType op;
	libTCGLabelID label;
	libTCGArg a_low;
	libTCGArg a_high;
	libTCGArg b_low;
	libTCGArg b_high;
	libTCGCondition condition;
} libTCGBranchConditional2;

typedef struct {
	libTCGOpType op;
	libTCGArg to;
	libTCGArg a_low;
	libTCGArg a_high;
	libTCGArg b_low;
	libTCGArg b_high;
	libTCGCondition condition;
} libTCGSetConditional2;

typedef struct {
	libTCGOpType op;
	libTCGWidth to_width;
	libTCGExtendBits from_width;
	bool sign_extend;
	libTCGArg to;
	libTCGArg from;
} libTCGExtend;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGByteSwapBits bits;
	libTCGArg to;
	libTCGArg from;
} libTCGByteSwap;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGUnaryOp unary_op;
	libTCGArg to;
	libTCGArg from;
} libTCGUnaryArithmetic;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	bool is_signed;
	libTCGArg to_a;
	libTCGArg to_b;
	libTCGArg from_a;
	libTCGArg from_b;
	libTCGArg from_c;
} libTCGDiv2;

typedef struct {
	libTCGOpType op;
	libTCGWidth width;
	libTCGArg to;
	libTCGArg from;
} libTCGCTPOP;

typedef struct {
	libTCGOpType op;
	bool sign_extend;
	libTCGArg to;
	libTCGArg from;
} libTCGCastExtend;

typedef struct {
	libTCGOpType op;
	bool from_high;
	libTCGArg to;
	libTCGArg from;
} libTCGCastExtract;

typedef struct {
	libTCGOpType op;
	// TODO: remove this useless information?
	libTCGExitBlockID block; // next block to execute, unlikely to be useful here
	bool address_valid; // true to indicate that address holds actual data
	uint64_t address; // address of the block we're leaving, not very useful
} libTCGExitBlock;

typedef struct {
	libTCGOpType op;
	libTCGBlockID block; // block to jump to, unlikely to be useful here because of missing lookup table
} libTCGGotoBlock;

typedef struct {
	libTCGOpType op;
	libTCGArg address;
} libTCGGotoAddress;

typedef struct {
	libTCGOpType op;
	libTCGWidth to_width;
	libTCGGuestAccessWidth from_width;
	libTCGGuestAccessAlign align;
	bool sign_extend;
	bool byte_swap;
	uint8_t mmu_index;
	libTCGArg to;
	libTCGArg address;
} libTCGGuestLoad;

typedef struct {
	libTCGOpType op;
	libTCGWidth from_width;
	libTCGGuestAccessWidth to_width;
	libTCGGuestAccessAlign align;
	bool sign_extend;
	bool byte_swap;
	uint8_t mmu_index;
	libTCGArg from;
	libTCGArg address;
} libTCGGuestStore;

typedef struct {
	libTCGOpType op;
	uint64_t address; // address in input binary that following block's first instruction was at.
} libTCGBlockStart;

// Combination of all the possible op structs.
typedef union {
	libTCGOpType op;
	libTCGDiscard discard;
	libTCGSetLabel set_label;
	libTCGCall call;
	libTCGBranch branch;
	libTCGMemoryBarrier memory_barrier;
	libTCGMove move;
	libTCGMoveImmediate move_immediate;
	libTCGSetConditional set_conditional;
	libTCGMoveConditional move_conditional;
	libTCGHostLoad host_load;
	libTCGHostStore host_store;
	libTCGBinaryArithmetic binary;
	libTCGDeposit deposit;
	libTCGExtract extract;
	libTCGExtract2 extract2;
	libTCGBranchConditional branch_conditional;
	libTCGBinary2Arithmetic binary2;
	libTCGMultiply2 multiply2;
	libTCGBranchConditional2 branch_conditional2;
	libTCGSetConditional2 set_conditional2;
	libTCGExtend extend;
	libTCGByteSwap byte_swap;
	libTCGUnaryArithmetic unary;
	libTCGDiv2 div2;
	libTCGCTPOP ctpop;
	libTCGCastExtend cast_extend;
	libTCGCastExtract cast_extract;
	libTCGExitBlock exit_block;
	libTCGGotoBlock goto_block;
	libTCGGotoAddress goto_address;
	libTCGGuestLoad guest_load;
	libTCGGuestStore guest_store;
	libTCGBlockStart block_start;
} libTCGOp;
