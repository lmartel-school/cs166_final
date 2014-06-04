Editor = function(parent, rope, vis) {
  this.container = parent;
  this.rope = rope;
  this.vis = vis;
  this.lastChar = new Character(' ');
  this.lastChar.addClass(CURSOR);
  parent.append(this.lastChar.elems[0]);
  this.chars = [this.lastChar];
  this.cursor = 0;

  // Setup key listeners
  var editor = this;
  $('body').keypress(function(e) {
    editor.keypress(e);
  });
  // Special case for backspace
  $('body').keydown(function(e) {
    console.log(e.which);
    switch (e.which) {
      case 8:
        editor.keypress(e);
        e.preventDefault();
        break;
      case 37:
      case 38:
      case 39:
      case 40:
        editor.moveCursor(e.which);
    }
  });
};

Editor.prototype.moveCursor = function(dir) {
  if (dir == 37 || dir == 39) {
    dir = (dir == 37) ? -1 : 1;
    if (0 <= this.cursor + dir && this.cursor + dir < this.chars.length) {
      this.chars[this.cursor].removeClass(CURSOR);
      this.cursor += dir;
      this.chars[this.cursor].addClass(CURSOR);
    }
  } else if (dir == 38) {
    var newCursor = this.cursor;
    var curPos = this.chars[this.cursor].position(dir);
    while (0 <= newCursor - 1) {
      var newPos = this.chars[newCursor - 1].position(dir);
      if (newPos.left == curPos.left ||
          (newPos.left < curPos.left &&
           newPos.top < curPos.top)) {
        this.chars[this.cursor].removeClass(CURSOR);
        this.cursor = newCursor - 1;
        this.chars[this.cursor].addClass(CURSOR);
        break;
      }
      newCursor -= 1;
    }
  } else {
    var newCursor = this.chars.length - 1;
    var curPos = this.chars[this.cursor].position(dir);
    while (0 <= newCursor - 1) {
      var newPos = this.chars[newCursor - 1].position(dir);
      if (newPos.left <= curPos.left &&
          newPos.top <= curPos.top + 85) {
        this.chars[this.cursor].removeClass(CURSOR);
        this.cursor = newCursor - 1;
        this.chars[this.cursor].addClass(CURSOR);
        break;
      }
      newCursor -= 1;
    }
  }
};

Editor.prototype.keypress = function(e) {
  var ch;
  switch (e.which) {
    case 8:  e.preventDefault();
             this.removeCharacter(this.cursor - 1);
             if (this.cursor != 0) this.cursor--;
             return;
    case 13: if (this.cursor == this.chars.length - 1) return;
             this.rope.query(this.cursor, this.vis);
             return;
    default: ch = String.fromCharCode(e.which);
             break;
  }
  this.addCharacter(this.cursor, ch);
  this.cursor++;
};

Editor.prototype.removeCharacter = function(index) {
  if (index == -1) return;
  this.chars[index].remove();
  this.chars.splice(index, 1);

  this.rope.remove(index, this.vis);
};

Editor.prototype.addCharacter = function(index, ch) {
  newCh = new Character(ch);
  newCh.insertBefore(this.chars[index]);
  this.chars.splice(index, 0, newCh);

  this.rope.insert(index, ch, this.vis);
};

Character = function(ch) {
  this.character = ch;
  if (ch == '\n') {
    space = $('<span>').addClass(CHARACTER).text('\u240d');
    newline = $('<span>').addClass(CHARACTER).text(' ').addClass(NEWLINE);
    this.elems = [space, newline];
  } else {
    this.elems = [$('<span>').addClass(CHARACTER).text(ch)]
  }
};

Character.prototype.firstElem = function() {
  return this.elems[0];
};

Character.prototype.insertBefore = function(nextChar) {
  for (var i = 0; i < this.elems.length; i++) {
    nextChar.elems[0].before(this.elems[i]);
  }
};

Character.prototype.remove = function() {
  for (var i = 0; i < this.elems.length; i++) {
    this.elems[i].remove();
  }
};

Character.prototype.position = function() {
  return this.elems[0].position();
};

Character.prototype.addClass = function(className) {
  for (var i = 0; i < this.elems.length; i++) {
    this.elems[i].addClass(className);
  }
};

Character.prototype.removeClass = function(className) {
  for (var i = 0; i < this.elems.length; i++) {
    this.elems[i].removeClass(className);
  }
};


CHARACTER = 'char';
CURSOR = 'cursor';
NEWLINE = 'newline';
ENTER = 13;
