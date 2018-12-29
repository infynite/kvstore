//
// Created by Amin  Rezaei on 12/25/18.
//

#ifndef KVSTORE_SYNTAX_H
#define KVSTORE_SYNTAX_H

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
#define ereturn(T,V) if((T)==(V)) return;
#define nreturn(T,V) if((T)!=(V)) return;
#define ereturnv(T,V,RV) if((T)==(V)) return RV;
#define nreturnv(T,V,RV) if((T)!=(V)) return RV;
#endif //KVSTORE_SYNTAX_H
