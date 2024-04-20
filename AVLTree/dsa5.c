#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int id;
    char firstName[16];
    char lastName[16];
    char birthDate[20];
    struct Node *left;
    struct Node *right;
    int height;
} Node;

int Max(int a, int b) 
{
    return (a > b) ? a : b;
}

int height(Node *node) 
{
    if (node == NULL) 
        return 0;

    return node->height;
}

int getBalance(Node *node) 
{
    if (node == NULL) 
        return 0;

    return height(node->left) - height(node->right);
}

Node *leftRotate(Node *node) 
{
    Node *B = node->right;
    Node *Y = B->left;

    B->left = node;
    node->right = Y;

    node->height = 1 + Max(height(node->left), height(node->right));
    B->height = 1 + Max(height(B->left), height(B->right));

    return B;
}

Node *rightRotate(Node *node) 
{
    Node *A = node->left;
    Node *Y = A->right;

    A->right = node;
    node->left = Y;

    node->height = 1 + Max(height(node->left), height(node->right));
    A->height = 1 + Max(height(A->left), height(A->right));

    return A;
}

Node* newNode(int id, char* firstName, char* lastName, char* birthDate) 
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->id = id;

    for (int i = 0; (node->firstName[i] = firstName[i]) != '\0'; i++);
    for (int i = 0; (node->lastName[i] = lastName[i]) != '\0'; i++);
    for (int i = 0; (node->birthDate[i] = birthDate[i]) != '\0'; i++);

    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node* insert(Node* root, int id, char* firstName, char* lastName, char* birthDate) 
{
    if (root == NULL) 
        return newNode(id, firstName, lastName, birthDate);
    else if (id < root->id) 
        root->left = insert(root->left, id, firstName, lastName, birthDate);
    else  
        root->right = insert(root->right, id, firstName, lastName, birthDate);

    root->height = 1 + Max(height(root->left), height(root->right));
    int balanceFactor = getBalance(root);

    if (balanceFactor > 1 && id < root->left->id) 
        return rightRotate(root);

    if (balanceFactor < -1 && id > root->right->id) 
        return leftRotate(root);

    if (balanceFactor > 1 && id > root->left->id) 
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balanceFactor < -1 && id < root->right->id) 
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void searchInRange(Node *root, int low, int high, int *isFirstPrint) 
{
    if (root == NULL) 
        return;

    if (low < root->id) 
        searchInRange(root->left, low, high, isFirstPrint);

    if (low <= root->id && high >= root->id) 
    {
        if (*isFirstPrint) 
            *isFirstPrint = 0; 
        else 
            printf("\n"); 

        printf("%d %s %s %s", root->id, root->firstName, root->lastName, root->birthDate);
    }

    if (high > root->id) 
        searchInRange(root->right, low, high, isFirstPrint);
}

Node * minValueNode(Node* node) 
{
    Node* current = node;
    while (current->left != NULL) 
        current = current->left;

    return current;
}

Node* deleteNode(Node* root, int id) 
{
    if (root == NULL)
        return root;

    if (id < root->id) 
    {
        root->left = deleteNode(root->left, id);
    } 
    else if (id > root->id) 
    {
        root->right = deleteNode(root->right, id);
    } 
    else 
    {   
        if (root->left == NULL || root->right == NULL) 
        {
            Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) 
            {
                temp = root;
                root = NULL;
            } 
            else 
            {
                root->id = temp->id;
                
                for (int i = 0; (root->firstName[i] = temp->firstName[i]) != '\0'; i++);
                for (int i = 0; (root->lastName[i] = temp->lastName[i]) != '\0'; i++);
                for (int i = 0; (root->birthDate[i] = temp->birthDate[i]) != '\0'; i++);
                
                root->left = temp->left;
                root->right = temp->right;
                root->height = temp->height;
            }
            free(temp);
        } 
        else 
        {
            Node* temp = minValueNode(root->right);

            root->id = temp->id;
            for (int i = 0; (root->firstName[i] = temp->firstName[i]) != '\0'; i++);
            for (int i = 0; (root->lastName[i] = temp->lastName[i]) != '\0'; i++);
            for (int i = 0; (root->birthDate[i] = temp->birthDate[i]) != '\0'; i++);

            root->right = deleteNode(root->right, temp->id);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + Max(height(root->left), height(root->right));

    int balanceFactor = getBalance(root);

    if (balanceFactor > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balanceFactor < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balanceFactor > 1 && getBalance(root->left) < 0) 
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balanceFactor < -1 && getBalance(root->right) > 0) 
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


int main() 
{
    Node *AdelsonVelskyLandisTree = NULL;
    char command;
    int id = 0;
    int id2 = 0;
    int isFirstPrint = 1;

    while (scanf("%c", &command) != EOF) 
    {
        if (command == 'i') 
        {
            char firstName[16], lastName[16], birthDate[20];
            scanf("%d %15s %15s %19s", &id, firstName, lastName, birthDate);
            AdelsonVelskyLandisTree = insert(AdelsonVelskyLandisTree, id, firstName, lastName, birthDate);
        } 
        else if (command == 's') 
        {
            int res = scanf("%d %d", &id, &id2);
            if (res == 1) 
                searchInRange(AdelsonVelskyLandisTree, id, id,&isFirstPrint);
            else if (res == 2) 
                if (id2 > id)
                    searchInRange(AdelsonVelskyLandisTree, id, id2,&isFirstPrint);
                else
                    searchInRange(AdelsonVelskyLandisTree, id2, id,&isFirstPrint);
        } 
        else if (command == 'd') 
        {
            scanf("%d", &id);
            AdelsonVelskyLandisTree = deleteNode(AdelsonVelskyLandisTree, id);
        }
    }

    return 0;
}