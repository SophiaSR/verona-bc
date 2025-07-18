#pragma once

#include <trieste/trieste.h>
#include <vbcc.h>
#include <vbcc/lang.h>

namespace vc
{
  using namespace trieste;
  using namespace trieste::wf::ops;
  using namespace vbcc;

  inline const auto Paren = TokenDef("paren");
  inline const auto Bracket = TokenDef("bracket");
  inline const auto Brace = TokenDef("brace");
  inline const auto List = TokenDef("list");
  inline const auto DoubleColon = TokenDef("doublecolon");
  inline const auto TripleColon = TokenDef("triplecolon");
  inline const auto Dot = TokenDef("dot");
  inline const auto DontCare = TokenDef("dontcare");
  inline const auto Ident = TokenDef("ident", flag::print);
  inline const auto Use = TokenDef("use");

  inline const auto ClassDef = TokenDef(
    "classdef", flag::symtab | flag::lookup | flag::lookdown | flag::shadowing);
  inline const auto TypeAlias = TokenDef(
    "typealias",
    flag::symtab | flag::lookup | flag::lookdown | flag::shadowing);
  inline const auto FieldDef = TokenDef("fielddef");
  inline const auto Function =
    TokenDef("function", flag::symtab | flag::lookdown);
  inline const auto ParamDef =
    TokenDef("paramdef", flag::lookup | flag::shadowing);

  inline const auto TypeName = TokenDef("typename");
  inline const auto TypeElement = TokenDef("typeelement");
  inline const auto TypeParams = TokenDef("typeparams");
  inline const auto TypeParam =
    TokenDef("typeparam", flag::lookup | flag::shadowing);
  inline const auto TypeArgs = TokenDef("typeargs");
  inline const auto ClassBody = TokenDef("classbody");
  inline const auto RawString = TokenDef("rawstring", flag::print);

  inline const auto Isect = TokenDef("isect");
  inline const auto RefType = TokenDef("reftype");
  inline const auto TupleType = TokenDef("tupletype");
  inline const auto FuncType = TokenDef("functype");
  inline const auto NoArgType = TokenDef("noargtype");

  inline const auto Expr = TokenDef("expr");
  inline const auto ExprSeq = TokenDef("exprseq");
  inline const auto Tuple = TokenDef("tuple");
  inline const auto TupleLHS = TokenDef("tuplelhs");
  inline const auto Lambda = TokenDef("lambda", flag::symtab);
  inline const auto Block = TokenDef("block", flag::symtab);
  inline const auto QName = TokenDef("qname");
  inline const auto QElement = TokenDef("qelement");
  inline const auto Op = TokenDef("op");
  inline const auto Apply = TokenDef("apply");
  inline const auto Binop = TokenDef("binop");
  inline const auto Unop = TokenDef("unop");
  inline const auto Nulop = TokenDef("nulop");

  inline const auto Let = TokenDef("let", flag::lookup | flag::shadowing);
  inline const auto Var = TokenDef("var", flag::lookup | flag::shadowing);
  inline const auto If = TokenDef("if");
  inline const auto Else = TokenDef("else");
  inline const auto While = TokenDef("while");
  inline const auto For = TokenDef("for");
  inline const auto Break = TokenDef("break");
  inline const auto Continue = TokenDef("continue");

  inline const auto TypeArgsPat = T(Bracket) << (T(List, Group) * End);

  inline const auto LiteralPat =
    T(True, False, Bin, Oct, Int, Hex, Float, HexFloat, String, RawString);

  inline const auto ApplyLhsPat = LiteralPat /
    T(ExprSeq,
      LocalId,
      Tuple,
      QName,
      Method,
      Call,
      CallDyn,
      Convert,
      Binop,
      Unop,
      Nulop);

  inline const auto ApplyRhsPat =
    ApplyLhsPat / T(DontCare, If, While, For, When, Apply);

  inline const auto ExprPat = ApplyRhsPat / (T(Else, Ref) << Any);

  inline const auto wfType =
    TypeName | Union | Isect | RefType | TupleType | FuncType | NoArgType;

  inline const auto wfBody =
    Use | TypeAlias | Break | Continue | Return | Raise | Throw | Expr;

  inline const auto wfBinop = Add | Sub | Mul | Div | Mod | Pow | And | Or |
    Xor | Shl | Shr | Eq | Ne | Lt | Le | Gt | Ge | Min | Max | LogBase | Atan2;

  inline const auto wfUnop = Neg | Not | Abs | Ceil | Floor | Exp | Log | Sqrt |
    Cbrt | IsInf | IsNaN | Sin | Cos | Tan | Asin | Acos | Atan | Sinh | Cosh |
    Tanh | Asinh | Acosh | Atanh;

  inline const auto wfNulop = None | Const_E | Const_Pi | Const_Inf | Const_NaN;

