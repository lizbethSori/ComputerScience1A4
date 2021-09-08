//COP3502C assignment 4
//This program was written by: Lizbeth Ramirez Soriano
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "leak_detector_c.h"
#define MAXLEN 30

//struct of the items
typedef struct itemNode {
  char name[MAXLEN];
  int count;
  struct itemNode *left, *right;
} itemNode;

//struct of the tree
typedef struct treeNameNode {
  char treeName[MAXLEN];
  struct treeNameNode *left, *right;
  itemNode *Tree;
} treeNameNode;

//creating the tree 
treeNameNode* createTree(char* name) {
  //DMA main tree
  treeNameNode* treename = (treeNameNode*)malloc(sizeof(treeNameNode));
  strcpy(treename->treeName, name);
  //make sure everything is set to NULL
  treename->left = NULL;
  treename->right = NULL;
  treename->Tree = NULL;
  return treename;
}

//same as createTree
itemNode* createitem(char* name, int count) {
  //similar process as create tree
  itemNode* itemnode = (itemNode*)malloc(sizeof(itemNode));
  strcpy(itemnode->name, name);
  itemnode->count = count;
  itemnode->left = NULL;
  itemnode->right = NULL;
  return itemnode;
}

//inserting the name into the tree
//group names
treeNameNode* inserttreeName(treeNameNode* root, treeNameNode* newNode){
  //if root null then we will return the other root
  if (root == NULL) {
  return newNode;
  }
  else {
    //check if item is bigger than 0
    //if so it will go to the left
    if (strcmp(root->treeName, newNode->treeName) > 0) {
      //keep going left if there is a node in the left
      if (root->left != NULL) {
        root->left = inserttreeName(root->left, newNode);
      }//if not then insert it
      else {
        root->left = newNode;
      }
    }
    else {
      //else it will go to the right
      if (root->right != NULL) {
        //keep going right then it will keep going right till there no node in the right
        root->right = inserttreeName(root->right, newNode);
      }//else insert it
      else {
        root->right = newNode;
      }
    }
  }
  return root;
}

// do similar function as inserttreeName
//inserting items
itemNode* insertItem(itemNode* treeRoot, itemNode* newNode) {
  if (treeRoot == NULL) {
    return newNode;
  }
  else {
    //check if item is bigger than 0 if so it goes left
    if (strcmp(treeRoot->name, newNode->name) > 0) {
      //keep going till there is an empty spot
      if (treeRoot->left != NULL) {
        treeRoot->left = insertItem(treeRoot->left, newNode);
      }//if not then insert it
      else {
        treeRoot->left = newNode;
      }
    }
    else {
      //keep going right till there is an empty spot
      if (treeRoot->right != NULL) {
        treeRoot->right = insertItem(treeRoot->right, newNode);
      }
      else {//if not then insert it to the right
        treeRoot->right = newNode;
      }
    }
  }
  return treeRoot;
}

//Scanning in the Names of the trees and built the group tree 
treeNameNode* buildNameTree(FILE* inFile, int N) {
  char name[30];
  //check if the infile is empty
  if (inFile == NULL) {
    return NULL;
  }
  treeNameNode* nameRoot = NULL;
  for (int x = 0; x < N; x++) {
    fscanf(inFile, "%s", name);//scan in names
    treeNameNode* newNameNode = createTree(name);//create the trees names
    nameRoot = inserttreeName(nameRoot, newNameNode);//inserted the Names
  }
  return nameRoot;
}

//Try to find the names in the main tree
treeNameNode* searchNameNode(treeNameNode* root, char treeName[50]){
  if (root == NULL) {
    return NULL;
  }
  treeNameNode* treenameNode;
  //check if treeName and the root treename is the same if so then it return the root
  if (strcmp(root->treeName, treeName) == 0) {
    return root;  
  }
  //search left
  else if (strcmp(root->treeName, treeName) > 0) {
    treenameNode = searchNameNode(root->left, treeName);
  }
  //search right
  else {
    treenameNode = searchNameNode(root->right, treeName);
  }
  return treenameNode;
}

