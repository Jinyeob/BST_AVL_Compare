#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define MAX(a,b) (((a)>(b))?(a):(b))
typedef char BOOL;

/*BST ����ü*/
typedef struct bstNodeStruct
{
	int value;
	struct bstNodeStruct* leftChild;
	struct bstNodeStruct* rightChild;
}bstNode;

/*AVL ����ü*/
struct avlNode {
	void* key;
	int height;   // ����Ʈ�� ����

	struct avlNode* left;
	struct avlNode* right;
};
struct avlTree {
	unsigned int count;
	BOOL(*compare)(void* argu1, void* argu2);  // �������� ������ �Ǹ� TRUE

	struct avlNode* root;
};

/*Ž�� Ƚ�� üũ ���� ��������*/
int bst_search_cnt = 0, avl_search_cnt=0;
//int bst_height = 0, avl_height = 0;

/*���� �Լ���*/
int get_bst_Tree_height(bstNode* root);
int get_avl_Tree_height(struct avlNode* root);
int* rand_num(num);

void func_bst(int num);
void func_avl(int num);


bstNode* BST_insert(bstNode* root, int value);
bstNode* findMinbstNode(bstNode* root);
bstNode* BST_delete(bstNode* root, int value);
bstNode* BST_search(bstNode* root, int value);
void BST_print(bstNode* root);

struct avlTree* CreateTree(BOOL(compFunc)(void*, void*));
void AddNode(struct avlTree* tree, void* data);
BOOL _insert(struct avlTree*, struct avlNode**, struct avlNode*);
BOOL Compare(void*, void*);
void Traverse(struct avlNode*);
struct avlNode* leftBalance(struct avlNode*);
struct avlNode* rightBalance(struct avlNode*);
struct avlNode* rottLeft(struct avlNode*);
struct avlNode* rottRight(struct avlNode*);
int GetHeightDiff(struct avlNode*);
int getHeight(struct avlNode* node);
struct avlNode* AVL_search(struct avlNode* root, int value);



/**************************main**************************/
/*                                                      */
/*                                                      */
/*                                                      */
/*                                                      */
/********************************************************/

int main()
{
	//100~2000 ���� 500�� ����
	int n = 0;
	srand((unsigned int)time(NULL));

	for (int i = 100; i <= 2000; i = 500*n) {
		n++;
		printf("*****************��尳��:[%d]*****************\n", i);

		//���� ���, ���� ���� Ž�� Ƚ���� �� (100ȸ ����)
		func_bst(i); //bst
		func_avl(i); //avl

		printf("\n");
	}
	return 0;
}

/********************************************************/
/*                                                      */
/*                                                      */
/*                                                      */
/*                                                      */
/********************************************************/

int get_bst_Tree_height(bstNode* root) { // bst Ʈ���� ����
	if (!root)
		return 0;
	else {
		int left_h = get_bst_Tree_height(root->leftChild); // ���� ����Ʈ���� ���̸� ��ȯȣ���� ���� ��´�.
		int right_h = get_bst_Tree_height(root->rightChild); // ���� ������� ������ ����Ʈ���� ���̸� ��´�.
		return 1 + (left_h > right_h ? left_h : right_h); // �� �� ū ���� 1�� ���� ��ȯ�Ѵ�.
	}
}
int get_avl_Tree_height(struct avlNode* root) { // avl Ʈ��
	if (!root)
		return 0;
	else {
		int left_h = get_avl_Tree_height(root->left); // ���� ����Ʈ���� ���̸� ��ȯȣ���� ���� ��´�.
		int right_h = get_avl_Tree_height(root->right); // ���� ������� ������ ����Ʈ���� ���̸� ��´�.
		return 1 + (left_h > right_h ? left_h : right_h); // �� �� ū ���� 1�� ���� ��ȯ�Ѵ�.
	}
}


int* rand_num(int num) {
	int i, temp, x;
	int* data = (int*)malloc(sizeof(int) * num);
	for (i = 0; i < num; i++) {
		data[i] = rand() % num + 1;
		for (x = 0; x < i; x++) {
			if (data[i] == data[x]) {
				i--;
				break;
			}
		}
	}
	return data;
}

