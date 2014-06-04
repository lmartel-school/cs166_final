Rope = function() {
};

Rope.prototype.insert = function(pos, ch, vis) {
  common.naclModule.postMessage('i' + pos + ' ' + ch);
  var r = this;
  common.naclModule.callback = function(ignored) {
    r.toJSON(function(objStr) {
      vis.redraw(JSON.parse(objStr));
    });
  };
};

Rope.prototype.remove = function(pos, vis) {
  common.naclModule.postMessage('r' + pos + ' ' + (pos + 1));
  var r = this;
  common.naclModule.callback = function(ignored) {
    r.toJSON(function(objStr) {
      vis.redraw(JSON.parse(objStr));
    });
  };
};

Rope.prototype.query = function(index, vis) {
  common.naclModule.postMessage('q' + index);
  var r = this;
  common.naclModule.callback = function(ignored) {
    r.toJSON(function(objStr) {
      vis.redraw(JSON.parse(objStr));
    });
  };
}

Rope.prototype.string = function(fn) {
  common.naclModule.postMessage('g');
  common.naclModule.callback = fn;
};

Rope.prototype.toJSON = function(fn) {
  common.naclModule.postMessage('j');
  common.naclModule.callback = fn;
}

function splitString(str) {
  if (str.length <= 3) {
    return {"name":str.replace(/\n/, '\\n')};
  }
  var split = str.length / 2;
  return {"name" : "Weight: " + str.length,
          "children" : [
            splitString(str.substring(0, split)),
            splitString(str.substring(split))
          ]
  };
}
