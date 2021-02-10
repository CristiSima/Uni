style="{BasedOnStyle: LLVM,IndentWidth: 4,AllowShortLoopsOnASingleLine: false,AllowShortIfStatementsOnASingleLine: Never,BraceWrapping: {BeforeElse: false,AfterFunction: false},TabWidth: 4,UseTab: Always,BreakBeforeBraces: Linux,MaxEmptyLinesToKeep: 1,IndentCaseLabels: false,AllowShortIfStatementsOnASingleLine: false}"
clang-format -style="$style" temp.h > temp
