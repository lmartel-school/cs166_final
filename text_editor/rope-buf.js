RopeBuf = function(bufSize, splay) {
  this.bufSize = bufSize;
  this.root = new Node("");
  this.shouldSplay = splay;
};

RopeBuf.prototype.insert = function(pos, ch, vis) {
  var parent = null;
  var node = this.root;
  var done = false;
  var dir;
  while (node != null) {
    var size = node.str.length;
    // New char MUST go in left subtree
    if (pos < node.leftSize) {
      node.leftSize++;
      parent = node;
      dir = "left";
      node = node.left;

    // New char MUST go in right subtree
    } else if (pos > node.leftSize + size) {
      pos -= node.leftSize + size;
      parent = node;
      dir = "right";
      node = node.right;

    // New character either goes at beginning of cur node or in left subtree
    } else if (pos == node.leftSize) {
      // We can fit it in this node.
      if (node.str.length < this.bufSize) {
        node.str = ch + node.str;
        done = true;
        break;

      // We have to put it at end of left subtree
      } else {
        node.leftSize++;
        parent = node;
        dir = "left";
        node = node.left;
      }

    // New character either goes at end of cur node or in right subtree
    } else if (pos == node.leftSize + size) {
      // We can fit it in this node.
      if (node.str.length < this.bufSize) {
        node.str += ch;
        done = true;
        break;

      // We have to put it at end of left subtree
      } else {
        parent = node;
        dir = "right";
        node = node.right;
        pos = 0;
      }

    // Final case, character must go in current node. If we can't we fit it
    // in, then we break it into two pieces.
    } else {
      pos -= node.leftSize;
      var fullStr = node.str.substring(0, pos) + ch + node.str.substring(pos);
      // There is room
      if (node.str.length < this.bufSize) {
        node.str = fullStr;

      // Breaking apart and replacing the node with just a single character.
      } else {
        var newLeft = new Node(fullStr.substring(0, this.bufSize / 2));
        var newRight = new Node(fullStr.substring(this.bufSize / 2 + 1));

        newLeft.left = node.left;
        newLeft.right = null;
        newLeft.parent = node;
        newLeft.leftSize = node.leftSize;

        newRight.left = null;
        newRight.right = node.right;
        newRight.parent = node;
        newRight.leftSize = 0;

        node.left = newLeft;
        node.right = newRight;
        node.leftSize = newLeft.leftSize + newLeft.str.length;
        node.str = "" + fullStr.substr(this.bufSize / 2, 1);
      }
      done = true;
      break;
    }
  }

  if (!done) {
    // We've got to a point where we want to insert where there isn't anything.
    var newNode = new Node("" + ch);
    newNode.parent = parent;
    if (dir == "left") {
      parent.left = newNode;
    } else {
      parent.right = newNode;
    }
    node = newNode;
  }

  if (this.shouldSplay) this.splay(node);

  vis.redraw(this.toJSON());
};

RopeBuf.prototype.remove = function(pos, vis) {
  var dir;
  var node = this.root;

  while (true) {
    // Character is in left side
    if (pos < node.leftSize) {
      node.leftSize--;
      node = node.left;
      dir = "left";

    // Character is in right wide
    } else if (pos >= node.leftSize + node.str.length) {
      pos -= node.leftSize + node.str.length;
      node = node.right;
      dir = "right";

    // Character is in this node
    } else {
      pos -= node.leftSize;
      node.str = node.str.substring(0, pos) + node.str.substring(pos + 1);
      break;
    }
  }

  if (this.shouldSplay) this.splay(node);

  if (node.str.length == 0) this.removeNode(node, dir);

  vis.redraw(this.toJSON());
};

RopeBuf.prototype.query = function(pos, vis) {
  var node = this.root;
  while (true) {
    if (pos < node.leftSize) {
      node = node.left;
    } else if (pos >= node.leftSize + node.str.length) {
      node = node.right;
    } else {
      break;
    }
  }
  this.splay(node);
  vis.redraw(this.toJSON());
}

