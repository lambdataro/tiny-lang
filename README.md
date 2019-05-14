# tiny-lang

小さい自作言語のコンパイラ

## 環境

- Windows 10
- CMake 3.14.2
- mingw-64 8.1.0
- flat assembler 1.73.11

## コンパイル

```sh
$ cmake .
$ make
```

## 文法
```
プログラム ::= print(式)
式 ::= <文字列>
```
