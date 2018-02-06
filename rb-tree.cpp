//C++11

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <unistd.h>
#include "mylib.h"

const int STEPX = 35;
const int STEPY = 35;
const int RADIUS = 24;
const int BLACK = 0;
const int RED = 1;

PAINTSTRUCT ps;
HWND hwnd = GetConsoleWindow();
HDC dc = GetDC(hwnd);
HBRUSH brush;

using namespace std;

class tree{
	public:
		int val;
		int col;
		int line;
		int color;
		tree* left;
		tree* right;
		tree* parent;
		tree* next;
		
		tree();
};

tree::tree(){
	val = 0;
	next = left = right = parent = nullptr;
}

void showTree_infix(tree*);

void showTree_prefix(tree*);

void showTree_postfix(tree*);

void showTree_weightFirst(tree*, tree*&, tree*&, int);

void addNode(int, tree*&, tree*);

int delNode(int, tree*&);

tree* findNode(int, tree*);

tree* findMax(tree*);

tree* findMin(tree*);

void delTree(tree*&);

void menu(int);

void alignTree_left(tree*);

void alignTree_right(tree*);

int countSteps_right(int&, tree*);

int countSteps_left(int&, tree*);

void shiftNode(tree*);

void paintCarcas(tree*);

void rotate_left(tree*);

void rotate_right(tree*);

tree* getGrandparent(tree*);

tree* getUncle(tree*);

tree* getBrother(tree*);

void riseTree(tree*);

void downTree(tree*);

void insert_case1(tree*);

void insert_case2(tree*);

void insert_case3(tree*);

void insert_case4(tree*);

void insert_case5(tree*);

void delete_case1(tree*);

void delete_case2(tree*);

void delete_case3(tree*);

void delete_case4(tree*);

void delete_case5(tree*);

void delete_case6(tree*);

int main(){
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	system("title Бинарное дерево");
	 
	tree* node = nullptr;
	tree* find = nullptr;
	tree* head = nullptr;
	tree* tail = nullptr;
	char* temp;
	char ch;
	
	while (1){
		system("cls");
		if (node == nullptr){
			cout << "Дерево пусто\n\n";
			menu(1);
			ch = getch();
			
			while((ch != '1') && (ch != 27)){
				printf("\a");
				ch = getch();
			}	
		}
		else{
			//Выравниваем дерево
			//свдигаем всех потомков
			shiftNode(node->left);
			shiftNode(node->right);
			//левое
			alignTree_left(node->left);
			//правое
			alignTree_right(node->right);
			menu(2);
			
			paintCarcas(node); // строим каркас
			if (find == nullptr) showTree_weightFirst(node, head, tail, 0);
			else showTree_weightFirst(node, head, tail, find->val);
			
			find = nullptr;
			ch = getch();
			
			while(((ch < '1') || (ch>'9')) && (ch != 27)){
				printf("\a");
				ch = getch();
			}
		}
		
		switch (ch){			
			case '1':
				cout << "Вводите числа. Для завершения ввода нажмите Enter:\n";
		
				while (1){
					temp = readInt();
					if (temp[0] == '\0') break;
					addNode(atoi(temp), node, node);
					cout << " ";
					if (node->parent != nullptr)
						node = node->parent;
				}
//				cout << "\n";
			break;
			
			case '2':
				cout << "Для удаления элемента введите его ключ\n";
				
				temp = readInt();
				if (temp[0] == '\0') break;
				ch = delNode(atoi(temp), node);
				
				if (!ch){
					cout << "\nЭлемент с таким ключем не найден...\n\n";
					system("pause");
				}
			break;
			
			case '3':
				cout << "Введите ключ искомого элемента\n";
				
				temp = readInt();
				if (temp[0] == '\0') break;
				find = findNode(atoi(temp), node);
				
				if (find == nullptr){
					cout << "\nЭлемент с таким ключем не найден...\n\n";
					system("pause");
				}
			break;
			
			case '4':
				find = findMax(node);
			break;
			
			case '5':
				find = findMin(node);
			break;
			
			case '6':
				cout << "Префиксная форма:\n\n";
				showTree_prefix(node);
				cout << "\n";
				system("pause");
			break;
			
			case '7':
				cout << "Инфиксная форма:\n\n";
				showTree_infix(node);
				cout << "\n";
				system("pause");
			break;
			
			case '8':
				cout << "Постфиксная форма:\n\n";
				showTree_postfix(node);
				cout << "\n";
				system("pause");
			break;
			
			case '9':
				delTree(node);
				node = nullptr;
				
				cout << "Дерево удалено...\n\n";
				system("pause");
			break;
			
			case 27:
				return 0;
		}
		
		//Поднимаем корень
		while ((node != nullptr) && (node->parent != nullptr)) node = node->parent;
	}
} //main