//Scanning in items
void populateTrees(FILE* inFile, treeNameNode* nameRoot, int I) {
  //check if the infile is NULL
  if (inFile == NULL) {
    return;
  }
  else {
    int count;
    char treeName[30];
    char itemName[30];
    //for loop to how many items for each group
    for (int x = 0; x < I; x++) {
      fscanf(inFile, "%s %s %d", treeName, itemName, &count);//Scan in the group name, item and count
      //create new item node
      itemNode* newItem = createitem(itemName, count);
      //make sure the tree name is right
      treeNameNode* treeNode = searchNameNode(nameRoot, treeName);
      //insert item into the tree so we can have subtree
      treeNode->Tree = insertItem(treeNode->Tree, newItem);
    }
  }
}

//uses recursion to show the right order
void displayTree(treeNameNode* root,FILE* outFile) {
  if(root != NULL) {
    displayTree(root->left,outFile);

    printf("%s ", root->treeName);//print out each group
    fprintf(outFile,"%s ", root->treeName);

    displayTree(root->right,outFile);
  }
}

//Similar to displayInOrderNameTree but we print out the subtree
void DisplaySubTree(itemNode* root, FILE* outFile) {
  if(root != NULL) {
    DisplaySubTree(root->left, outFile);

    printf("%s ", root->name); //print out the inside of each group
    fprintf(outFile,"%s ", root->name);

    DisplaySubTree(root->right, outFile);
  }
}

//similar to displayInOrderNameTree but it will print out the title of the group
void traverse_in_traverse(treeNameNode* root, FILE* outFile) {
  if(root != NULL) {
    traverse_in_traverse(root->left, outFile);

    printf("\n===%s===\n", root->treeName);//title of each group
    fprintf(outFile,"\n===%s===\n", root->treeName);

    DisplaySubTree(root->Tree, outFile); //display the items in the group
    traverse_in_traverse(root->right,outFile);
  }
}

//subtree of search
itemNode* searchSubTree(itemNode* root, char item[50], FILE* outFile, char name[30]) {
  //if root is NULL then return NULL
  if(root == NULL) {
    return NULL;
  }
    itemNode* Node;
    //if item is the same as the root name then we will print the found
    if(strcmp(root->name, item) == 0)
    {
      if(root->count <= 0)
      {
        printf("\n%s not found in %s", item, name);
        fprintf(outFile,"\n%s not found in %s", item, name);
      }
      else {
        printf("\n%d %s found in %s",root->count, root->name, name);
        fprintf(outFile,"\n%d %s found in %s",root->count, root->name, name);
      }
      return root;
    }
    //else we keep search left or right if the item doesn't match it root item.
    else if (strcmp(root->name, item) > 0)
    {
      Node = searchSubTree(root->left, item,outFile, name);
    }
    else{
      Node = searchSubTree(root->right, item, outFile, name);
    }
    return Node;
}

//search the main tree since we are calling certain trees
itemNode* searchitem(treeNameNode* root, char treename[50], char item[50], itemNode* Root, FILE* outFile)
{
  //check if the root is NULL
  if(root == NULL)
  {
    return NULL;
  }

  itemNode *items;
  //if it is the right group we then search the sub tree
  if(strcmp(root->treeName, treename) == 0)
  {
    //it there is an item in the subtree we would print out the group since the other half is in search sub
    if (searchSubTree(root->Tree, item, outFile, treename))
    {
    }
    //else we state that we could not find the item
    else {
      printf("\n%s not found in %s", item, root->treeName);
      fprintf(outFile,"\n%s not found in %s", item, root->treeName);
    }
  }
  //if the group is not found in the first try we search left and right
  else if(strcmp(root->treeName, treename) > 0)
  {
    searchitem(root->left, treename, item, Root,outFile);
  }
  else 
  {
    searchitem(root->right, treename, item, Root, outFile);
  }
  return items;
}

//items before functions
int itembeforeItem(itemNode* root, char item[30], int count)
{
  if(root != NULL)
  {
    //if the item name is less then 0 then we would add one, left and right
    if(strcmp(root->name, item) > 0)
    {
      return 1 + itembeforeItem(root->left, item, count) + itembeforeItem(root->right, item, count);
    }
    //else we just add left and right
    else {
     return itembeforeItem(root->left, item, count) + itembeforeItem(root->right, item, count); 
    }
  }
  return 0;
}
//find the total of nodes in a sub tree
int total(itemNode* root)
{
  if (root != NULL)
  {
    if(root->count <= 0)
    {
      printf("%d",root->count);
      int Total = 1+total(root->left) + total(root->right);
      return Total -1 ;
    }
    //return the max number of nodes in a subtree
    return 1+total(root->left) + total(root->right);
  }
  return 0;
}