/*BST*/
void func_bst(int num) {
	int* arr;
	int tmp;
	double sum_cnt = 0;
	double sum_height = 0;
	for (int i = 0; i < 100; i++) {
		bstNode* bst_tree = NULL;

		arr = rand_num(num);
		//���� ������ �Է�

		for (int j = 0; j < num; j++) {
			//BST�Է�			
			//printf("%d ", arr[j]);
			bst_tree = BST_insert(bst_tree, arr[j]);
		}
		//printf("\n============================================\n");

		bst_search_cnt = 0;

		tmp = rand() % num;
		BST_search(bst_tree, tmp);

		sum_cnt += bst_search_cnt;
		sum_height += get_bst_Tree_height(bst_tree);
		//printf("bst height: %d \n", get_bst_Tree_height(bst_tree));
		//printf("bst cnt: %d\n", bst_search_cnt);
	}
	//BST_print(bst_tree);
	printf("BST ��� ����(100ȸ ����): %.1f\n", sum_height/100);
	printf("BST ���� ���� Ž�� Ƚ���� ��� (100ȸ ����): %.1f\n\n", sum_cnt / 100);
}

/*AVL*/
void func_avl(int num) {
	int* arr;
	int tmp;
	double sum_cnt = 0;
	double sum_height = 0;
	int* key;
	for (int i = 0; i < 100; i++) {
		struct avlTree* avl_tree;
		avl_tree = CreateTree(Compare);
		arr = rand_num(num);
		//���� ������ �Է�
		for (int j = 0; j < num; j++) {
			//AVL�Է�
			key = (int*)malloc(sizeof(int));
			*key = arr[j];
			//printf("%d ", arr[j]);
			AddNode(avl_tree, (void*)key);
			//	Traverse(tree->root);
		}
		//printf("\n============================================\n");

		avl_search_cnt = 0;
		tmp = rand() % num;
		AVL_search(avl_tree->root, tmp);

		sum_cnt += avl_search_cnt;
		sum_height += get_avl_Tree_height(avl_tree->root);
	}

	printf("AVL ��� ����(100ȸ ����): %.1f\n", sum_height/100);
	printf("AVL ���� ���� Ž�� Ƚ���� ��� (100ȸ ����): %.1f\n\n", sum_cnt / 100);
}



/**************************BST**************************/
/*                                                     */
/*                                                     */
/*                                                     */
/*                                                     */
/*******************************************************/


bstNode* root = 0;

bstNode* BST_insert(bstNode* root, int value)
{
	if (root == 0)
	{
		root = (bstNode*)malloc(sizeof(bstNode));
		root->leftChild = root->rightChild = 0;
		root->value = value;
		return root;
	}
	else
	{
		if (root->value > value)
			root->leftChild = BST_insert(root->leftChild, value);
		else
			root->rightChild = BST_insert(root->rightChild, value);
	}
	return root;
}
bstNode* findMinbstNode(bstNode* root)
{
	bstNode* tmp = root;
	while (tmp->leftChild != 0)
		tmp = tmp->leftChild;
	return tmp;
}
bstNode* BST_delete(bstNode* root, int value)
{
	bstNode* tbstNode = 0;
	if (root == 0)
		return 0;

	if (root->value > value)
		root->leftChild = BST_delete(root->leftChild, value);
	else if (root->value < value)
		root->rightChild = BST_delete(root->rightChild, value);
	else
	{
		if (root->rightChild != 0 && root->leftChild != 0)
		{
			tbstNode = findMinbstNode(root->rightChild);
			root->value = tbstNode->value;
			root->rightChild = BST_delete(root->rightChild, tbstNode->value);
		}
		else
		{
			tbstNode = (root->leftChild == 0) ? root->rightChild : root->leftChild;
			free(root);
			return tbstNode;
		}
	}

	return root;
}
bstNode* BST_search(bstNode* root, int value)
{
	if (root == 0)
		return 0;

	if (root->value == value)
		return root;
	else if (root->value > value) {
		++bst_search_cnt;
		return BST_search(root->leftChild, value);
	}
	else {
		++bst_search_cnt;
		return BST_search(root->rightChild, value);
	}
}
void BST_print(bstNode* root)
{
	if (root == 0)
		return;

	printf("%d ", root->value);
	BST_print(root->leftChild);
	BST_print(root->rightChild);
}




/**************************AVL**************************/
/*                                                     */
/*                                                     */
/*                                                     */
/*                                                     */
/*******************************************************/

