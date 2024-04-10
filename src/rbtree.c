#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

// 트리 생성
rbtree *new_rbtree(void) {
    rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
    node_t *nil = (node_t *)calloc(1, sizeof(node_t));

    nil->color = RBTREE_BLACK;
    t->nil = nil;
    t->root = nil;

    return t;
}

// 현재 노드를 기준으로 왼쪽 회전
void left_rotation(rbtree *t, node_t *x) {
    node_t *y = x->right;

    x->right = y->left;

    if (y->left != t->nil)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == t->nil)
        t->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void right_rotation(rbtree *t, node_t *x) {
    node_t *y = x->left;

    x->left = y->right;

    if (y->right != t->nil)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == t->nil)
        t->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->right = x;
    x->parent = y;
}

void free_node(rbtree *t, node_t *x) {
    // 후위 순회 방식으로 RB Tree 내의 노드 메모리 반환
    if (x->left != t->nil) // 왼쪽으로 파고들기
        free_node(t, x->left);
    if (x->right != t->nil) // 오른쪽으로 파고들기
        free_node(t, x->right);
    free(x);  // 해당노드 메모리 free
    x = NULL; // free 후에는 사용한 변수는 NULL로 초기화(안전함)
}

void delete_rbtree(rbtree *t) {
    // t의 루트가 nil이 아닐 떄 노드 free
    if (t->root != t->nil)
        free_node(t, t->root);
    // nil노드 free
    free(t->nil);
    free(t);
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
    node_t *y;

    while (z->parent->color == RBTREE_RED) {
        // z의 부모가 조부모의 왼쪽 서브 트리일 경우
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;

            // CASE 1 : 노드 z의 삼촌 y가 적색인 경우
            if (y->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            // CASE 2 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotation(t, z);
                }
                // CASE 3 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                right_rotation(t, z->parent->parent);
            }
        }
        // z의 부모가 조부모의 왼쪽 서브 트리일 경우
        else {
            y = z->parent->parent->left;

            // CASE 4 : 노드 z의 삼촌 y가 적색인 경우
            if (y->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            // CASE 5 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotation(t, z);
                }
                // CASE 6 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotation(t, z->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *y = t->nil;
    node_t *x = t->root;
    node_t *z = (node_t *)calloc(1, sizeof(node_t));

    z->key = key;

    while (x != t->nil) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == t->nil) {
        t->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = t->nil;
    z->right = t->nil;
    z->color = RBTREE_RED;

    rbtree_insert_fixup(t, z);

    return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {

    node_t *current = t->root;

    while (current != t->nil) {
        if (current->key == key)
            return current;

        if (current->key < key)
            current = current->right;
        else
            current = current->left;
    }

    return NULL;
}

node_t *rbtree_min(const rbtree *t) {

    if (t->root == t->nil) {
        return NULL;
    }
    node_t *curr = t->root;

    while (curr->left != t->nil) {
        curr = curr->left;
    }
    return curr;
}

node_t *rbtree_max(const rbtree *t) {

    if (t->root == t->nil) {
        return NULL;
    }

    node_t *curr = t->root;

    while (curr->right != t->nil) {
        curr = curr->right;
    }
    return curr;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
    if (u->parent == t->nil) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree *t, node_t *x) {
    while (x != t->root && x->color == RBTREE_BLACK) {
        // CASE 1 ~ 4 : LEFT CASE
        if (x == x->parent->left) {
            node_t *w = x->parent->right;

            // CASE 1 : x의 형제 w가 적색인 경우
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                left_rotation(t, x->parent);
                w = x->parent->right;
            }

            // CASE 2 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            }

            // CASE 3 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
            else {
                if (w->right->color == RBTREE_BLACK) {
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    right_rotation(t, w);
                    w = x->parent->right;
                }

                // CASE 4 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                left_rotation(t, x->parent);
                x = t->root;
            }
        }
        // CASE 5 ~ 8 : RIGHT CASE
        else {
            node_t *w = x->parent->left;

            // CASE 5 : x의 형제 w가 적색인 경우
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotation(t, x->parent);
                w = x->parent->left;
            }

            // CASE 6 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
            if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            }

            // CASE 7 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
            else {
                if (w->left->color == RBTREE_BLACK) {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotation(t, w);
                    w = x->parent->left;
                }

                // CASE 8 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                right_rotation(t, x->parent);
                x = t->root;
            }
        }
    }

    x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
    node_t *x;
    node_t *y = z;
    color_t yOriginalColor = y->color;

    if (z->left == t->nil) {
        x = z->right;
        rbtree_transplant(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        rbtree_transplant(t, z, z->left);
    } else {
        y = z->right;
        while (y->left != t->nil) {
            y = y->left;
        }
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            rbtree_transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rbtree_transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == RBTREE_BLACK) {
        rbtree_delete_fixup(t, x);
    }

    free(z);

    return 0;
}

void subtree_to_array(const rbtree *t, node_t *curr, key_t *arr, size_t n, size_t *index) {
    if (curr == t->nil)
        return;

    subtree_to_array(t, curr->left, arr, n, index);
    if (*index < n) {
        arr[(*index)++] = curr->key;
    } else
        return;
    subtree_to_array(t, curr->right, arr, n, index);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    if (t->root == t->nil)
        return 0;

    size_t index = 0;
    subtree_to_array(t, t->root, arr, n, &index);
    return 0;
}