//prints out how many nodes are before alphabetically
treeNameNode* itembeforeTree(treeNameNode* root, char group[30], char item[30], FILE* outFile)
{
  //check if the root equal to NULL
  if(root == NULL)
  {
    return root;
  }
  treeNameNode* tree;
  //check if the group name is the right group name in root
  if(strcmp(root->treeName, group) == 0)
  {
    //find item before it will print greatest to least
    //it will also start in zero
    int c = itembeforeItem(root->Tree,item, 0);
    int t = total(root->Tree);
    int before;
    before = t - (c+1); //to stop the start of zero we will add one and make sure the max is subtracted so there no negatives
    printf("\nitem before %s: %d",item, before);
    fprintf(outFile,"\nitem before %s: %d",item, before);
  }
  //else search left or right till you find the right group
  else if(strcmp(root->treeName, group) > 0)
  {
    itembeforeTree(root->left, group, item,outFile);
  }
  else{
    itembeforeTree(root->right, group, item,outFile);
  }
  return tree;
}

//Delete the insides of a subtree
itemNode* deleteSubTree(itemNode* Tree)
{
  if(Tree == NULL)
  {
    return NULL;
  }
  //make sure it goes to every node 
  deleteSubTree(Tree->left);
  deleteSubTree(Tree->right);
  //free every node in the subtree
  free(Tree);
  return NULL;
}

//delete the sub Tree
treeNameNode* delete_Tree(treeNameNode* root, char treename[30], FILE* outFile)
{
  //check if the root equal to NULL
  if(root == NULL)
  {
    return NULL;
  }
  //find the right group name and so we will delete the inside of a tree
  if(strcmp(root->treeName, treename) == 0)
  {
    printf("\n%s deleted",root->treeName);
    fprintf(outFile,"\n%s deleted",root->treeName);
    deleteSubTree(root->Tree);
    //after the delete set it to NULL so there no double free
    root->Tree = NULL;
  }
  //else keep search in the main tree in the left or right
  else if(strcmp(root->treeName, treename) > 0)
  {
    delete_Tree(root->left, treename,outFile);
  }
  else 
  {
    delete_Tree(root->right, treename, outFile);
  }
  return NULL;
}

//find the min in the main tree
treeNameNode* mintree(treeNameNode* root)
{
  if(root->left == NULL)
  {
    return root;
  }
  else {
    return mintree(root->left);
  }
}

//Delete a node from the Tree
treeNameNode* Delete(treeNameNode* root, char treename[30])
{
  char *i;
  //check if the root equal NULL and if so return NULL
  if(root == NULL)
  {
    return NULL;
  }
  //find the right name in the main tree name
  if(strcmp(root->treeName, treename) > 0)
  {
    root->left = Delete(root->left, treename);
  }
  else if(strcmp(root->treeName,treename) < 0)
  {
    root->right = Delete(root->right, treename);
  }
  //if the Node has one child we will free the that child
  else {
    //if left is NULL then free right
    if (root->left == NULL) {
      struct treeNameNode* temp = root->right;
      free(root);
      return temp;
    }
    //else if right is empty free left
    else if (root->right == NULL) {
      struct treeNameNode* temp = root->left;
      free(root);
      return temp;
    }
    // node with two children then we find the inorder
    struct treeNameNode* temp = mintree(root->right);
    //then copy the inorder
    strcpy(root->treeName, temp->treeName);
    // Delete tree name
    root->right = Delete(root->right, temp->treeName);
  }
  return root;
}


//find the min in the subtree
itemNode* min(itemNode* root)
{
  if(root->left == NULL)
  {
    return root;
  }
  else {
    return min(root->left);
  }
}

