#include <ciokomb.h>

typedef struct {
  int32 PrevIndex;
  int32 NextIndex;
} __AristNode;

static CIOKOMB_API_SRC(int32, __arist_get_block_size, CiokombArist* arist){
  return arist->__ElementSize + sizeof(__AristNode);
}

static CIOKOMB_API_SRC(__AristNode*, __arist_get_node, CiokombArist* arist, int32 index){
  char* element = &(arist->Bytes[__arist_get_block_size(arist) * index]);
  return element != null ? (__AristNode*)(element + arist->__ElementSize) : null;
}

static CIOKOMB_API_SRC(int32, __arist_get_max_count, CiokombArist* arist){
  return arist->__AllocatedSize / __arist_get_block_size(arist);
}

static CIOKOMB_API_SRC(void, __arist_init, CiokombArist* arist, int32 startIndex, int32 maxCount){
  for (int32 i = startIndex; i < maxCount; ++i){
    __AristNode* node = __arist_get_node(arist, i);
    node->PrevIndex = node->NextIndex = -1;
  }
}

CIOKOMB_API_SRC(CiokombArist*, ciokomb_arist_new, int32 elementSize){
  if (elementSize <= 0) return null;
  CiokombArist* arist = malloc(sizeof(CiokombArist));
  if (arist == null) return null;

  arist->Count = 0;
  arist->RootIndex = -1;
  arist->__EmptyIndex = 0;
  arist->__ElementSize = elementSize;
  arist->__AllocatedSize = __arist_get_block_size(arist) * 4;
  arist->Bytes = malloc(arist->__AllocatedSize);
  if (arist->Bytes == null){
    ciokomb_arist_delete(arist);
    return null;
  }

  __arist_init(arist, 0, __arist_get_max_count(arist));
  return arist;
}

CIOKOMB_API_SRC(void, ciokomb_arist_delete, CiokombArist* arist){
  if (arist == null) return;
  if (arist->Bytes != null){
    free(arist->Bytes);
  }
  free(arist);
}

CIOKOMB_API_SRC(int32, ciokomb_arist_get_count, CiokombArist* arist){
  return arist->Count;
}

CIOKOMB_API_SRC(void*, ciokomb_arist_get_element, CiokombArist* arist, int32 index){
  if (__arist_get_max_count(arist) <= index) return null;

  __AristNode* node = __arist_get_node(arist, index);
  return 0 <= node->PrevIndex ? ((char*)node) - arist->__ElementSize : null;
}

CIOKOMB_API_SRC(int32, ciokomb_arist_each_root_index, CiokombArist* arist){
  return arist->RootIndex;
}

CIOKOMB_API_SRC(int32, ciokomb_arist_each_next_index, CiokombArist* arist, int32 index){
  __AristNode* node = __arist_get_node(arist, index);
  if (node == null) return -1;

  return (0 <= node->NextIndex) && (arist->RootIndex != node->NextIndex) ? node->NextIndex : -1;
}

CIOKOMB_API_SRC(int32, ciokomb_arist_lock, CiokombArist* arist){
  int32 maxCount = __arist_get_max_count(arist);
  if (arist->Count == maxCount){
    int32 allocatedSize = arist->__AllocatedSize * 2;
    char* allocatedBytes = realloc(arist->Bytes, allocatedSize);
    if (allocatedBytes == null) return -1;

    arist->Bytes = allocatedBytes;
    arist->__AllocatedSize = allocatedSize;
    maxCount *= 2;
    __arist_init(arist, arist->Count, maxCount);
  }

  int32 lockedIndex = arist->__EmptyIndex;
  __AristNode* node = __arist_get_node(arist, lockedIndex);
  if (arist->RootIndex == -1){
    arist->RootIndex = node->PrevIndex = node->NextIndex = lockedIndex;
  }else{
    __AristNode* head = __arist_get_node(arist, arist->RootIndex);
    __AristNode* tail = __arist_get_node(arist, head->PrevIndex);
    node->NextIndex = tail->NextIndex;
    node->PrevIndex = head->PrevIndex;
    head->PrevIndex = tail->NextIndex = lockedIndex;
  }
  ++arist->Count;
  if (arist->Count == maxCount){
    arist->__EmptyIndex = arist->Count;
  }else{
    for (int32 i = lockedIndex + 1; i < maxCount; ++i){
      node = __arist_get_node(arist, i);
      if (node->PrevIndex == -1){
        arist->__EmptyIndex = i;
        break;
      }
    }
  }
  return lockedIndex;
}

CIOKOMB_API_SRC(void, ciokomb_arist_unlock, CiokombArist* arist, int32 index){
  __AristNode* node = __arist_get_node(arist, index);
  if (node == null) return;
  if (node->PrevIndex == -1) return;

  if (index < arist->__EmptyIndex){
    arist->__EmptyIndex = index;
  }

  --arist->Count;
  __AristNode* prev = __arist_get_node(arist, node->PrevIndex);
  __AristNode* next = __arist_get_node(arist, node->NextIndex);
  if (arist->Count == 0){
    arist->RootIndex = -1;
  }else if (arist->RootIndex == index){
    arist->RootIndex = node->NextIndex;
  }
  prev->NextIndex = node->NextIndex;
  next->PrevIndex = node->PrevIndex;
  node->PrevIndex = node->NextIndex = -1;
}