  inline const auto wfExprStructure = ExprSeq | DontCare | Ident | wfLiteral |
    String | RawString | Tuple | Let | Var | New | Lambda | QName | Op |
    Method | Call | CallDyn | If | Else | While | For | When | Equals | Ref |
    Try | Convert | Binop | Unop | Nulop | FieldRef | Load;

  inline const auto wfFuncLhs = Lhs >>= Lhs | Rhs;
  inline const auto wfFuncId = Ident >>= Ident | SymbolId;

  // clang-format off
  inline const auto wfPassStructure =
      (Top <<= ClassDef++)
    | (ClassDef <<= Ident * TypeParams * ClassBody)[Ident]
    | (ClassBody <<= (ClassDef | Use | TypeAlias | FieldDef | Function)++)
    | (Use <<= TypeName)[Include]
    | (TypeAlias <<= Ident * TypeParams * Type)[Ident]
    | (FieldDef <<= Ident * Type * Body)
    | (Function <<=
        wfFuncLhs * wfFuncId * TypeParams * Params * Type * Body)[Ident]
    | (TypeName <<= TypeElement++[1])
    | (TypeElement <<= Ident * TypeArgs)
    | (TypeParams <<= TypeParam++)
    | (TypeParam <<= Ident * (Lhs >>= Type) * (Rhs >>= Type))[Ident]
    | (TypeArgs <<= (Type | Expr)++)
    | (Params <<= ParamDef++)
    | (ParamDef <<= Ident * Type * Body)[Ident]
    | (Type <<= ~wfType)
    | (Union <<= wfType++[2])
    | (Isect <<= wfType++[2])
    | (RefType <<= wfType)
    | (TupleType <<= wfType++[2])
    | (FuncType <<= (Lhs >>= wfType) * (Rhs >>= wfType))
    | (Body <<= wfBody++)
    | (Expr <<= wfExprStructure++)
    | (ExprSeq <<= Expr++)
    | (FieldRef <<= Expr * FieldId)
    | (Load <<= Expr)
    | (Tuple <<= Expr++[2])
    | (Lambda <<= TypeParams * Params * Type * Body)
    | (Block <<= TypeParams * Params * Type * Body)
    | (QName <<= QElement++[1])
    | (QElement <<= wfFuncId * TypeArgs)
    | (Op <<= wfFuncId * TypeArgs)
    | (Method <<= Expr * wfFuncId * TypeArgs)
    | (Call <<= QName * Args)
    | (CallDyn <<= Method * Args)
    | (Args <<= Expr++)
    | (If <<= Expr * Block)
    | (Else <<= Expr * Block)
    | (While <<= Expr * Block)
    | (For <<= Expr * Block)
    | (When <<= Expr * Block)
    | (Equals <<= (Lhs >>= Expr) * (Rhs >>= Expr))
    | (Let <<= Ident * Type)[Ident]
    | (Var <<= Ident * Type)[Ident]
    | (Break <<= Expr)
    | (Continue <<= Expr)
    | (Return <<= Expr)
    | (Raise <<= Expr)
    | (Throw <<= Expr)
    | (Convert <<= (Type >>= wfPrimitiveType) * Args)
    | (Binop <<= (Op >>= wfBinop) * Args)
    | (Unop <<= (Op >>= wfUnop) * Args)
    | (Nulop <<= (Op >>= wfNulop) * Args)
    ;
  // clang-format on

  inline const auto wfExprSugar = wfExprStructure - Lambda;

  // clang-format off
  inline const auto wfPassSugar =
      wfPassStructure
    | (ParamDef <<= Ident * Type)[Ident]
    | (Expr <<= wfExprSugar++)
    ;
  // clang-format on

  inline const auto wfExprApplication = (wfExprSugar | LocalId | Apply) - Ident;

  // clang-format off
  inline const auto wfPassApplication =
      wfPassSugar
    | (Expr <<= wfExprApplication++)
    | (Apply <<= Expr++[2])
    ;
  // clang-format on

  inline const auto wfExprOperators = wfExprApplication - (Op | Apply);

  // clang-format off
  inline const auto wfPassOperators =
      wfPassApplication
    | (Expr <<= wfExprOperators)
    | (New <<= ~Expr)
    | (Ref <<= Expr)
    | (Try <<= Expr)
    ;
  // clang-format on

  inline const auto wfBodyANF = Use | TypeAlias | Const | ConstStr | Convert |
    Copy | Move | RegisterRef | FieldRef | ArrayRefConst | New | NewArrayConst |
    Load | Store | Lookup | Call | CallDyn | Typetest | Var | wfBinop | wfUnop |
    wfNulop;