//delete the node in the subtree
itemNode* deleteitem(itemNode* root, char item[30])
{
  //check if the root equal to NULL;
  if(root == NULL)
  {
    return NULL;
  }
  //find the right item whether it on the right or in the left
  if(strcmp(root->name, item) > 0)
  {
    root->left = deleteitem(root->left, item);
  }
  else if(strcmp(root->name,item) < 0)
  {
    root->right = deleteitem(root->right, item);
  }
  else {
    //check if it has one child and if so we free the other child
    if (root->left == NULL) {
      struct itemNode* temp = root->right;
      free(root);
      return temp;
    } 
    else if (root->right == NULL) {
      struct itemNode* temp = root->left;
      free(root);
      return temp;
    }
    // node with two children:
    struct itemNode* temp = min(root->right);
    strcpy(root->name, temp->name);
    root->count = temp->count;
    //delete node
    root->right = deleteitem(root->right, temp->name);
  }
  return root;
}

treeNameNode* deleteitemintree(treeNameNode* root, char group[30], char item[30], FILE* outFile)
{
  //check if the root is equal to NULL
  if(root == NULL)
  {
    return root;
  }

  treeNameNode* rt;
  //if the group is the right name for treename then we pint out results and find the item to delete it
  if(strcmp(root->treeName, group) == 0)
  {
    deleteitem(root->Tree, item);//find the item and delete it
    printf("\n%s deleted from %s",item, root->treeName);
    fprintf(outFile,"\n%s deleted from %s",item, root->treeName);
  }
  //if not keep searching left or right
  else if(strcmp(root->treeName, group) > 0)
  {
    deleteitemintree(root->left, group, item, outFile);
  }
  else{
    deleteitemintree(root->right, group, item, outFile);
  }
  return rt;
}

//update the count in item
itemNode* replacesub(itemNode *node, char item[30], int replace, FILE* outFile)
{
  itemNode *Root;
  //check if the root is equal to NULL
  if(node == NULL)
  {
    return  Root;
  }
  //if we find the right item then we put the replace number into the count
    if(strcmp(node->name, item) == 0)
    {
      printf("\n%s reduced",item);
      fprintf(outFile,"\n%s reduced",item);
      node->count= node->count - replace;
    }
    //if we can't find the right item then we keep looking
    else if(strcmp(node->name, item) < 0)
    {
      replacesub(node->left, item, replace, outFile);
    }
    else {
      replacesub(node->right, item, replace, outFile);
    }
    return Root;
}

//find the main tree name so we can search the items
treeNameNode* replaceTree(treeNameNode *node, char group[30], char item[30], int replace, FILE* outFile)
{
  treeNameNode *root;
  //check if the root is equal to NULL
  if(node == NULL)
  {
    return  root;
  }
  //if we found the right group then we will search it items now
  if(strcmp(node->treeName, group) == 0)
  {
    replacesub(node->Tree, item, replace, outFile);
  }
  //else we keep looking in the left or right
  else if(strcmp(node->Tree->name, group) < 0)
  {
    replaceTree(node->left, group, item, replace, outFile);
  }
  else {
    replaceTree(node->right, group, item, replace, outFile);
  }
  return root;
}

//add all the counts in items
int counter(itemNode* root)
{
  if(root == NULL)
  {
    return 0;
  }
  //search left and right
  counter(root->left);
  counter(root->right);
  //add all the counts in each node
  return root->count+counter(root->left) + counter(root->right);
}

//find the right tree group
treeNameNode* count(treeNameNode* root, char group[30], FILE* outFile)
{
  treeNameNode* rt;
  int c;
  //check if the root equal to NULL
  if(root == NULL)
  {
    return NULL;
  }
  //if it right then we will get the total count and print out the results
  if(strcmp(root->treeName, group) == 0)
  {
    c = counter(root->Tree);
    printf("\n%s count %d",root->treeName, c);
    fprintf(outFile,"\n%s count %d",root->treeName, c);
  }
  //else we keep search in the left or right
  else if(strcmp(root->treeName, group) > 0)
  {
    count(root->left, group, outFile);
  }
  else {
    count(root->right, group, outFile);
  }
  return 0;
}

//find the max height 
int max(int a, int b)
{
  return (a >= b) ? a : b;
}

