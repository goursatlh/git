#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LRU_HASH_MASK   16

typedef struct list_t
{
   struct list_t *pre; 
   struct list_t *next; 
}list;

struct rb_node
{
    struct rb_node left, right;
    bool color;
};

struct rb_root
{

};

typedef struct
{
   list *head; 
   list *tail; 
}list_head;

typedef struct 
{
    rb rb_node;
    list list_lru;
    int key;
    int value;
} LRUCache_Note;

typedef struct 
{
    rb_tree rb_root;
    list_head list_lru_head;
    int capacity;
    int num;
} LRUCache;

int lRUCache_hash(int key)
{
    int index = key%LRU_HASH_MASK;
    return index;
}

LRUCache* lRUCacheCreate(int capacity) 
{
       LRUCache *lrucache = (LRUCache *)malloc(sizeof(LRUCache));
       if (lrucache == NULL)
           return NULL;
       memset(lrucache, 0, sizeof(LRUCache));
       lrucache->capacity = capacity;
       return lrucache;
}

void lRUCache_AddToLruList(LRUCache* obj, LRUCache_Note *pNode)
{
    list *lru_list_head, *lru_list_tail, *lru_list_node;

    // add to the list_lru
    lru_list_head = obj->list_lru_head.head;
    lru_list_tail = obj->list_lru_head.tail;
    lru_list_node = &(pNode->list_lru);
    printf("lru list key %d head %x tail %x \n", pNode->key, lru_list_head, lru_list_tail);
    if (lru_list_head == NULL)
    {
        // list_lru is empty
        obj->list_lru_head.head =  lru_list_node; 
        obj->list_lru_head.tail =  lru_list_node;
    }
    else
    {
        // add to the end of list_lru
        lru_list_tail->next =  lru_list_node;
        lru_list_node->pre = lru_list_tail;
        lru_list_node->next = NULL;
        obj->list_lru_head.tail = lru_list_node;
    }
    return;
}

void lRUCache_AddToHashList(LRUCache* obj, LRUCache_Note *pNode)
{
    int index = lRUCache_hash(pNode->key);
    list *hash_list_head = obj->hash2list[index].head;
    list *hash_list_tail = obj->hash2list[index].tail;
    list *hash_list_node;

    printf("hash list key %d index %d head %x tail %x \n", pNode->key, index, hash_list_head, hash_list_tail);
    // add to the list_hash
    hash_list_node = &(pNode->list_hash);
    if (hash_list_head == NULL)
    {
        obj->hash2list[index].head = hash_list_node; 
        obj->hash2list[index].tail = hash_list_node; 
    }
    else
    {
        hash_list_tail->next = hash_list_node;
        hash_list_node->pre = hash_list_tail;
        obj->hash2list[index].tail = hash_list_node; 
    }
    return;
}

void lRUCache_RemoveFromLruList(LRUCache* obj, LRUCache_Note *pNode)
{
    list *lru_list_node = &(pNode->list_lru);

    // remove from the lru list
    if (lru_list_node->pre == NULL)
    {
        if (lru_list_node->next == NULL)
        {
            printf("Log: This is the only node in lru_list.\n");
            obj->list_lru_head.head = NULL;
            obj->list_lru_head.tail = NULL;
        }
        else
        {
            lru_list_node->next->pre = NULL;
            obj->list_lru_head.head = lru_list_node->next;
        }
        return;
    }
    else
    {
        lru_list_node->pre->next = lru_list_node->next;
        if (lru_list_node->next)
        {
            lru_list_node->next->pre = lru_list_node->pre;
        }
        else
        {
            obj->list_lru_head.tail = lru_list_node->pre;
        }
    }

    return;
}

void lRUCache_RemoveFromHashList(LRUCache* obj, LRUCache_Note *pNode)
{
    int index = lRUCache_hash(pNode->key);
    list *hash_list_node = &(pNode->list_hash); 

    if (hash_list_node->pre == NULL)
    {
         obj->hash2list[index].head = hash_list_node->next;
         if (hash_list_node->next)
         {
            hash_list_node->next->pre = NULL;
         }
         else
         {
             obj->hash2list[index].tail = NULL;
         }
    }
    else
    {
         hash_list_node->pre->next = hash_list_node->next;
         if (hash_list_node->next)
         {
            hash_list_node->next->pre = hash_list_node->pre;
         }
         else
         {
            obj->hash2list[index].tail = hash_list_node->pre;
         }
    }
    printf("hash list key %d index %d remove node %x\n", pNode->key, index, pNode);
    return;
}

void lRUCache_AllocNew(LRUCache* obj, int key, int value)
{
    LRUCache_Note *pNode = NULL;
    
    if (obj->num+1 > obj->capacity)
    {
        printf("Assert: can't add new node -> execute the capacity of LRUCache.\n");
        return;
    }
    pNode = (LRUCache_Note *)malloc(sizeof(LRUCache_Note));
    if (pNode == NULL)
        return;
    memset(pNode, 0, sizeof(LRUCache_Note));
    pNode->key = key;
    pNode->value = value;
    lRUCache_AddToLruList(obj, pNode);
    lRUCache_AddToHashList(obj, pNode);

    // update the number of elements in the cache
    ++obj->num;
    printf("put %d %d num %d pNode %x\n", key, value, obj->num, pNode);
    return;
}