void showTree_infix(tree* root){
	if (root == nullptr) return;
	
	showTree_infix(root->left);
	
	cout << root->val << " ";
	
	showTree_infix(root->right);
}

void showTree_prefix(tree* root){
	if (root == nullptr) return;
	
	cout << root->val << " ";
	
	showTree_prefix(root->left);
	
	showTree_prefix(root->right);
}

void showTree_postfix(tree* root){
	if (root == nullptr) return;
	
	showTree_postfix(root->left);
	
	showTree_postfix(root->right);
	
	cout << root->val << " ";
}

void showTree_weightFirst(tree* node, tree*& head, tree*& tail, int max){
	SetBkMode(dc, TRANSPARENT);
	SetTextColor(dc, RGB(255, 255, 255));
	char strPrint[3] {0};
	
	if (node == nullptr) return;
	tree *p = nullptr;
	char flag = 0;
	
	if (head == nullptr) flag = 1;
	
//	sleep(1);
	itoa(node->val, strPrint, 10);//value в строку
	
	if (node->val == max){
		brush = CreateSolidBrush(RGB(0, 255, 0));
		SelectObject(dc, brush);
		Ellipse(dc, node->col-5, node->line-5, node->col+RADIUS, node->line+RADIUS);
		TextOut(dc, node->col+3, node->line, strPrint, 3);
	}
	else {
		if (node->color == RED) brush = CreateSolidBrush(RGB(255, 0, 0));
		if (node->color == BLACK) brush = CreateSolidBrush(RGB(0, 0, 0));
		
		
		SelectObject(dc, brush);
		Ellipse(dc, node->col-5, node->line-5, node->col+RADIUS, node->line+RADIUS);
		TextOut(dc, node->col+3, node->line, strPrint, 3);
	}
	
	if (node->left != nullptr){
		p = new tree;
		p->val = node->left->val;
		p->col = node->left->col;
		p->line = node->left->line;
		p->left = node->left->left;
		p->right = node->left->right;
		p->parent = node->left->parent;
		p->color = node->left->color;
		if (tail != nullptr) tail->next = p;
		tail = p;
		if (head == nullptr) head = tail;
	}
	
	if (node->right != nullptr){
		p = new tree;
		p->val = node->right->val;
		p->col = node->right->col;
		p->line = node->right->line;
		p->left = node->right->left;
		p->right = node->right->right;
		p->parent = node->right->parent;
		p->color = node->right->color;
		if (tail != nullptr) tail->next = p;
		tail = p;
		if (head == nullptr) head = tail;
	}
	
	if (!flag){
		p = head;
		head = head->next;
		if (head == nullptr) tail = nullptr;
		delete(p);
	}
	
	DeleteObject(brush);
	showTree_weightFirst(head, head, tail, max);
}

