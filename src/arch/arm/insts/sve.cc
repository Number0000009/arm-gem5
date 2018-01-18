/*
 * Copyright (c) 2017 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Giacomo Gabrielli
 */

// TODO: add support for suffixes of register specifiers in disasm strings.

#include "arch/arm/insts/sve.hh"

namespace ArmISA {

const char*
svePredTypeToStr(SvePredType pt)
{
    switch (pt) {
      case SvePredType::MERGE:
        return "m";
      case SvePredType::ZERO:
        return "z";
      default:
        return "";
    }
}

std::string
SvePredCountPredOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(IntRegClass, dest));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, op1));
    return ss.str();
}

std::string
SvePredCountOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    if (destIsVec) {
        printReg(ss, RegId(VecRegClass, dest), true);
    } else {
        printReg(ss, RegId(IntRegClass, dest));
    }
    ccprintf(ss, ", ");
    uint8_t opWidth = 64;
    printReg(ss, RegId(PredRegClass, gp), false, opWidth);
    ccprintf(ss, ", ");
    if (srcIs32b)
        opWidth = 32;
    printReg(ss, RegId(IntRegClass, dest), false, opWidth);
    return ss.str();
}

std::string
SveIndexIIOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", #%d, #%d", imm1, imm2);
    return ss.str();
}

std::string
SveIndexIROp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", #%d, ", imm1);
    printReg(ss, RegId(IntRegClass, op2));
    return ss.str();
}

std::string
SveIndexRIOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(IntRegClass, op1));
    ccprintf(ss, ", #%d", imm2);
    return ss.str();
}

std::string
SveIndexRROp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(IntRegClass, op1));
    ccprintf(ss, ", ");
    printReg(ss, RegId(IntRegClass, op2));
    return ss.str();
}

std::string
SveWhileOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest));
    ccprintf(ss, ", ");
    uint8_t opWidth;
    if (srcIs32b)
        opWidth = 32;
    else
        opWidth = 64;
    printReg(ss, RegId(IntRegClass, op1), false, opWidth);
    ccprintf(ss, ", ");
    printReg(ss, RegId(IntRegClass, op2), false, opWidth);
    return ss.str();
}

std::string
SveCompTermOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(IntRegClass, op1));
    ccprintf(ss, ", ");
    printReg(ss, RegId(IntRegClass, op2));
    return ss.str();
}

std::string
SveUnaryPredOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, "/m, ");
    printReg(ss, RegId(VecRegClass, op1), true);
    return ss.str();
}

std::string
SveUnaryUnpredOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op1), true);
    return ss.str();
}

std::string
SveUnaryWideImmUnpredOp::generateDisassembly(Addr pc,
                                             const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", #");
    ss << imm;
    return ss.str();
}

std::string
SveUnaryWideImmPredOp::generateDisassembly(Addr pc,
                                           const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, (isMerging ? "/m" : "/z"));
    ccprintf(ss, ", #");
    ss << imm;
    return ss.str();
}

std::string
SveBinImmUnpredConstrOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, op1));
    ccprintf(ss, ", #");
    ss << imm;
    return ss.str();
}

std::string
SveBinImmPredOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, "/m, ");
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", #");
    ss << imm;
    return ss.str();
}

std::string
SveBinWideImmUnpredOp::generateDisassembly(Addr pc,
                                           const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", #");
    ss << imm;
    return ss.str();
}

std::string
SveBinDestrPredOp::generateDisassembly(Addr pc,
                                       const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, "/m, ");
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op2), true);
    return ss.str();
}

std::string
SveBinConstrPredOp::generateDisassembly(Addr pc,
                                        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    if (predType == SvePredType::MERGE || predType == SvePredType::ZERO) {
        ccprintf(ss, "/%s", svePredTypeToStr(predType));
    }
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op2), true);
    return ss.str();
}

std::string
SveBinUnpredOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op2), true);
    return ss.str();
}

std::string
SvePredLogicalOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    if (isSel) {
        ccprintf(ss, ", ");
    } else {
        ccprintf(ss, "/z, ");
    }
    printReg(ss, RegId(PredRegClass, op1));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, op2));
    return ss.str();
}

std::string
SvePredBinPermOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, op1));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, op2));
    return ss.str();
}

