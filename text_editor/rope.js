Rope = function() {
  this.str = "";
};

Rope.prototype.insert = function(pos, ch) {
  this.str = this.str.substring(0, pos) + ch + this.str.substring(pos);
};

Rope.prototype.remove = function(pos) {
  this.str = this.str.substring(0, pos) + this.str.substring(pos + 1);
};

Rope.prototype.string = function() {
  return this.str;
};

Rope.prototype.toJSON = function() {
  return splitString(this.str);
}

function splitString(str) {
  if (str.length <= 3) {
    return {"name":str};
  }
  var split = str.length / 2;
  return {"name" : "Weight: " + str.length,
          "children" : [
            splitString(str.substring(0, split)),
            splitString(str.substring(split))
          ]
  };
}
