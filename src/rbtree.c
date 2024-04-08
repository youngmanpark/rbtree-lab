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
void search_until_nil_and_delete(rbtree *t, node_t *node);
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

void insert_fix(rbtree *t, node_t *node);
node_t *return_uncle_node(const node_t *node);
void right_rotate(rbtree *t, node_t *node);
void left_rotate(rbtree *t, node_t *node);
void exchange_color(node_t *a, node_t *b);

// 노드를 삽입하고 불균형을 복구하는 함수(노드 삽입)
node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 새노드 생성
  node_t *n_node = (node_t *)calloc(1, sizeof(node_t));
  n_node->key = key;          // key 값 세팅
  n_node->color = RBTREE_RED; // 새 노드는 항상 red로 초기화
  n_node->left = t->nil;      // 새 노드의 자식 노드는 nil로 초기화
  n_node->right = t->nil;

  // 새노드 삽입할 위치 find 후 노드 삽입
  node_t *current = t->root; // 현재 위치를 트리의 root 노드로 초기화

  if (current == t->nil)
  {
    t->root = n_node;             // root가 nil이면 tree가 비어있는것으로 root에 새노드 삽입
    n_node->color = RBTREE_BLACK; // root이면 노드 색을 black으로 다시 바꿈
  }

  while (current != t->nil)
  {
    if (key < current->key) // key값이 현재 위치노드의 키값보다 작을 때
    {
      if (current->left == t->nil) // 현재 위치노드의 left가 nil이면 새로운 노드를 삽입할 곳
      {
        current->left = n_node; // 현재 위치노드의 left자식으로 새로운 노드 삽입
        break;
      }
      current = current->left; // 다음 반복문을 위해 현재 위치를 그전 노드의 left로 변경
    }
    else // key값이 현재 위치노드의 키값보다 클 때
    {
      if (current->right == t->nil) // 현재 위치 노드의 right가 nil이면 새로운 노드를 삽입할 곳
      {
        current = current->right; // 현재 위치노드의 right자식으로 새로운 노드 삽입
        break;
      }
      current = current->right; // 현재 위치를 그전 노드의 right로 변경
    }
  }

  n_node->parent = current; // 삽입 후 새 노드의  부모 노드 초기화

  // 삽입 후 double red 발생시 불균형 트리 복구 fix()
  insert_fix(t, n_node);

  return n_node;
}
void insert_fix(rbtree *t, node_t *node)
{
  node_t *parent_node = node->parent;
  node_t *grand_parent_node = node->parent->parent;
  node_t *uncle_node;

  // root 노드 색 변경
  if (node == t->root)
  {
    node->color = RBTREE_BLACK;
    return;
  }
  // double red가 일어나지 않을 때
  if (parent_node->color == RBTREE_BLACK)
    return;
    
  uncle_node = (grand_parent_node->left == parent_node) ? grand_parent_node->right : grand_parent_node->left;

  // case 1: 삼촌 노드가 red일때
  if (uncle_node->color == RBTREE_RED)
  {
    parent_node->color = RBTREE_BLACK; // 부모,심촌 black으로 변경
    uncle_node->color = RBTREE_BLACK;
    grand_parent_node->color = RBTREE_RED; // 조부모 red로 변경
    insert_fix(t, grand_parent_node);      // node=node.parent.parent 상태로 재귀
    return;
  }

  // 부모가 조부모의 left일 경우
  if (grand_parent_node->left == parent_node)
  {
    // case 2: 현재 노드가 부모의 left일 경우(부모는 조부모의 left)
    if (parent_node->left == node)
    {
      right_rotate(t, parent_node);
      exchange_color(parent_node, parent_node->right);
      return;
    }

    // case 3:현재 노드가 부모노드의 right일 경우(부모는 조부모의 left)
    if (parent_node->right == node)
    {
      left_rotate(t, node);
      right_rotate(t, node);
      exchange_color(node, node->right);
      return;
    }
  }
  // 부모가 조부모의 right일 경우
  if (grand_parent_node->right == parent_node)
  {
    // case 4:현재 노드가 부모노드의 left일 경우(부모는 조부모의 right)
    if (parent_node->left == node)
    {
      right_rotate(t, node);
      left_rotate(t, node);
      exchange_color(node, node->left);
      return;
    }

    // case 5:현재 노드가 부모노드의 right일 경우(부모는 조부모의 right)
    if (parent_node->right == node)
    {
      left_rotate(t, parent_node);
      exchange_color(parent_node, parent_node->left);
    }
  }

  t->root->color = RBTREE_BLACK;
}
node_t *return_uncle_node(const node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = node->parent->parent;

  if (grand_parent->left == parent)
  {
    return grand_parent->right;
  }
  else
  {
    return grand_parent->left;
  }
}
// 오른쪽으로 회전하는 함수
void right_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_right = node->right;

  // 부모가 루트인 경우: 현재 노드를 루트로 지정 (노드를 삭제한 경우만 해당)
  if (parent == t->root)
    t->root = node;
  else
  { // 1-1) 노드의 부모를 grand_parent로 변경
    if (grand_parent->left == parent)
      grand_parent->left = node;
    else
      grand_parent->right = node;
  }
  node->parent = grand_parent; // 1-2) 노드를 grand_parent의 자식으로 변경 (양방향 연결)
  parent->parent = node;       // 2-1) parent의 부모를 노드로 변경
  node->right = parent;        // 2-2) parent를 노드의 자식으로 변경 (양방향 연결)
  node_right->parent = parent; // 3-1) 노드의 자식의 부모를 parent로 변경
  parent->left = node_right;   // 3-2) 노드의 자식을 부모의 자식으로 변경 (양방향 연결)
}

// 왼쪽으로 회전하는 함수
void left_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_left = node->left;

  // 부모가 루트인 경우: 현재 노드를 루트로 지정 (노드를 삭제한 경우만 해당)
  if (parent == t->root)
    t->root = node;
  else
  { // 1-1) 노드의 부모를 grand_parent로 변경
    if (grand_parent->left == parent)
      grand_parent->left = node;
    else
      grand_parent->right = node;
  }
  node->parent = grand_parent; // 1-2) 노드를 grand_parent의 자식으로 변경 (양방향 연결)
  parent->parent = node;       // 2-1) parent의 부모를 노드로 변경
  node->left = parent;         // 2-2) parent를 노드의 자식으로 변경 (양방향 연결)
  parent->right = node_left;   // 3-1) 노드의 자식의 부모를 parent로 변경
  node_left->parent = parent;  // 3-2) 노드의 자식을 부모의 자식으로 변경 (양방향 연결)
}

void exchange_color(node_t *a, node_t *b)
{
  int tmp = a->color;
  a->color = b->color;
  b->color = (tmp == RBTREE_BLACK) ? RBTREE_BLACK : RBTREE_RED;
}
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
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
