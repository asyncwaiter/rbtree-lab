#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  // nil 노드는 항상 black
  nil->color = RBTREE_BLACK;
  // 할당한 tree의 nil과 root를 nil 노드로 설정
  t->nil = nil;
  t->root = nil;
  return t;
}


void left_rotate(rbtree *t, node_t *x){
  // y는 오른쪽 자식
  node_t *y = x->right;
  // y의 왼쪽 자식을 x의 오른쪽 자식으로 설정
  x->right = y->left;
  
  // y의 왼쪽이 nil이 아니면 부모를 x로 설정
  if (y->left != t->nil)
    y->left->parent = x;

  // y의 부모를 x의 부모로 대체
  y->parent = x->parent;
  
  // x가 root라면 y가 root
  if(x->parent == t->nil)
    t->root = y;
  // x가 왼쪽 자식이라면, y를 왼쪽으로
  else if (x == x->parent->left)
    x->parent->left = y;
  // x가 오른쪽 자식이라면, y를 오른쪽으로
  else
    x->parent->right = y;

  // y의 왼쪽 자식을 x로, x의 부모를 y로 마무리
  y->left = x;
  x->parent = y;
}

// 위와 대칭
void right_rotate(rbtree *t, node_t *x){
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

void delete_subtree(rbtree *t, node_t *node) {
    // 최하단에 도착하면 종료
    if (node == t->nil) {
      return;
    }
    delete_subtree(t, node->left);
    delete_subtree(t, node->right);
    free(node);
  }

void delete_rbtree(rbtree *t) {
  if (t == NULL) {
    return;
  }
  // 최하단의 자식까지 메모리 반환
  delete_subtree(t, t->root);
  // nil 노드와 트리 전체 반환
  free(t->nil);
  free(t);
}


void rbtree_insert_fixup(rbtree *t, node_t *z) {
  // 불균형은 부모가 RED일때만 맞춤
  while (z->parent->color == RBTREE_RED) {
    // 부모가 왼쪽 자식일 경우
    if (z->parent == z->parent->parent->left) {
      node_t *uncle = z->parent->parent->right;

      // CASE 1. 삼촌이 RED인 경우
      if (uncle->color == RBTREE_RED) { 
        // 부모와 삼촌을 BLACK 변경, 할머니는 RED 변경
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent; // 할머니로 넘어가서 문제없는지 재확인
      } else {
        // CASE 2. 삼촌이 BLACK, 부모는 왼쪽, 나는 오른쪽 자식인 경우
        if (z == z->parent->right) { 
          z = z->parent; 
          left_rotate(t, z); // 부모 기준으로 회전
        }
        // CASE 3. 삼촌이 BLACK, 부모와 내가 왼쪽 자식인 경우
        z->parent->color = RBTREE_BLACK; 
        z->parent->parent->color = RBTREE_RED; 
        right_rotate(t, z->parent->parent); // 부모 기준으로 회전
      }
    // 부모가 오른쪽 자식일 경우, 위와 대칭
    } else {
      node_t *uncle = z->parent->parent->left;
      if (uncle->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  // 루트는 항상 BLACK
  t->root->color = RBTREE_BLACK;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // key를 할당할 노드를 생성
  node_t *z = (node_t *)malloc(sizeof(node_t));
  z->key = key;
  z->left = t->nil;
  z->right = t->nil; // 자식 노드는 nil로 설정
  z->color = RBTREE_RED; // 항상 RED로 추가
  
  node_t *y = t->nil; // 부모의 위치를 기억할 노드
  node_t *x = t->root; // 탐색을 위한 노드
  // 최하단부까지 위치를 탐색
  while(x != t->nil){
	  y=x;
	  if (z->key < x->key)
		  x = x->left;
		else
			x = x->right;
  }
  // 위치를 찾음
  z->parent = y;
  if (y == t->nil) // 위의 while을 타지 않은 경우 -> 루트
	  t->root = z;
	else if (z->key < y->key)
		y->left = z;
	else
		y->right = z;
	
  // 불균형을 맞춤
	rbtree_insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // 탐색할 노드, root에서 탐색 시작
  node_t *current = t->root;
  
  while(current != t->nil){
    if (key < current->key) // 탐색 노드보다 key가 작으면 왼쪽
      current = current->left;
    else if (key > current->key) // 탐색 노드보다 key가 크면 오른쪽
      current = current->right;
    else
      return current; // 찾으면 리턴
  }
  return NULL; // 없으면 NULL
}


node_t *rbtree_min(const rbtree *t) {
  // 탐색할 노드, root 부터
  node_t *current = t->root;
  if (current == t->nil) {
    return NULL;
  }
  // 왼쪽 자식이 nil이 아니면 계속 왼쪽으로 이동
  while (current->left != t->nil) {
    current = current->left;
  }
  // 가장 밑의 노드 리턴
  return current;
}

// 위와 대칭 -> 오른쪽으로 가서 최대값 리턴
node_t *rbtree_max(const rbtree *t) {
  node_t *current = t->root;
  if (current == t->nil) {
    return NULL;
  }
  while (current->right != t->nil) {
    current = current->right;
  }
  return current;
}

// u와 v 노드를 교체하는 함수
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  // u의 부모가 nil이면 v가 루트
  if (u->parent == t->nil) {
    t->root = v;
  // u가 왼쪽 자식이면 u부모의 왼쪽에 v연결
  } else if (u == u->parent->left) {
    u->parent->left = v;
  // 오른쪽 자식이면 오른쪽에 v 연결
  } else {
    u->parent->right = v;
  }
  // v의 부모를 u의 부모로 변경 
  v->parent = u->parent;
}

void rbtree_erase_fixup(rbtree *t, node_t *x) {
  // x가 RED가 되면 종료
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t *brother = x->parent->right; // 형제 노드

      // CASE 1. 형제 노드가 RED인 경우
      if (brother->color == RBTREE_RED) {
        // 형제 BLACK, 부모 RED변경 후 회전
        brother->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        brother = x->parent->right; // 새로운 형제 설정
      }
      // CASE 2. 형제 자식이 둘 다 BLACK인 경우
      if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK) { 
        brother->color = RBTREE_RED; // 형제를 RED 변경
        x = x->parent; // 부모에서 다시 확인
      } else {
        // CASE 3. 형제의 오른쪽 자식이 BLACK, 왼쪽 자식이 RED인 경우
        if (brother->right->color == RBTREE_BLACK) {
          // 형제 왼쪽 자식을 BLACK, 형제를 RED로 변경 후, 회전
          brother->left->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          right_rotate(t, brother);
          brother = x->parent->right; // 새로운 형제 설정
        }
        // CASE 4. 형제의 오른쪽 자식이 RED인 경우
        // 형제의 색을 부모의 색으로, 부모와 오른쪽 자식은 BLACK 변경 후, 회전
        brother->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        brother->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root; // 루트에서 종료
      }
    } else { // 위와 대칭
      node_t *brother = x->parent->left;
      if (brother->color == RBTREE_RED) { 
        brother->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        brother = x->parent->left;
      }
      if (brother->right->color == RBTREE_BLACK && brother->left->color == RBTREE_BLACK) {  
        brother->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (brother->left->color == RBTREE_BLACK) { 
          brother->right->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          left_rotate(t, brother);
          brother = x->parent->left;
        }
        brother->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        brother->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  // root를 BLACK으로 유지
  x->color = RBTREE_BLACK;
}

// subtree에서 min을 찾는 용도의 함수
node_t *rbtree_minimum(rbtree *t, node_t *x) {
  while (x->left != t->nil) {
    x = x->left;
  }
  return x;
}


int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z; // 삭제할 노드
  node_t *x; // 조정용 노드
  color_t y_original_color = y->color; // 삭제할 노드의 원래 색상

  // CASE 1. 왼쪽 자식이 nil인 경우
  if (z->left == t->nil) {
    x = z->right;
    rbtree_transplant(t, z, z->right);
  // CASE 2. 오른쪽 자식이 nil인 경우
  } else if (z->right == t->nil) {
    x = z->left;
    rbtree_transplant(t, z, z->left);
  // CASE 3. 왼쪽과 오른쪽 자식이 모두 있는 경우
  } else {
    y = rbtree_minimum(t, z->right); // 오른쪽 자식중 최소값 노드
    y_original_color = y->color; // 최소값 노드의 원래 색상
    x = y->right; // 최소값의 오른쪽 자식
    
    // 삭제할 노드의 자식이 최소값인 경우
    if (y->parent == z) {
      // x가 여전히 y의 자식임을 명확히 함
      // y가 z의 자리를 대신하게 될 때, x는 y의 오른쪽 자식으로 남아 있어야하기 때문
      x->parent = y; 
    } else {
      rbtree_transplant(t, y, y->right); // y의 자리를 y오른쪽 자식으로 교체
      y->right = z->right; // 삭제할 노드의 오른쪽 자식을 오른쪽에 설정
      y->right->parent = y;
    }

    // 삭제할 노드의 자리를 y로 교체
    rbtree_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  // 삭제할 노드 메모리 해제
  free(z);

  // 삭제할 노드의 색상이 BLACK인 경우에만 조정 필요
  if (y_original_color == RBTREE_BLACK) {
    rbtree_erase_fixup(t, x);
  }

  return 0;
}

// 중위 순회
void inorder_traversal(const rbtree *t, node_t *node, key_t *arr, int *index) {
  if (node != t->nil) {
    inorder_traversal(t, node->left, arr, index);
    arr[*index] = node->key;
    (*index)++;
    inorder_traversal(t, node->right, arr, index);
  }
}

// 트리를 배열로 반환
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if (t->root == t->nil) {
    return 0;
  }
  int index = 0;
  inorder_traversal(t, t->root, arr, &index);
  return index; // 저장된 노드 수 리턴
}