#include "rbtree.h"

#include <stdlib.h>

// 새 트리를 생성하는 함수
rbtree *new_rbtree(void)
{
  // tree 구조체 동적 할당
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  // nill 노드 생성 및 초기화
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));

  nil->color = RBTREE_BLACK; // nil은 BLACK 기본

  // tree의 nill과 root를 nil노드로 설정(tree가 빈 경우 root는 nil노드여야 한다.)
  t->nil = nil;
  t->root = nil;

  return t;
}

// 트리를 순회하면서 각 노드의 메모리를 반환하는 함수(트리 삭제)
void delete_rbtree(rbtree *t)
{
  node_t *node = t->root; // tree의 root node 가져오기

  if (node != t->nil)                     // nil노드가 아니라면
    search_until_nil_and_delete(t, node); // 자식 노드 순회하며 메모리 반환

  // nil노드와 tree메모리 반환
  // 모든 노드 삭제 후 nil노드와 tree 구조체 메모리 반환
  free(t->nil);
  free(t);
}

/*
 각 노드의 메모리 반환
 루트 노드부터 시작하여 각 노드의 자식 노드를 순회하면서 모든 노드의 메모리를 반환
 노드의 자식 노드가 nil노드가 아니면, 해당 자식 노드를 루트로 하여 재귀적으로 함수를 호출
*/
void search_until_nil_and_delete(rbtree *t, node_t *node)
{
  if (node->left != t->nil) // 자식(왼쪽) 노드가 존재하면
    search_until_nil_and_delete(t, node->left);
  if (node->right != t->nil) // 자식(오른쪽) 노드가 존재하면
    search_until_nil_and_delete(t, node->right);

  // 해당 노드 메모리 반환
  free(node);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 새 노드 생성
  // 새 노드 동적으로 생성
  // 노드의 key를 입력받고, color를 red로, 자식 노드들은 nil노드로 설정
  // 이미 같은 key의 값이 존재해도 하나 더 추가 한다.(multiset)

  // 새 노드 삽입할 위치 탐색(rbtree_find()호출)
  // 반복문 종료 후 새 노드의 부모를 지정
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // 루트 노드부터 탐색을 시작하여 탐색 중인 노드보다 key값이 작은 경우 왼쪽 자식으로, 큰경우 오른쪽 자식으로 이동
  // 탐색 중인 노드의 자식 노드가 nil노드인 경우, 새 노드를 해당 자식 노드로 추가하고, 반복문을 종료한다.

  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
