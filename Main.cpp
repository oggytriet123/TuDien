#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// cau truc luu thong tin 1 tu
typedef struct Word {
	char key[100]; // nghia tieng anh
	char type[30]; // loai tu
	char meaning[521]; //nghia
	char example[300]; // vd
	char favorite; // danh dau ua thich
} Word;

// nut trong cay nhi phan
typedef struct BSTNode {
	Word data; // du lieu
	struct BSTNode *left; // con trai
	struct BSTNode *right; // con phai
} BSTNode;

// tao nut moi
BSTNode *createNode(Word w) {
	BSTNode *newNode = (BSTNode*)malloc(sizeof(BSTNode));
	if(newNode == NULL) {
		printf("Loi");
		exit(1);
	}
	newNode->data = w; // sao chep du lieu
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

// chen tu vao cay
BSTNode* insert(BSTNode *root,Word w) {
	//neu cay rong => tao nut moi
	if(root == NULL) {
		return createNode(w);
	}	
	//so sanh tu khoa
	int cmp = strcmp(w.key, root->data.key);
	
	if(cmp < 0) {
		root->left = insert(root->left,w); // chen vao nhanh trai
	} else if (cmp > 0) {
		root->right = insert(root->right,w); // chen vao nhanh phai
	} else {
		printf("Tu %s da ton tai trong tu dien!\n",w.key);
	}
	return root;
}

//ham kiem tra
void inorder(BSTNode *root) {
	if(root != NULL) {
		inorder(root->left);
		printf("%s (%s): %s\n",root->data.key,root->data.type,root->data.meaning);
		inorder(root->right);
	}
}

// tra cuu tu
BSTNode* search(BSTNode *root,char *key) {
	if(root == NULL) {
		return NULL;
	}
	int cmp = strcmp(key, root->data.key); //so sanh tu khoa
	if (cmp == 0) {
		return root; // da tim thay
	} else if (cmp > 0) {
		search(root->right,key);
	}else 
		search(root->left,key);	
} 

// xoa tu
BSTNode* findMin(BSTNode *root) {
	while(root->left != NULL) 
		root = root->left;
		return root;
	
}

BSTNode* deleteWord(BSTNode *root, char *key) {
	if(root == NULL) {
		return NULL;
	}
	int cmp = strcmp(key,root->data.key);
	if(cmp <0) 
		root->left = deleteWord(root->left,key);
	else if(cmp > 0)
		root->right = deleteWord(root->right,key);
	else {
		//da tim thay nut can xoa
		if(root->left == NULL) {
			BSTNode *temp = root->right;
			free(root);
			return temp;
		} else if(root->right == NULL) {
			BSTNode *temp = root->left;
			free(root);
			return temp;
		} else {
			BSTNode *temp = findMin(root->right);
			root->data = temp->data;
			root->right = deleteWord(root->right,temp->data.key);			
		}
	}
	return root;
}
//sua tu
void editWord(BSTNode *root,char *key) {
	BSTNode *found = search(root,key);
	if(found == NULL) {
		printf("Khong tim thay tu '%s'\n'",key);
		return;
	}
	printf("\n?? Tim thay tu: %s\n", found->data.key);
    printf("Loai tu hien tai: %s\n", found->data.type);
    printf("Nghia hien tai: %s\n", found->data.meaning);
    printf("Vi du hien tai: %s\n", found->data.example);
    printf("Yeu thich: %s\n", found->data.favorite ? "Co" : "Khong");
	 int choice;
    printf("\nBan muon sua gi?\n");
    printf("1. Nghia\n");
    printf("2. Loai tu\n");
    printf("3. Vi du\n");
    printf("4. Trang thai yeu thich\n");
    printf("Lua chon: ");
    scanf("%d", &choice);
    getchar(); 

    switch (choice) {
        case 1:
            printf("Nhap nghia moi: ");
            fgets(found->data.meaning, sizeof(found->data.meaning), stdin);
            found->data.meaning[strcspn(found->data.meaning, "\n")] = 0;
            break;
        case 2:
            printf("Nhap loai tu moi: ");
            fgets(found->data.type, sizeof(found->data.type), stdin);
            found->data.type[strcspn(found->data.type, "\n")] = 0;
            break;
        case 3:
            printf("Nhap vi du moi: ");
            fgets(found->data.example, sizeof(found->data.example), stdin);
            found->data.example[strcspn(found->data.example, "\n")] = 0;
            break;
        case 4:
            found->data.favorite = !found->data.favorite;
            printf("Trang thai yeu thich da doi thanh: %s\n",
                   found->data.favorite ? "Co" : "Khong");
            break;
        default:
            printf("Lua chon khong hop le!\n");
    }

    printf("\n? Da cap nhat thong tin tu '%s'!\n", found->data.key);
}
// in toan bo
void printHeader() {
    printf("\n==============================================================\n");
    printf("| %-15s | %-6s | %-20s | %-8s |\n",
           "Word", "Type", "Meaning", "Favorite");
    printf("==============================================================\n");
}

void inorderPrint(BSTNode *root) {
    if (root != NULL) {
        inorderPrint(root->left);
        printf("| %-15s | %-6s | %-20s | %-8s |\n",
               root->data.key,
               root->data.type,
               root->data.meaning,
               root->data.favorite ? "Yes" : "No");
        inorderPrint(root->right);
    }
}

void printDictionary(BSTNode *root) {
    printHeader();
    inorderPrint(root);
    printf("==============================================================\n");
}
void saveToFile(BSTNode *root, FILE *f) {
    if (root == NULL) return;
    saveToFile(root->left, f);
    fprintf(f, "%s|%s|%s|%s|%d\n",
            root->data.key,
            root->data.type,
            root->data.meaning,
            root->data.example,
            root->data.favorite);
    saveToFile(root->right, f);
}

void ghitep(BSTNode *root, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Khong the mo tep de ghi!\n");
        return;
    }
    saveToFile(root, f);
    fclose(f);
    printf("Da ghi du lieu tu dien ra tep '%s' thanh cong!\n", filename);
}
BSTNode* doctep(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Tep '%s' khong ton tai. Se tao moi khi them tu.\n", filename);
        return NULL;
    }

    BSTNode *root = NULL;
    Word w;
    while (fscanf(f, "%[^|]|%[^|]|%[^|]|%[^|]|%d\n",
                  w.key, w.type, w.meaning, w.example, &w.favorite) == 5) {
        root = insert(root, w);
    }

    fclose(f);
    printf("Da doc tu dien tu tep '%s' thanh cong!\n", filename);
    return root;
}


	