struct avlTree* CreateTree(BOOL(compFunc)(void*, void*))
{
	struct avlTree* newTree;

	newTree = (struct avlTree*)malloc(sizeof(struct avlTree));
	newTree->count = 0;
	newTree->root = 0;
	newTree->compare = compFunc;

	return newTree;
}
void AddNode(struct avlTree* tree, void* data)
{
	struct avlNode* node;

	node = (struct avlNode*)malloc(sizeof(struct avlNode));
	node->height = 1;
	node->key = data;
	node->left = 0;
	node->right = 0;

	if (tree->count == 0) {
		tree->root = node;
		tree->count++;
	}
	else
		_insert(tree, &(tree->root), node);
}
BOOL _insert(struct avlTree* tree, struct avlNode** root, struct avlNode* node)
{
	if (tree->compare((*root)->key, node->key)) {
		if ((*root)->left == 0) {
			(*root)->left = node;
			tree->count++;
			if ((*root)->right == 0) {
				(*root)->height++;

				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			if (_insert(tree, &((*root)->left), node)) {
				(*root)->height++;
				if (GetHeightDiff((*root)) > 0)
					(*root) = leftBalance((*root));
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}
	else {
		if ((*root)->right == 0) {
			(*root)->right = node;
			tree->count++;
			if ((*root)->left == 0) {
				(*root)->height++;

				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			if (_insert(tree, &((*root)->right), node)) {
				(*root)->height++;
				if (GetHeightDiff((*root)) < 0)
					(*root) = rightBalance((*root));
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}
}
BOOL Compare(void* argu1, void* argu2)
{
	if ((*(int*)(argu1)) > (*(int*)(argu2)))
		return TRUE;
	else
		return FALSE;
}
void Traverse(struct avlNode* root)
{
	if (root == 0)
		return;

	//printf("%d(%d)\n", *(int*)(root->key), GetHeightDiff(root));
	Traverse(root->left);
	Traverse(root->right);
}
struct avlNode* leftBalance(struct avlNode* root)
{

	if (GetHeightDiff(root->left) > 0)
		root = rottRight(root);
	else {
		root->left = rottLeft(root->left);
		root = rottRight(root);
	}

	return root;
}
struct avlNode* rightBalance(struct avlNode* root)
{

	if (GetHeightDiff(root->right) < 0)
		root = rottLeft(root);
	else {
		root->right = rottRight(root->right);
		root = rottLeft(root);
	}

	return root;
}
struct avlNode* rottRight(struct avlNode* root)
{
	struct avlNode* tmp;

	tmp = root->left;
	root->left = tmp->right;
	tmp->right = root;

	if (getHeight(root->left) > getHeight(root->right)) {
		root->height = getHeight(root->left) + 1;
	}
	else {
		root->height = getHeight(root->right) + 1;
	}

	if (getHeight(tmp->left) > getHeight(tmp->right)) {
		tmp->height = getHeight(tmp->left) + 1;
	}
	else {
		tmp->height = getHeight(tmp->right) + 1;
	}

	return tmp;
}
struct avlNode* rottLeft(struct avlNode* root)
{
	struct avlNode* tmp;

	tmp = root->right;
	root->right = tmp->left;
	tmp->left = root;

	if (getHeight(root->left) > getHeight(root->right)) {
		root->height = getHeight(root->left) + 1;
	}
	else {
		root->height = getHeight(root->right) + 1;
	}

	if (getHeight(tmp->left) > getHeight(tmp->right)) {
		tmp->height = getHeight(tmp->left) + 1;
	}
	else {
		tmp->height = getHeight(tmp->right) + 1;
	}

	return tmp;
}
int GetHeightDiff(struct avlNode* root)
{
	int left = 0;
	int right = 0;

	if (root->left != 0)
		left = root->left->height;

	if (root->right != 0)
		right = root->right->height;

	return left - right;
}
int getHeight(struct avlNode* node)
{
	if (node == 0)
		return 0;
	else
		return node->height;
}
struct avlNode* AVL_search(struct avlNode* root, int value)
{
	if (root == 0)
		return 0;

	if (*(int*)root->key == value)
		return root;
	else if (*(int*)root->key > value) {
		++avl_search_cnt;
		return AVL_search(root->left, value);
	}
	else {
		++avl_search_cnt;
		return AVL_search(root->right, value);
	}
}