void paintCarcas(tree* node){
	if (node == nullptr) return;
	
	paintCarcas(node->left);
	
	if (node->parent == nullptr) MoveToEx(dc, node->col+10, node->line+10, NULL);
	
	if (node->left != nullptr) {
		MoveToEx(dc, node->col+10, node->line+10, NULL);
		LineTo(dc, node->left->col+10, node->left->line+10);
	}
	
	if (node->right != nullptr) {
		MoveToEx(dc, node->col+10, node->line+10, NULL);
		LineTo(dc, node->right->col+10, node->right->line+10);
	}
	
	paintCarcas(node->right);
}

void addNode(int value, tree*& node, tree* parent){
	if (node == nullptr){
		node = new tree;
		node->val = value;
		node->parent = parent;
		node->color = RED;
		
		if (parent == nullptr){
			node->col = 400;
			node->line = 340;
		}
		else{
			if (parent->left == node) node->col = parent->col - STEPX;
			if (parent->right == node) node->col = parent->col + STEPX;
			
			node->line = parent->line + STEPY;
		}
		//оптимизация. Случай 1
		insert_case1(node);
		
		return;
	}
	
	if (value < node->val){
		addNode(value, node->left, node);
	}
	else addNode(value, node->right, node);
}

void insert_case1(tree* node){
	if (node->parent == nullptr)
		node->color = BLACK;
	else insert_case2(node);
}

void insert_case2(tree* node){
	if (node->parent->color == BLACK) return;
	else insert_case3(node);
}

void insert_case3(tree* node){
	tree* uncle = getUncle(node);
	tree* grand;
	
	if ((uncle != nullptr) && (uncle->color == RED)){
		node->parent->color = BLACK;
		uncle->color = BLACK;
		grand = getGrandparent(node);
		grand->color = RED;
		insert_case1(grand);
	}
	else insert_case4(node);
}

void insert_case4(tree* node){
	tree* grand = getGrandparent(node);
	
	if ((node == node->parent->left) && (node->parent == grand->right)){
		rotate_right(node->parent);
		node = node->right;
	}
	else if ((node == node->parent->right) && (node->parent == grand->left)){
		rotate_left(node->parent);
		node = node->left;
	}
	
	insert_case5(node);
}

void insert_case5(tree* node){
	tree* grand = getGrandparent(node);
	node->parent->color = BLACK;
	grand->color = RED;
	if ((node == node->parent->left) && (node->parent == grand->left))
		rotate_right(grand);
	else rotate_left(grand);
}

int delNode(int value, tree*& node){
	int res;
	tree* p;
	
	if (node == nullptr){
		return 0;
	}
	
	if (node->val == value){		
		//Если это лист
		if ((node->left == nullptr) && (node->right == nullptr)){
			//Если корень
			if (node->parent == nullptr){
				p = node;
				node = nullptr;
				delete p;
				return 1;
			}
			else {
				if (node->color == BLACK) delete_case1(node);
				
				if (node->parent->left == node) node->parent->left = nullptr;
				else node->parent->right = nullptr;
				
				delete node;
				return 1;
			}
		}
		//Если есть оба потомока
		if ((node->left != nullptr) && (node->right != nullptr)){
			
			p = findMax(node->left);
			node->val = p->val;
			
			delNode(p->val, p);
			return 1;
		}
		else{//Если один потомок
			if (node->left != nullptr){
				p = findMax(node->left);
				node->val = p->val;
				
				delNode(p->val, p);
				return 1;
			}
			else {
				p = findMin(node->right);
				node->val = p->val;
				
				delNode(p->val, p);
				return 1;
			}
			return 1;
		}
	}
	
	res = delNode(value, node->left);
	if (res) return 1;
	
	res = delNode(value, node->right);
	if (res) return 1;
	else return 0;
	
}

void delete_case1(tree* node){
	if (node->parent != nullptr) delete_case2(node);
}

void delete_case2(tree* node){
	tree* brother = getBrother(node);
	
	if ((brother != nullptr) && (brother->color == RED)){
		node->parent->color = RED;
		brother->color = BLACK;
		
		if (node == node->parent->left) rotate_left(node->parent);
		else rotate_right(node->parent);
	}
	
	delete_case3(node);
}

