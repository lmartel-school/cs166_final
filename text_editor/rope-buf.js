RopeBuf = function(bufSize) {
  this.bufSize = bufSize;
  this.root = new Node("");
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
    if (dir == "left") {
      parent.left = newNode;
    } else {
      parent.right = newNode;
    }
  }

  vis.redraw(this.toJSON());
};

RopeBuf.prototype.remove = function(pos, vis) {
  common.naclModule.postMessage('r' + pos + ' ' + (pos + 1));
  common.naclModule.callback = fn;
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
  if (node == null) return {"name":"NULL"};
  var escapedStr = node.str.replace(/\n/, '\\n');
  while (escapedStr != escapedStr.replace(/\n/, '\\n'))
    escapedStr = escapedStr.replace(/\n/, '\\n');
  var json = {"name" : escapedStr};
  var children = [];
  children.push(nodeToJSON(node.left));
  children.push(nodeToJSON(node.right));
  if (children.length > 0) json["children"] = children;
  return json;
};
