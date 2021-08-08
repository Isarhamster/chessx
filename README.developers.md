**Note:** If you start working with code, please read though the following rules:

### 1. Official ChessX's source code at

https://github.com/Isarhamster/chessx

### 2. Use Qt naming scheme.

You may have a look at the [Qt API](https://doc.qt.io/qt-5/) to learn it
 
Here are some examples:

* _Classes:_ `BoardView` , `Filter`
* _Classes with acronyms:_ `PgnDatabase`
* _Class variables:_ `m_list`, `m_darkSquareColor`
* _Reading class variable:_ `QString title() const`
* _Setting class variable:_ `void setTitle(const QString& s) const`
* _Checks:_ `isReadOnly()`
* _Functions with acronyms:_ `moveToSan()`, `fromSan()`

### 3. Use Doxygen format to comment your code

* Start a class documentation with the [Doxygen](https://www.doxygen.nl/index.html) header

```
  /** @ingroup Database
    The Board class represents a chess position.
    Pieces are kept directly on the board which is internally represented as simple array[0..63].
    You can easily and quickly make and undo moves, although undoing moves requires keeping track
    of captured pieces.
  */
```

  * **@ingroup** should contain `Database` (for non-GUI classes) or `GUI`
  * Next sentence should have `The Foo class represents` or `... is` syntax
  
* Document each function 
```

  /** Sets widget color. */ 
  void setColor(const QColor& color);
  /** @return widget color. */ 
  QColor color() const;
```
  
* Rebuild documentation with 

`$ doxygen Doxyfile`

and verify that your class is documented correctly.

### 4. Format your code with tabs

We advocate using [astyle](http://astyle.sourceforge.net) to indent your code. To use astyle, put

`
suffix=none style=linux mode=c force-indent=tab=4 one-line=keep-blocks unpad=paren pad=oper
`

in `~/.astylerc`.  Then you can indent your code using: 

`
$ astyle <file1> <file2> ...
`
