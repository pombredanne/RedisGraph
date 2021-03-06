/*
 * Copyright 2018-2019 Redis Labs Ltd. and Contributors
 *
 * This file is available under the Apache License, Version 2.0,
 * modified with the Commons Clause restriction.
 */

#include "serialize_store.h"
#include "../../stores/store.h"
#include "../../util/rmalloc.h"

/* Deserialize label store */
void* RdbLoadStore(RedisModuleIO *rdb) {
  /* Format:
   * id
   * label
   * #properties
   * properties
   */

  LabelStore *s = rm_calloc(1, sizeof(LabelStore));

  s->id = RedisModule_LoadUnsigned(rdb);
  
  s->label = RedisModule_LoadStringBuffer(rdb, NULL);

  s->properties = NewTrieMap();
  size_t len = 0;
  uint64_t propCount = RedisModule_LoadUnsigned(rdb);

  for(int i = 0; i < propCount; i++) {
    // Load property string from RDB file
    char *prop = RedisModule_LoadStringBuffer(rdb, &len);

    // Add the string directly to the store's triemap using the RDB-given length
    TrieMap_Add(s->properties, prop, len, NULL, TrieMap_NOP_REPLACE);

    // Immediately free the string, as the store does not reference it.
    RedisModule_Free(prop);
  }

  return s;
}

/* Serialize label store */
void RdbSaveStore(RedisModuleIO *rdb, void *value) {
  /* Format:
   * id
   * label
   * #properties
   * properties
   */

  LabelStore *s = value;

  RedisModule_SaveUnsigned(rdb, s->id);
  RedisModule_SaveStringBuffer(rdb, s->label, strlen(s->label) + 1);
  RedisModule_SaveUnsigned(rdb, s->properties->cardinality);

  if(s->properties->cardinality) {
    char *ptr;
    tm_len_t len;
    void *v;
    TrieMapIterator *it = TrieMap_Iterate(s->properties, "", 0);
    while(TrieMapIterator_Next(it, &ptr, &len, &v)) {
      RedisModule_SaveStringBuffer(rdb, ptr, len);
    }
    TrieMapIterator_Free(it);
  }
}
