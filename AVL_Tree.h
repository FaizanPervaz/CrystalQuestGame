#pragma once
#include<iostream>
using namespace std;

class node {
 public:
     int id;
     int reward_score;
     int count;
     node* left;
     node* right;
     int height;

     node()
     {
         this->id = 0;
         this->reward_score = 0;
         this->count = 1;
         this->left = nullptr;
         this->right = nullptr;
     }
     node(int id, int reward_score) {
         this->id = id;
         this->reward_score = reward_score;
         this->count = 1;
         this->left = nullptr;
         this->right = nullptr;
     }
 };

 //function to get height of node
 int height(struct node* N)
 {
     if (N == NULL)
         return 0;
     return N->height;
 }

 class AVLTree {
 public:
     node* root;

     AVLTree() {
         root = nullptr;
     }

     // Recursive function to insert a node in the AVL tree
     node* insertnode(node* root, int id, int reward_score) {
         // Base case
         if (root == nullptr) {
             return new node(id, reward_score);
         }

         // Recursive insertion
         if (id < root->id) {
             root->left = insertnode(root->left, id, reward_score);
         }
         else if (id > root->id) {
             root->right = insertnode(root->right, id, reward_score);
         }
         else {
             root->count++;
             return root;
         }

         // Update the height of the current node
         root = updateHeight(root);

         // Check if the tree needs balancing
         int balanceFactor = getBalanceFactor(root);
         if (balanceFactor > 1) {
             if (id < root->left->id) {
                 return rightRotate(root);
             }
             else if (id > root->left->id) {
                 root->left = leftRotate(root->left);
                 return rightRotate(root);
             }
         }
         else if (balanceFactor < -1) {
             if (id > root->right->id) {
                 return leftRotate(root);
             }
             else if (id < root->right->id) {
                 root->right = rightRotate(root->right);
                 return leftRotate(root);
             }
         }

         return root;
     }

     // Recursive function to delete a node from the AVL tree
     node* deletenode(node* root, int id) {
         // Base case
         if (root == nullptr) {
             return root;
         }

         // Recursive deletion
         if (id < root->id) {
             root->left = deletenode(root->left, id);
         }
         else if (id > root->id) {
             root->right = deletenode(root->right, id);
         }
         else {
             if (root->count > 1) {
                 root->count--;
                 return root;
             }

             if (root->left == nullptr && root->right == nullptr) {
                 delete root;
                 return nullptr;
             }
             else if (root->left == nullptr || root->right == nullptr) {
                 node* temp = root->left ? root->left : root->right;
                 *root = *temp;
                 delete temp;
             }
             else {
                 node* temp = getMin(root->right);
                 root->id = temp->id;
                 root->reward_score = temp->reward_score;
                 root->count = temp->count;
                 root->right = deletenode(root->right, temp->id);
             }
         }

         // Update the height of the current node
         root = updateHeight(root);

         // Check if the tree needs balancing
         int balanceFactor = getBalanceFactor(root);
         if (balanceFactor > 1)
         {
         if (getBalanceFactor(root->left) >= 0) {
         return rightRotate(root);
         }
         else {
         root->left = leftRotate(root->left);
         return rightRotate(root);
         }
         }
         else if (balanceFactor < -1) {
         if (getBalanceFactor(root->right) <= 0) {
         return leftRotate(root);
         }
         else {
         root->right = rightRotate(root->right);
         return leftRotate(root);
         }
         }
         return root;
     }

     // Function to update the height of a node
     node* updateHeight(node* node) {
         int leftHeight = node->left ? node->left->height : -1;
         int rightHeight = node->right ? node->right->height : -1;
         node->height = max(leftHeight, rightHeight) + 1;
         return node;
     }

     // Function to get the balance factor of a node
     int getBalanceFactor(node* node) {
         int leftHeight = node->left ? node->left->height : -1;
         int rightHeight = node->right ? node->right->height : -1;
         return leftHeight - rightHeight;
     }

     // Function to perform a right rotation
     node* rightRotate(node* n) {
         node* NewRoot = n->left;
         n->left = NewRoot->right;
         NewRoot->right = n;
         n = updateHeight(n);
         NewRoot = updateHeight(NewRoot);
         return NewRoot;
     }

     // Function to perform a left rotation
     node* leftRotate(node* n) {
         node* NewRoot = n->right;
         n->right = NewRoot->left;
         NewRoot->left = n;
         n = updateHeight(n);
         NewRoot = updateHeight(NewRoot);
         return NewRoot;
     }

     // Function to get the node with the minimum id in a subtree
     node* getMin(node* node) {
         while (node->left) {
             node = node->left;
         }
         return node;
     }
 };

 void printTree(node* root) {
     if (root == nullptr) {
         return;
     }
     printTree(root->left);
     cout << "Node ID: " << root->id << ", Reward Score: " << root->reward_score << ", Count: " << root->count << endl;
     printTree(root->right);
 }

 void Score(node* root) {
     int score = 0;
     if (root == nullptr) {
         return;
     }
     printTree(root->left);
     score += root->reward_score;
     printTree(root->right);
     cout << "Total Score : " << score << endl;
 }