std::string
SveCmpOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, "/z, ");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op2), true);
    return ss.str();
}

std::string
SveCmpImmOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, "/z, ");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, ", #");
    ss << imm;
    return ss.str();
}

std::string
SveTerPredOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, "/m, ");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op2), true);
    return ss.str();
}

std::string
SveTerImmUnpredOp::generateDisassembly(Addr pc,
                                       const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op2), true);
    ccprintf(ss, ", #");
    ss << imm;
    return ss.str();
}

std::string
SveReducOp::generateDisassembly(Addr pc,
                                const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(FloatRegClass, dest));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op1), true);
    return ss.str();
}

std::string
SvePtrueOp::generateDisassembly(Addr pc,
                                const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest), true);
    if (imm != 0x1f) {
        ccprintf(ss, ", ");
        ss << sveDisasmPredCountImm(imm);
    }
    return ss.str();
}

std::string
SveIntCmpOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest));
    ccprintf(ss, "/z, ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, ", ");
    if (op2IsWide) {
        printReg(ss, RegId(VecRegClass, op2), true);
    } else {
        printReg(ss, RegId(VecRegClass, op2), true);
    }
    return ss.str();
}

std::string
SveIntCmpImmOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest));
    ccprintf(ss, "/z, ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, ", #");
    ss << imm;
    return ss.str();
}

std::string
SveAdrOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", [");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op2), true);
    if (offsetFormat == SveAdrOffsetUnpackedSigned) {
        ccprintf(ss, ", sxtw");
    } else if (offsetFormat == SveAdrOffsetUnpackedUnsigned) {
        ccprintf(ss, ", uxtw");
    } else if (mult != 1) {
        ccprintf(ss, ", lsl");
    }
    if (mult != 1) {
        ss << __builtin_ctz(mult);
    }
    ccprintf(ss, "]");
    return ss.str();
}

std::string
SveElemCountOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    static const char suffix[9] =
        {'\0', 'b', 'h', '\0', 'w', '\0', '\0', '\0', 'd'};
    std::stringstream ss;
    ss << "  " << mnemonic << suffix[esize] << "   ";
    if (dstIsVec) {
        printReg(ss, RegId(VecRegClass, dest), true);
    } else {
        if (dstIs32b) {
            printReg(ss, RegId(IntRegClass, dest), false, 32);
        } else {
            printReg(ss, RegId(IntRegClass, dest), false, 64);
        }
    }
    if (pattern != 0x1f) {
        ccprintf(ss, ", ");
        ss << sveDisasmPredCountImm(pattern);
        if (imm != 1) {
            ccprintf(ss, ", mul #");
            ss << std::to_string(imm);
        }
    }
    return ss.str();
}

std::string
SvePartBrkOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, isMerging ? "/m, " : "/z, ");
    printReg(ss, RegId(PredRegClass, op1));
    return ss.str();
}

std::string
SvePartBrkPropOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, "/z, ");
    printReg(ss, RegId(PredRegClass, op1));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, op2));
    return ss.str();
}

std::string
SveScalarSelectOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(IntRegClass, dest), false, scalar_width);
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    if (conditional) {
        ccprintf(ss, ", ");
        printReg(ss, RegId(IntRegClass, dest), false, scalar_width);
    }
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op1), true);
    return ss.str();
}

std::string
SveUnaryPredPredOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, op1));
    return ss.str();
}

std::string
SveTblOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", { ");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, " }, ");
    printReg(ss, RegId(VecRegClass, op2), true);
    return ss.str();
}

std::string
SveUnpackOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, op1));
    return ss.str();
}

std::string
SvePredTestOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, gp));
    ccprintf(ss, ", ");
    printReg(ss, RegId(PredRegClass, op1));
    return ss.str();
}

std::string
SvePredUnaryOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(PredRegClass, dest));
    return ss.str();
}

std::string
SveBinImmUnpredDestrOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, ", #");
    ss << imm;
    return ss.str();
}

std::string
SveBinImmIdxUnpredOp::generateDisassembly(Addr pc,
        const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss, "", false);
    printReg(ss, RegId(VecRegClass, dest), true);
    ccprintf(ss, ", ");
    printReg(ss, RegId(VecRegClass, op1), true);
    ccprintf(ss, "[");
    ss << imm;
    ccprintf(ss, "]");
    return ss.str();
}