LRUCache_Note *lRUCache_RemoveLRU(LRUCache* obj)
{
    LRUCache_Note *pNodeDel = NULL;
    list *lru_list_head, *lru_list_tail;

    // 1 get the LRU element
    if (obj->num > 0)
    {
        lru_list_head = obj->list_lru_head.head;
        lru_list_tail = obj->list_lru_head.tail;
        if ((lru_list_tail == NULL) || (lru_list_head == NULL))
        {
            printf("Alert: the LRU list is empty, but LRUCache number is not zero.\n");
            return;
        }
        pNodeDel =  (LRUCache_Note *)((char *)lru_list_head-sizeof(list));
        lRUCache_RemoveFromLruList(obj, pNodeDel);
        lRUCache_RemoveFromHashList(obj, pNodeDel);
        --obj->num;
        printf("remove %d %d \n", pNodeDel->key , pNodeDel->value);
    }
    return pNodeDel; 
}

void lRUCacheUpdate(LRUCache* obj, LRUCache_Note *pNode)
{
    list *lru_list_node = &(pNode->list_lru);

    //printf("%s/%d here\n", __FUNCTION__, __LINE__);
    // remove the current node, and move it to the end of the list
    if (lru_list_node->next == NULL)
    {
        // already in the end, and do nothing
        printf("%s/%d here\n", __FUNCTION__, __LINE__);
        return; 
    }
    else
    {
        // remove from the old position
        lRUCache_RemoveFromLruList(obj, pNode);     
        // insert to the end
        lRUCache_AddToLruList(obj, pNode);
    }
    //printf("%s/%d here\n", __FUNCTION__, __LINE__);
    return;
}

int lRUCacheGet(LRUCache* obj, int key) 
{
    int index = lRUCache_hash(key);
    LRUCache_Note *pNode = NULL;
    list *hash_list_node = obj->hash2list[index].head;

    printf("get key %d hash list head %x tail %x\n", key, hash_list_node, obj->hash2list[index].tail);
    for (;hash_list_node != NULL; hash_list_node = hash_list_node->next)
    {
        pNode = (LRUCache_Note *)hash_list_node;
        printf("get pNode %x\n", pNode);
        if (pNode->key == key)
        {
            //printf("%s/%d find the key : %d, value is %c\n", __FUNCTION__, __LINE__, key, pNode->value);
            printf("get %d %d\n", pNode->key, pNode->value);
            lRUCacheUpdate(obj, pNode);
            return pNode->value;
        }
    }
    printf("get %d -1\n", key);
    return -1;
}

void lRUCachePut(LRUCache* obj, int key, int value) 
{
    int index = lRUCache_hash(key);
    LRUCache_Note *pNode = NULL;
    list *hash_list_node = obj->hash2list[index].head;
    
    // find the key first
    for (;hash_list_node != NULL; hash_list_node = hash_list_node->next)
    {
        pNode = (LRUCache_Note *)hash_list_node;
        if (pNode->key == key)
        {
            printf("key already exists, update value to: %d: %d \n", key, value);
            pNode->value = value;
            lRUCacheUpdate(obj, pNode);
            return;
        }
    }

    if (obj->num+1>obj->capacity)
    {
        pNode = lRUCache_RemoveLRU(obj);
        //reuse the del node
        memset(pNode, 0, sizeof(LRUCache_Note));
        pNode->key = key;
        pNode->value= value;
        lRUCache_AddToLruList(obj, pNode);
        lRUCache_AddToHashList(obj, pNode);
        ++obj->num;
    }
    else
    {
        lRUCache_AllocNew(obj, key, value);
    }
    return;
}

void lRUCacheFree(LRUCache* obj) 
{
    list * lru_list_node = obj->list_lru_head.head;
    LRUCache_Note *pNode = NULL;

    for (; lru_list_node != NULL; )
    {
       pNode = (LRUCache_Note *)((char *)lru_list_node - sizeof(list));
       lru_list_node = lru_list_node->next;
       free(pNode);
    }

    return;    
}

void lRUCacheShow(LRUCache* obj)
{
    list * lru_list_node = obj->list_lru_head.head;
    LRUCache_Note *pNode = NULL;

    printf("\nNode list(total num %d) : ", obj->num);
    for (; lru_list_node != NULL; lru_list_node = lru_list_node->next)
    {
       pNode = (LRUCache_Note *)((char *)lru_list_node - sizeof(list));
       printf("{%d %d} ", pNode->key, pNode->value);
    }
    printf("\n");

    return;
}

int main(int argc, char **argv)
{
    int value = 0;
    LRUCache *pLRUCache = NULL;
    pLRUCache = lRUCacheCreate(2);
    lRUCachePut(pLRUCache, 1, 1);
    lRUCachePut(pLRUCache, 2, 2);
    lRUCacheShow(pLRUCache);
    //lRUCachePut(pLRUCache, 2, 'd');
    value = lRUCacheGet(pLRUCache, 1);
    //value = lRUCacheGet(pLRUCache, 1);
    lRUCacheShow(pLRUCache);
    lRUCachePut(pLRUCache, 57, 188);
    lRUCachePut(pLRUCache, 71, 188);
    lRUCacheShow(pLRUCache);
    value = lRUCacheGet(pLRUCache, 2);
    return 0;
}







