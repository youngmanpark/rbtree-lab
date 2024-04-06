#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

// tree를 구현하기 위한 구조체와 열거형을 정의
typedef enum
{
  RBTREE_RED,
  RBTREE_BLACK
} color_t;

typedef int key_t;

// tree의 각 노드를 표현하는 구조체
typedef struct node_t
{
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

// tree 자체를 나타내는 구조체
typedef struct
{
  node_t *root;
  node_t *nil; // for sentinel
} rbtree;

rbtree *new_rbtree(void);     // 새 트리를 생성하는 함수
void delete_rbtree(rbtree *); // 트리를 순회하면서 각 노드의 메모리를 반환하는 함수

node_t *rbtree_insert(rbtree *, const key_t);     // 노드를 삽입하고 불균형을 복구하는 함수
node_t *rbtree_find(const rbtree *, const key_t); // key에 해당하는 노드를 반환하는 함수
node_t *rbtree_min(const rbtree *);               // key가 최소값에 해당하는 노드를 반환하는 함수
node_t *rbtree_max(const rbtree *);               // key가 최대값에 해당하는 노드를 반환하는 함수
int rbtree_erase(rbtree *, node_t *);             // 노드를 삭제하는 함수

int rbtree_to_array(const rbtree *, key_t *, const size_t); //'t'를 inorder로 'n'번 순회한 결과를 'arr'에 담는 함수

#endif // _RBTREE_H_