RopeBuf.prototype.removeNode = function(node, dir) {
  // Special case don't delete root when it's all that's left
  if (node == this.root && node.left == null && node.right == null) return;

  // Node has no children, set parent to null
  if (node.left == null && node.right == null) {
    if (dir == "left") node.parent.left = null;
    else node.parent.right = null;

  // If one child is null we can replace with non-null child
  } else if (node.left == null) {
    if (node == this.root) {
      this.root = node.right;
    } else {
      if (dir == "left") {
        node.parent.left = node.right;
      } else {
        node.parent.right = node.right;
      }
    }
    node.right.parent = node.parent;
  } else if (node.right == null) {
    if (node == this.root) {
      this.root = node.left;
    } else {
      if (dir == "left") {
        node.parent.left = node.left;
      } else {
        node.parent.right = node.left;
      }
    }
    node.left.parent = node.parent;

  // Otherwise both children exist. We'll take the rightmost node of the
  // left child and bring it up to replace us. (That way we have fewer
  // leftSizes to update.
  } else {
    var predecessor = node.left;
    while (predecessor.right != null) predecessor = predecessor.right;

    this.removeNode(predecessor, "left");

    // Set up predecessor's size.
    predecessor.leftSize = node.leftSize - predecessor.str.length;

    // Set up predecessor's children and parent
    predecessor.right = node.right;
    node.right.parent = predecessor;
    predecessor.left = node.left;
    if (node.left != null) node.left.parent = predecessor;
    predecessor.parent = node.parent;

    // Now let node's parent know.
    if (node == this.root) {
      this.root = predecessor;
    } else {
      if (dir == "left") {
        node.parent.left = predecessor;
      } else {
        node.parent.right = predecessor;
      }
    }
  }
};

RopeBuf.prototype.string = function() {
  return nodeToString(this.root);
};

RopeBuf.prototype.toJSON = function() {
  return nodeToJSON(this.root);
};

Node = function(str) {
  this.leftSize = 0;
  this.str = str;
  this.left = null;
  this.right = null;
  this.parent = null;
};

nodeToString = function(node) {
  if (node == null) return "";
  return nodeToString(node.left) + node.str + nodeToString(node.right);
};

nodeToJSON = function(node) {
  if (node == null) return {"name":"\u2718"};
  var escapedStr = node.str.replace(/\n/, '\\n');
  while (escapedStr != escapedStr.replace(/\n/, '\\n'))
    escapedStr = escapedStr.replace(/\n/, '\\n');
  var json = {"name" : "'" + escapedStr + "'"};
  var children = [];
  children.push(nodeToJSON(node.left));
  children.push(nodeToJSON(node.right));
  if (children.length > 0) json["children"] = children;
  return json;
};

RopeBuf.prototype.splay = function(node) {
  while (node.parent != null) {
    /* ZIG CASE */
    if (node.parent.parent == null) {
      this.rotate(node);
    } else {
      var parent = node.parent;
      /* ZIG-ZIG CASE */
      if (   (node == parent.left  && parent == parent.parent.left )
          || (node == parent.right && parent == parent.parent.right)) {
        this.rotate(parent);
        this.rotate(node);
      } else {
      /* ZIG-ZAG CASE */
        this.rotate(node);
        this.rotate(node);
      }
    }
  }
  this.root = node;
};

RopeBuf.prototype.rotate = function(child) {
  var parent = child.parent;
  if (child == parent.left) {
    parent.left = child.right;
    /* Fix that child's parent if we can. */
    if (child.right != null) {
      child.right.parent = parent;
    }
    child.right = parent;

    // Update leftSize values:
    parent.leftSize -= child.leftSize + child.str.length;
  } else {
    parent.right = child.left;
    /* Fix that child's parent if we can. */
    if (child.left != null) {
      child.left.parent = parent;
    }
    child.left = parent;

    // Update leftSize values:
    child.leftSize += parent.leftSize + parent.str.length;
  }

  child.parent = parent.parent;
  parent.parent = child;

  /* Adjust original grandparent (new parent) */
  if (child.parent != null) {
    if (child.parent.left == parent) {
      child.parent.left = child;
    } else {
      child.parent.right = child;
    }
  }
}