std::string
sveDisasmPredCountImm(uint8_t imm)
{
    switch (imm) {
      case 0x0:
        return "POW2";
      case 0x1:
      case 0x2:
      case 0x3:
      case 0x4:
      case 0x5:
      case 0x6:
      case 0x7:
        return "VL" + std::to_string(imm);
      case 0x8:
      case 0x9:
      case 0xa:
      case 0xb:
      case 0xc:
      case 0xd:
        return "VL" + std::to_string(1 << ((imm & 0x7) + 3));
      case 0x1d:
        return "MUL4";
      case 0x1e:
        return "MUL3";
      case 0x1f:
        return "ALL";
      default:
        return "#" + std::to_string(imm);
    }
}

unsigned int
sveDecodePredCount(uint8_t imm, unsigned int num_elems)
{
    assert(num_elems > 0);

    switch (imm) {
      case 0x0:
        // POW2
        return 1 << (31 - __builtin_clz((uint32_t) num_elems));
      case 0x1:
      case 0x2:
      case 0x3:
      case 0x4:
      case 0x5:
      case 0x6:
      case 0x7:
        // VL1, VL2, VL3, VL4, VL5, VL6, VL7
        return (num_elems >= imm) ? imm : 0;
      case 0x8:
      case 0x9:
      case 0xa:
      case 0xb:
      case 0xc:
      case 0xd:
        // VL8, VL16, VL32, VL64, VL128, VL256
        {
            unsigned int pcount = 1 << ((imm & 0x7) + 3);
            return (num_elems >= pcount) ? pcount : 0;
        }
      case 0x1d:
        // MUL4
        return num_elems - (num_elems % 4);
      case 0x1e:
        // MUL3
        return num_elems - (num_elems % 3);
      case 0x1f:
        // ALL
        return num_elems;
      default:
        return 0;
    }
}

uint64_t
sveExpandFpImmAddSub(uint8_t imm, uint8_t size)
{
    static constexpr uint16_t fpOne16 = 0x3c00;
    static constexpr uint16_t fpPointFive16 = 0x3800;
    static constexpr uint32_t fpOne32 = 0x3f800000;
    static constexpr uint32_t fpPointFive32 = 0x3f000000;
    static constexpr uint64_t fpOne64 = 0x3ff0000000000000;
    static constexpr uint64_t fpPointFive64 = 0x3fe0000000000000;

    switch (size) {
      case 0x1:
        return imm ? fpOne16 : fpPointFive16;
      case 0x2:
        return imm ? fpOne32 : fpPointFive32;
      case 0x3:
        return imm ? fpOne64 : fpPointFive64;
      default:
        panic("Unsupported size");
    }
}

uint64_t
sveExpandFpImmMaxMin(uint8_t imm, uint8_t size)
{
    static constexpr uint16_t fpOne16 = 0x3c00;
    static constexpr uint32_t fpOne32 = 0x3f800000;
    static constexpr uint64_t fpOne64 = 0x3ff0000000000000;

    switch (size) {
      case 0x1:
        return imm ? fpOne16 : 0x0;
      case 0x2:
        return imm ? fpOne32 : 0x0;
      case 0x3:
        return imm ? fpOne64 : 0x0;
      default:
        panic("Unsupported size");
    }
}

uint64_t
sveExpandFpImmMul(uint8_t imm, uint8_t size)
{
    static constexpr uint16_t fpTwo16 = 0x4000;
    static constexpr uint16_t fpPointFive16 = 0x3800;
    static constexpr uint32_t fpTwo32 = 0x40000000;
    static constexpr uint32_t fpPointFive32 = 0x3f000000;
    static constexpr uint64_t fpTwo64 = 0x4000000000000000;
    static constexpr uint64_t fpPointFive64 = 0x3fe0000000000000;

    switch (size) {
      case 0x1:
        return imm ? fpTwo16 : fpPointFive16;
      case 0x2:
        return imm ? fpTwo32 : fpPointFive32;
      case 0x3:
        return imm ? fpTwo64 : fpPointFive64;
      default:
        panic("Unsupported size");
    }
}

}  // namespace ArmISA
