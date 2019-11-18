/**
 *
 * Balanced Quad Tree (pa3)
 *
 * This file will be used for grading.
 *
 */

#include "QTree.h"

// Return the biggest power of 2 less than or equal to n
int biggestPow2(int n) {
  if( n < 1 ) return 0;
  int v = 1;
  while( v <= n ) v <<= 1;
  return v >> 1;
}

QTree::Node::Node(PNG & im, pair<int,int> ul, int sz, Node *par)
  :upLeft(ul),size(sz),parent(par),nw(NULL),ne(NULL),sw(NULL),se(NULL)
{
  var = varAndAvg(im,ul,size,avg);
}

QTree::~QTree(){
  clear();
}

QTree::QTree(const QTree & other) {
  copy(other);
}


QTree & QTree::operator=(const QTree & rhs){
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}


QTree::QTree(PNG & imIn, int leafB, RGBAPixel frameC, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(true), frameColor(frameC)
{ /* YOUR CODE HERE */
  im = imIn;
  root = new Node(im, initUpLeft(), im2pow2(im), NULL);
  numLeaf = 1;
  split(root);
}


QTree::QTree(PNG & imIn, int leafB, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(false)
{ /* YOUR CODE HERE */
  im = imIn;
  frameColor = NULL;
  root = new Node(im, initUpLeft(), im2pow2(im), NULL);
  numLeaf = 1;
  split(root);
}


bool QTree::isLeaf( Node *t ) {
  /* YOUR CODE HERE */
  return t->nw == NULL && t->ne == NULL && t->sw == NULL && t->se == NULL;
}

void QTree::split( Node *t ) {

  /* YOUR CODE HERE */
  if (t->size == 1) {
    return;
  }

  // FOR BALANCED QTREES-------------------------------------------------
  // A split might cause one or two nbrs of the parent of t to split
  // to maintain balance.  Note that these two nbrs exist (unless they're
  // not in the image region) because the current set of leaves are
  // balanced.
  // if( t is a NW (or NE or SW or SE) child ) then we need to check that
  // the North and West (or North and East or South and West or
  // South and East) nbrs of t->parent have children. If they don't
  // we need to split them.


}


/* NNbr(t)
 * return the same-sized quad tree node that is north of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::NNbr(Node *t) {
  int x = (t->upLeft).first;
  int y = (t->upLeft).second;
  int h = t->size;
  return findNbr(pair<int, int>(x, y - h), h);
}

/* SNbr(t)
 * return the same-sized quad tree node that is south of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::SNbr(Node *t) {
  int x = (t->upLeft).first;
  int y = (t->upLeft).second;
  int h = t->size;
  return findNbr(pair<int, int>(x, y + h), h);
}

/* ENbr(t)
 * return the same-sized quad tree node that is east of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::ENbr(Node *t) {
  int x = (t->upLeft).first;
  int y = (t->upLeft).second;
  int h = t->size;
  return findNbr(pair<int, int>(x + h, y), h);
}

/* WNbr(t)
 * return the same-sized quad tree node that is west of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::WNbr(Node *t) {
  int x = (t->upLeft).first;
  int y = (t->upLeft).second;
  int h = t->size;
  return findNbr(pair<int, int>(x - h, y), h);
}

bool QTree::write(string const & fileName){
  /* YOUR CODE HERE */


  // include the following line to write the image to file.
  return(im.writeToFile(fileName));
}

void QTree::clear() {
  deleteNode(root);
}


void QTree::copy(const QTree & orig) {
  root = copyNode(orig.root, orig.im);
}

/* private helper functions */

void QTree::deleteNode(Node * t) {
  if (t == NULL) { return; }
  deleteNode(t->nw);
  deleteNode(t->ne);
  deleteNode(t->sw);
  deleteNode(t->se);
  t = NULL;
}

QTree::Node * QTree::copyNode(const Node * t, const PNG & im) {
  if (t == NULL) { return NULL; }
  PNG imCopy = im;
  Node * ret = new Node(imCopy, t->upLeft, t->size, t->parent);
  ret->nw = copyNode(t->nw, imCopy);
  ret->ne = copyNode(t->ne, imCopy);
  ret->sw = copyNode(t->sw, imCopy);
  ret->se = copyNode(t->se, imCopy);
  return ret;
}

int QTree::im2pow2(const PNG & im) {
  return biggestPow2(std::min((int) im.width(), (int) im.height()));
}

pair<int, int> QTree::initUpLeft() {
  return pair<int, int>(0, 0);
}

QTree::Node * QTree::findNbr(pair<int, int> ul, int h) {
  return findNbr(ul, h, root);
}

QTree::Node * QTree::findNbr(pair<int, int> ul, int h, Node * t) {
  if (t == NULL || t->size < h || isLeaf(t)) { return NULL; }
  if (t->size == h) { return t->upLeft == ul ? t : NULL; }
  int x = ul.first  - (t->upLeft).first;
  int y = ul.second - (t->upLeft).second;
  int u = t->size;
  int v = u >> 1;
  if (0 <= x && x < v && 0 <= y && y < v) { return findNbr(ul, h, t->nw); }
  if (v <= x && x < u && 0 <= y && y < v) { return findNbr(ul, h, t->ne); }
  if (0 <= x && x < v && v <= y && y < u) { return findNbr(ul, h, t->sw); }
  if (v <= x && x < u && v <= y && y < u) { return findNbr(ul, h, t->se); }
  return NULL;
}