void delete_case3(tree* node){
	tree* brother = getBrother(node);
	
	if ((node->parent->color == BLACK) &&
		(brother->color == BLACK) &&
		((brother->left == nullptr) || (brother->left->color == BLACK)) &&
		((brother->right == nullptr) || (brother->right->color == BLACK))){
			brother->color = RED;
			delete_case1(node->parent);
		} else
			delete_case4(node);
}

void delete_case4(tree* node){
	tree* brother = getBrother(node);
	
	if ((node->parent->color == RED) &&
		(brother->color == BLACK) &&
		((brother->left == nullptr) || (brother->left->color == BLACK)) &&
		((brother->right == nullptr) || (brother->right->color == BLACK))){
			brother->color = RED;
			node->parent->color = BLACK;
		} else
			delete_case5(node);
}

void delete_case5(tree* node){
	tree* brother = getBrother(node);
	
	if (brother->color == BLACK){
		if ((node == node->parent->left) &&
			(brother->left != nullptr) &&
			((brother->right == nullptr) || (brother->right->color == BLACK)) &&
			(brother->left->color == RED)){
				brother->color = RED;
				brother->left->color = BLACK;
				rotate_right(brother);
			}
		else if ((node == node->parent->right) &&
				(brother->right != nullptr) &&
				((brother->left == nullptr) || (brother->left->color == BLACK)) &&
				(brother->right->color == RED) &&
				(brother->left->color == BLACK)){
					brother->color = RED;
					brother->right->color = BLACK;
					rotate_left(brother);
				}
	}
	delete_case6(node);
}

void delete_case6(tree* node){
	tree* brother = getBrother(node);
	
	brother->color = node->parent->color;
	node->parent->color = BLACK;
	
	if (node == node->parent->left){
		brother->right->color = BLACK;
		rotate_left(node->parent);
	} else{
		brother->left->color = BLACK;
		rotate_right(node->parent);
	}
}

tree* findNode(int value, tree* node){
	tree* p;
	
	if (node == nullptr) return nullptr;
	
	if (node->val == value) return node;
	p = findNode(value, node->left);
	if (p != nullptr) return p;
	
	p = findNode(value, node->right);
	if (p != nullptr) return p;
	else return nullptr;	
}

tree* findMax(tree* node){
	while (node->right != nullptr){
		node = node->right;
	}
	return node;
}

tree* findMin(tree* node){
	while (node->left != nullptr){
		node = node->left;
	}
	return node;
}

void alignTree_left(tree* node){
	if (node == nullptr) return;
	int steps = 0;
	tree* p;
	countSteps_right(steps, node);
	countSteps_left(steps, node->parent->right);
	node->col -= steps*STEPX;
	
	//свдигаем всех потомков
	shiftNode(node->left);
	shiftNode(node->right);
	
	//продолжаем сдвигать по шагам
	alignTree_left(node->left);
	if (node->right != nullptr)	alignTree_left(node->right->left);
}

void alignTree_right(tree* node){
	if (node == nullptr) return;
	int steps = 0;
	tree* p;
	countSteps_left(steps, node);
	countSteps_right(steps, node->parent->left);
	node->col += steps*STEPX;
	
	//свдигаем всех потомков
	shiftNode(node->left);
	shiftNode(node->right);
	
	//продолжаем сдвигать по шагам
	alignTree_right(node->right);
	if (node->left != nullptr)	alignTree_right(node->left->right);
}

int countSteps_right(int& steps, tree* node){
	if (node == nullptr) return steps;
	if (node->right != nullptr) steps++;
	else return steps;
	countSteps_right(steps, node->right);
}

int countSteps_left(int& steps, tree* node){
	if (node == nullptr) return steps;
	if (node->left != nullptr) steps++;
	else return steps;
	countSteps_left(steps, node->left);
}