int main() {
    BSTNode *root = NULL;
    int choice;
    char key[100];
    const char *filename = "dictionary.txt";
    root = doctep(filename);

    do {
        printf("\n==================== TU DIEN ANH - VIET ====================\n");
        printf("1. Tra cuu tu\n");
        printf("2. Them tu moi\n");
        printf("3. Xoa tu\n");
        printf("4. Sua nghia/thong tin tu\n");
        printf("5. In toan bo tu dien (in-order)\n");
        printf("0. Thoat va luu tu dien\n");
        printf("============================================================\n");
        printf("Nhap lua chon: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Nhap tu can tra: ");
                scanf("%s", key);
                {
                    BSTNode *found = search(root, key);
                    if (found) {
                        printf("\n? Tim thay tu: %s\n", found->data.key);
                        printf("Loai tu: %s\n", found->data.type);
                        printf("Nghia: %s\n", found->data.meaning);
                        printf("Vi du: %s\n", found->data.example);
                        printf("Yeu thich: %s\n", found->data.favorite ? "Co" : "Khong");
                    } else {
                        printf("Khong tim thay tu '%s'!\n", key);
                    }
                }
                break;

            case 2: {
                Word w;
                printf("Nhap tu tieng Anh: ");
                scanf("%s", w.key);
                getchar();
                printf("Nhap loai tu: ");
                fgets(w.type, sizeof(w.type), stdin);
                w.type[strcspn(w.type, "\n")] = 0;
                printf("Nhap nghia: ");
                fgets(w.meaning, sizeof(w.meaning), stdin);
                w.meaning[strcspn(w.meaning, "\n")] = 0;
                printf("Nhap vi du: ");
                fgets(w.example, sizeof(w.example), stdin);
                w.example[strcspn(w.example, "\n")] = 0;
                w.favorite = false;

                root = insert(root, w);
                printf("Da them tu moi thanh cong!\n");
                break;
            }

            case 3:
                printf("Nhap tu can xoa: ");
                scanf("%s", key);
                root = deleteWord(root, key);
                break;

            case 4:
                printf("Nhap tu can sua: ");
                scanf("%s", key);
                getchar();
                editWord(root, key);
                break;

            case 5:
                printf("\nTOAN BO TU DIEN:\n");
                printDictionary(root);
                break;

            case 0:
                printf("Dang luu du lieu vao tep...\n");
                ghitep(root, filename);
                printf("Tam biet!\n");
                break;

            default:
                printf("Lua chon khong hop le!\n");
        }

    } while (choice != 0);

    return 0;
}

