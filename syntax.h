//
// Created by Amin  Rezaei on 12/25/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#ifndef KVSTORE_SYNTAX_H
#define KVSTORE_SYNTAX_H

/* For Loop Macros */
#define range(T,START,END) for(int $##T=START;$##T<END;$##T++)
#define irange(T,CONDITION) for(int $##T=0;CONDITION;$##T++)
#define range2(T,START,END) \
              int $##T; \
              for($##T=START;$##T<END;$##T++)
#define irange2(T,CONDITION) \
              int $##T; \
              for($##T=0;CONDITION;$##T++)
#define range_block(T,START,END,CBLOCK) for(int $##T=START;$##T<END;$##T++){ \
  CBLOCK\
}
/* Return Macros */
#define ereturn(T,V) if((T)==(V)) return;
#define nreturn(T,V) if((T)!=(V)) return;
#define ereturnv(T,V,RV) if((T)==(V)) return RV;
#define nreturnv(T,V,RV) if((T)!=(V)) return RV;
/* Method Call Macros */
#define CALL(F,...) F(__VA_ARGS__)
#define CHAIN(F2,F1,...) F2(F1(__VA_ARGS__))
#define CHAIN2(F3,F2,F1,...) F3(F2(F1(__VA_ARGS__)))
#define CHAIN3(F4,F3,F2,F1,...) F4(F3(F2(F1(__VA_ARGS__))))
#define CHAIN4(F5,F4,F3,F2,F1,...) F5(F4(F3(F2(F1(__VA_ARGS__)))))
/* Casting Method Call Macros */
#define CCALL(C,F,...) (C)F(__VA_ARGS__)
#define CCHAIN(C,F2,F1,...) (C)F2(F1(__VA_ARGS__))
#define CCHAIN2(C,F3,F2,F1,...) (C)F3(F2(F1(__VA_ARGS__)))
#define CCHAIN3(C,F4,F3,F2,F1,...) (C)F4(F3(F2(F1(__VA_ARGS__))))
#define CCHAIN4(C,F5,F4,F3,F2,F1,...) (C)F5(F4(F3(F2(F1(__VA_ARGS__)))))
#endif //KVSTORE_SYNTAX_H

#pragma clang diagnostic pop