  // clang-format off
  inline const auto wfPassANF =
      wfPassOperators
    | (Function <<=
        wfFuncLhs * wfFuncId * TypeParams * Params * Type * Labels)[Ident]
    | (Labels <<= Label++)
    | (Label <<= LabelId * Body * (Return >>= wfTerminator))
    | (Body <<= wfBodyANF++)
    | (Const <<= wfDst * (Type >>= wfPrimitiveType) * (Rhs >>= wfLiteral))
    | (ConstStr <<= wfDst * String)
    | (Convert <<= wfDst * (Type >>= wfPrimitiveType) * wfSrc)
    | (Copy <<= wfDst * wfSrc)
    | (Move <<= wfDst * wfSrc)
    | (RegisterRef <<= wfDst * wfSrc)
    | (FieldRef <<= wfDst * Arg * FieldId)
    | (ArrayRefConst <<= wfDst * Arg * wfLit)
    | (New <<= wfDst * Type * Args)
    | (NewArrayConst <<= wfDst * Type * wfLit)
    | (Load <<= wfDst * wfSrc)
    | (Store <<= wfDst * wfSrc * Arg)
    | (FnPointer <<= wfFuncLhs * QName)
    | (Lookup <<= wfDst * wfSrc * wfFuncLhs * wfFuncId * TypeArgs * Int)
    | (Call <<= wfDst * FnPointer * Args)
    | (CallDyn <<= wfDst * wfSrc * Args)
    | (Args <<= Arg++)
    | (Arg <<= (Type >>= (ArgMove | ArgCopy)) * wfSrc)
    | (Typetest <<= wfDst * wfSrc * Type)
    | (Return <<= LocalId)
    | (Raise <<= LocalId)
    | (Throw <<= LocalId)
    | (Cond <<= LocalId * (Lhs >>= LabelId) * (Rhs >>= LabelId))
    | (Jump <<= LabelId)
    | (Add <<= wfDst * wfLhs * wfRhs)
    | (Sub <<= wfDst * wfLhs * wfRhs)
    | (Mul <<= wfDst * wfLhs * wfRhs)
    | (Div <<= wfDst * wfLhs * wfRhs)
    | (Mod <<= wfDst * wfLhs * wfRhs)
    | (Pow <<= wfDst * wfLhs * wfRhs)
    | (And <<= wfDst * wfLhs * wfRhs)
    | (Or <<= wfDst * wfLhs * wfRhs)
    | (Xor <<= wfDst * wfLhs * wfRhs)
    | (Shl <<= wfDst * wfLhs * wfRhs)
    | (Shr <<= wfDst * wfLhs * wfRhs)
    | (Eq <<= wfDst * wfLhs * wfRhs)
    | (Ne <<= wfDst * wfLhs * wfRhs)
    | (Lt <<= wfDst * wfLhs * wfRhs)
    | (Le <<= wfDst * wfLhs * wfRhs)
    | (Gt <<= wfDst * wfLhs * wfRhs)
    | (Ge <<= wfDst * wfLhs * wfRhs)
    | (Min <<= wfDst * wfLhs * wfRhs)
    | (Max <<= wfDst * wfLhs * wfRhs)
    | (LogBase <<= wfDst * wfLhs * wfRhs)
    | (Atan2 <<= wfDst * wfLhs * wfRhs)
    | (Neg <<= wfDst * wfSrc)
    | (Not <<= wfDst * wfSrc)
    | (Abs <<= wfDst * wfSrc)
    | (Ceil <<= wfDst * wfSrc)
    | (Floor <<= wfDst * wfSrc)
    | (Exp <<= wfDst * wfSrc)
    | (Log <<= wfDst * wfSrc)
    | (Sqrt <<= wfDst * wfSrc)
    | (Cbrt <<= wfDst * wfSrc)
    | (IsInf <<= wfDst * wfSrc)
    | (IsNaN <<= wfDst * wfSrc)
    | (Sin <<= wfDst * wfSrc)
    | (Cos <<= wfDst * wfSrc)
    | (Tan <<= wfDst * wfSrc)
    | (Asin <<= wfDst * wfSrc)
    | (Acos <<= wfDst * wfSrc)
    | (Atan <<= wfDst * wfSrc)
    | (Sinh <<= wfDst * wfSrc)
    | (Cosh <<= wfDst * wfSrc)
    | (Tanh <<= wfDst * wfSrc)
    | (Asinh <<= wfDst * wfSrc)
    | (Acosh <<= wfDst * wfSrc)
    | (Atanh <<= wfDst * wfSrc)
    | (Const_E <<= wfDst)
    | (Const_Pi <<= wfDst)
    | (Const_Inf <<= wfDst)
    | (Const_NaN <<= wfDst)
    ;
  // clang-format on

  Node seq_to_args(Node seq);
  Node make_typeargs(Node typeparams);
  Node make_selftype(Node node);
  Node lookup(Node ident);
  Node resolve(Node name);
  Node resolve_qname(Node qname, Node side, size_t arity);

  Parse parser(std::shared_ptr<Bytecode> state);
  PassDef structure();
  PassDef application();
  PassDef operators();
  PassDef anf();
  PassDef sugar();
  PassDef flatten();
}