void shiftNode(tree* node){
	if (node == nullptr) return;
	if (node == node->parent->left)	node->col = node->parent->col - STEPX;
	else node->col = node->parent->col + STEPX;
	shiftNode(node->left);
	shiftNode(node->right);
}

void delTree(tree*& root){
	if (root == nullptr) return;
	
	if (root->left != nullptr)
		delTree(root->left);
	if (root->right != nullptr)
		delTree(root->right);
//	delNode(root->val, root);
	delete root;
	return;
}

void rotate_left(tree* node){
	tree* pivot;
	
	pivot = node->right;
	
	if (pivot == nullptr) return;
	
	//Меняем родителя node
	pivot->parent = node->parent;
	
	if (node->parent != nullptr){
		if (node == node->parent->left)
			node->parent->left = pivot;
		else node->parent->right = pivot;
	}
	
	//Меняем потомка pivot
	node->right = pivot->left;
	if (pivot->left != nullptr)
		pivot->left->parent = node;
	
	//меняем местами node and pivot
	node->parent = pivot;
	pivot->left = node;
	
	//координаты
	node->line += STEPY;
	pivot->line -=STEPY;
	riseTree(pivot->right);
	downTree(node->left);
}

void rotate_right(tree* node){
	tree* pivot;
	
	pivot = node->left;
	if (pivot == nullptr) return;
	
	//Меняем родителя node
	pivot->parent = node->parent;
	
	if (node->parent != nullptr){
		if (node == node->parent->left)
			node->parent->left = pivot;
		else node->parent->right = pivot;
	}
	
	//Меняем потомка pivot
	node->left = pivot->right;
	if (pivot->right != nullptr)
		pivot->right->parent = node;
	
	//меняем местами node and pivot
	node->parent = pivot;
	pivot->right = node;
	
	//координаты
	node->line += STEPY;
	pivot->line -= STEPY;
	riseTree(pivot->left);
	downTree(node->right);
}

tree* getGrandparent(tree* node){
	if ((node != nullptr) && (node->parent != nullptr))
		return node->parent->parent;
	else return nullptr;
}

tree* getUncle(tree* node){
	if (node == nullptr) return nullptr;
	tree* grand = getGrandparent(node);
	
	if (grand == nullptr) return nullptr;
	
	if (node->parent == grand->left) return grand->right;
	else return grand->left;
}

tree* getBrother(tree* node){
	if (node == nullptr) return nullptr;
	
	if (node->parent == nullptr) return nullptr;
	
	if (node == node->parent->left) return node->parent->right;
	else return node->parent->left;
}

int isLeaf(tree* node){
	if (node == nullptr) return -1;
	
	if ((node->left == nullptr) && (node->right == nullptr)) 
		return 1;
	else return 0;
}

void riseTree(tree* node){
	if (node == nullptr) return;
	
	node->line -= STEPY;
	riseTree(node->left);
	riseTree(node->right);
	return;
}

void downTree(tree* node){
	if (node == nullptr) return;
	node->line += STEPY;
	downTree(node->left);
	downTree(node->right);
	return;
}

void menu(int p){
	int i = 1;
	if (p == 1){
		cout << i++ << ".   Добавить элементы в дерево.\n";
		cout << "Esc. Завершить программу.\n";
		cout << "\n";
		return;
	}
	
	if (p == 2){
		cout << i++ << ".   Добавить элементы в дерево\n";
		cout << i++ << ".   Удалить элемент по ключу\n";
		cout << i++ << ".   Найти элемент по ключу\n";
		cout << i++ << ".   Найти максимальный элемент\n";//4
		cout << i++ << ".   Найти минимальный элемент\n";
		cout << i++ << ".   Вывести дерево в префиксной форме\n";
		cout << i++ << ".   Вывести дерево в инфиксной форме\n";
		cout << i++ << ".   Вывести дерево в постфиксной форме\n";
		cout << i++ << ".   Удалить дерево\n";//9
//		cout << "\n";
		cout << "Esc. Завершить программу\n\n";
		return;
	}
}