//find the height for the tree
int height(itemNode *root)
{
  if(root == NULL)
  {
    return 0;
  }
  return 1+ max(height(root->left), height(root->right));
}
//find out if it balance and print results
int balance(itemNode* root, treeNameNode* Root, FILE* outFile)
{
  //find the balance without -1
  int leftheight = height(root->left);
  int rightheight = height(root->right);

  //add -1
  int left = leftheight-1;
  int right = rightheight-1;
  int difference = abs(right - left);
  //if the difference is bigger then 1 then it is not balance
  if(difference <= 1)
  {
    printf("\n%s: left height %d, right height %d, difference %d, balanced",Root->treeName, left, right, difference);
    fprintf(outFile,"\n%s: left height %d, right height %d, difference %d, balanced",Root->treeName, left, right, difference);
  }
  else{
    printf("\n%s: left height %d, right height %d, difference %d, not balanced",Root->treeName, left, right, difference);
    fprintf(outFile,"\n%s: left height %d, right height %d, difference %d, not balanced",Root->treeName, left, right, difference);
  }
  return 0;
}

//find the right group then print out the height
treeNameNode* Treeheight(treeNameNode* root, char group[30], FILE* outFile)
{
  if(root == NULL)
  {
    return root;
  }

  treeNameNode* tree;
  //check if we found the right group if then we find the balance and height in that subtree
  if(strcmp(root->treeName, group) == 0)
  {
    balance(root->Tree, root, outFile);
  }
  //else keep searching
  else if(strcmp(root->treeName, group) > 0)
  {
    tree = Treeheight(root->left, group, outFile);
  }
  else{
    tree = Treeheight(root->right, group, outFile);
  }
  return tree;
}
//scann the commands
void Scanningcommands(FILE* inFile, int Q, treeNameNode* Root, itemNode *root, FILE* outFile)
{
  char command[30];
  char item[30];
  char group[30];
  //for loops so we can scan the different kinds of commands
  for(int i = 0; i<Q; i++)
  {
    fscanf(inFile,"%s %s", command,group);//scan in commands and group since all command have it
    //some have items with it
    //search command
    if(strcmp(command,"search") == 0)
    {
      fscanf(inFile, " %s", item);
      //if we find group then go find item if not then the group does not exist
      if(searchNameNode(Root, group))
      {
        searchitem(Root, group, item, root,outFile);
      }
      else{
        printf("\n%s does not exist",group);
        fprintf(outFile,"\n%s does not exist",group);
      }
    }
    //item before command
    else if(strcmp(command, "item_before") == 0)
    {
      fscanf(inFile, "%s", item);
      itembeforeTree(Root, group, item, outFile);
    }
    //height balance command
    else if(strcmp(command, "height_balance")== 0)
    {
      Treeheight(Root, group, outFile);
    }
    //count command
    else if(strcmp(command, "count") == 0)
    {
      count(Root, group, outFile);
    }
    //reduce command
    else if(strcmp(command, "reduce") == 0)
    {
      int reduce;
      fscanf(inFile, "%s %d", item, &reduce);
      replaceTree(Root, group, item, reduce, outFile);
    }
    //delete item command
    else if(strcmp(command, "delete") == 0)
    {
      fscanf(inFile, "%s", item);
      deleteitemintree(Root, group, item, outFile);
    }
    //delete subtree
    else if(strcmp(command, "delete_name") == 0)
    {
      delete_Tree(Root, group, outFile);
      Delete(Root, group);
    }
    //else not a real command
    else 
    {
      printf("\nnot a real command");
    }
  }
  printf("\n");
  fprintf(outFile,"\n");
}

//freeing the tree for items
void freeTree(itemNode* root) {
  if(root != NULL) {
    freeTree(root->left);//left then right
    freeTree(root->right);
    free(root);//then the root
  }
}

//freeing the rest of the tree for name
//similar to freetree
void freeAll(treeNameNode* root) {
  if(root != NULL) {
    freeAll(root->left);
    freeAll(root->right);
    freeTree(root->Tree);
    free(root);
  }
}

int main(void) {  
  atexit(report_mem_leak);
  int N, I, Q;
  //make the files
  FILE* inFile = fopen("in.txt", "r");
  FILE* outFile = fopen("out.txt", "w");
  fscanf(inFile, "%d %d %d", &N, &I, &Q);//scan needed items
  treeNameNode* nameRoot = buildNameTree(inFile, N);//built the main tree
  populateTrees(inFile, nameRoot, I);//scan in items and built subtree

  displayTree(nameRoot, outFile);//display results
  traverse_in_traverse(nameRoot, outFile);//display result

  itemNode* root;
  Scanningcommands(inFile, Q, nameRoot,root,outFile);//commands

  freeAll(nameRoot);//free everything


  //close everything
  fclose(inFile);
  fclose(outFile);
}
