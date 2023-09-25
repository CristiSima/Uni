:- dynamic detailed_mode_disabled/0.
:- ensure_loaded('files.pl').
% student file for Ultimate Tic Tac Toe implementation

% initialState/1
% initialState(-State)
% Este adevărat pentru starea inițială a jocului.
initialBoardState([['', '', ''], ['', '', ''], ['', '', '']]).
initialState(([[A, A, A], [A, A, A], [A, A, A]], x, '')) :- initialBoardState(A).
% initialState(_) :- false.

% getBoards/2
% getBoards(+State, -Boards)
% Este adevărat dacă în starea State, informațiile din tablele individuale sunt
% cele din variabila Boards.
% Boards este legată la o listă de 9 elemente, fiecare element reprezentând o tablă.
% Ordinea tablelor este cea din lista positions (din utils.pl).
% Fiecare element din listă este o listă de 9 elemente, reprezentând
% pozițiile de pe tablă, ca x, 0, sau ''.
% Pozițiile sunt în ordinea din lista positions (din utils.pl).
transformBoard([[NW, N, NE], [W, C, E], [SW, S, SE]], [NW, N, NE, W, C, E, SW, S, SE]).
getBoards(([[NW, N, NE], [W, C, E], [SW, S, SE]], _, _), [NWt, Nt, NEt, Wt, Ct, Et, SWt, St, SEt]):-
                            transformBoard(NW, NWt),
                            transformBoard(N , Nt),
                            transformBoard(NE, NEt),
                            transformBoard(W , Wt),
                            transformBoard(C , Ct),
                            transformBoard(E , Et),
                            transformBoard(SW, SWt),
                            transformBoard(S , St),
                            transformBoard(SE, SEt).

% getBoard/3
% getBoard(+State, +UPos, -Board)
% Este adebărat dacă în starea State, la poziția UPos din tabla de UTTT,
% se află tabla individuală cu reprezentarea din Board.
% Reprezentarea tablei este descrisă în predicatul getBoards/2.

getBoard(State, UPos, Board) :- getBoards(State, Boards), getPos(Boards, UPos, Board).

% getUBoard/2
% getUBoard(stare(+Board, +UboardState, +Player, +NextMoves),
% -UboardState)
% Întoarce reprezentarea UBoard-ului, indicând tablele individuale câștigate,
% remizate, sau încă în desfășurare. Reprezentarea este aceeași ca a tablelor
% individuale (vezi getBoards/2).
getUBoard(State, [S_NW, S_N, S_NE, S_W, S_C, S_E, S_SW, S_S, S_SE]) :- getBoards(State, [NW, N, NE, W, C, E, SW, S, SE]),
                        getBoardResult(NW, S_NW),
                        getBoardResult(N,  S_N ),
                        getBoardResult(NE, S_NE),
                        getBoardResult(W,  S_W ),
                        getBoardResult(C,  S_C ),
                        getBoardResult(E,  S_E ),
                        getBoardResult(SW, S_SW),
                        getBoardResult(S,  S_S ),
                        getBoardResult(SE, S_SE).


% getPos/4
% getPos(+State, +UPos, +Pos, -Cell).
% Este adevărat dacă în starea State, în tabla individuală de la poziția UPos în UBoard,
% la poziția Pos pe tablă, se află simbolul Cell (x, 0, sau '').
getPos(State, UPos, Pos, Cell) :- getBoard(State, UPos, Board), getPos(Board, Pos, Cell).

% getPos/3
% getPos(+Board, +Pos, -Cell).
% Este adevărat dacă în tabla individuală reprezentată în Board, la poziția Pos,
% se află simbolul Cell (x, 0, sau ''). Predicatul poate fi folosit și pentru UBoard, caz
% în care Cell poate fi și r.
% getPos(_, _, _) :- false.
getPos([NW, N, NE, W, C, E, SW, S, SE], nw, NW).
getPos([NW, N, NE, W, C, E, SW, S, SE], n , N ).
getPos([NW, N, NE, W, C, E, SW, S, SE], ne, NE).
getPos([NW, N, NE, W, C, E, SW, S, SE], w , W ).
getPos([NW, N, NE, W, C, E, SW, S, SE], c , C ).
getPos([NW, N, NE, W, C, E, SW, S, SE], e , E ).
getPos([NW, N, NE, W, C, E, SW, S, SE], sw, SW).
getPos([NW, N, NE, W, C, E, SW, S, SE], s , S ).
getPos([NW, N, NE, W, C, E, SW, S, SE], se, SE).

% getNextPlayer/2
% getNextPlayer(+State), -NextPlayer)
% Este adevărat dacă în starea State, jucătorul care urmează este NextPlayer
% (poate fi x sau 0)..
getNextPlayer((_, N, _), N). % :- nextPlayer(O, N).
% getNextPlayer((_, 0, _), x).

% getNextAvailableBoards/2
% getNextAvailableBoards(+State, -NextBoardsPoss)
% Este adevărat dacă în starea State, pozițiile din NextBoardsPoss sunt pozițiile
% din UBoard ale tablelor disponibile pentru următoarea mutare.
% TODO
internalGetNextAvailableBoards((Boards, S, ''), Pos) :- member(Pos, [nw, n, ne, w, c, e, sw, s, se]), getBoard((Boards, S, ''), Pos, Board), getBoardResult(Board, BoardResult), BoardResult=''.

getNextAvailableBoards((Boards, S, ''), LPos) :- findall(Pos, internalGetNextAvailableBoards((Boards, S, ''), Pos), LPos), !.
getNextAvailableBoards((Boards, S, EPos), LPos) :- getBoard((Boards, S, EPos), EPos, NextBoard), getBoardResult(NextBoard, NextBoardResult), dif(NextBoardResult, ''),
                                                findall(Pos, internalGetNextAvailableBoards((Boards, S, ''), Pos), LPos), !.
getNextAvailableBoards((_, _, Pos), [Pos]).

% getBoardResult/2
% getBoardResult(+Board, -Result)
% Este adevărat dacă pentru o tablă individuală (sau UBoard) cu reprezentarea
% din Board, rezultatul este Result. Result poate fi:
% x sau 0, dacă jucătorul respectiv a câștigat jocul pe tabla dată;
% r, dacă s-a ajuns la remiză (toate pozițiile au fost completate dar
% tabla nu a fost câștigată);
% '', dacă tabla nu a fost câștigată și nu s-au completat toate pozițiile.
% NOTĂ: este deja definit predicatul player_wins/2 în utils.pl.
getBoardResult(Board, P) :- player_wins(P, Board), !.
getBoardResult(Board, r) :- not(member('', Board)), !.
getBoardResult(Board, '').

% buildState/3
% buildState(+Boards, +PreviousPos, -State)
% Este adevărat dacă starea State corespunde stării jocului în care tablele
% individuale sunt cele din lista Boards, iar ultima mutare a fost în
% poziția PreviousPos într-o tablă individuală.
% NOTĂ: nu contează în care tablă individuală s-a realizat ultima mutare.

count([],X,0).
count([X|T],X,Y):- count(T,X,Z), Y is 1+Z.
count([X1|T],X,Z):- X1\=X,count(T,X,Z).

doubleCount([],Y,0).
doubleCount([X|T],Y,R):- doubleCount(T,Y,RR), count(X, Y, Z), R is RR+Z.

calcNextPlayer(BoardsList, 0) :- doubleCount(BoardsList, x, Xs), doubleCount(BoardsList, 0, Zs), Zs < Xs, !.
calcNextPlayer(BoardsList, x).


buildState(BoardsList, Pos, (Board, NP, Pos)) :- getBoards((Board, _, _), BoardsList), calcNextPlayer(BoardsList, NP).

% validMove/2
% validMove(+State, +Move)
% Este adevărat dacă mutarea Move este legală în starea State.
% Move este fie o poziție, în cazul în care este o singură tablă disponibilă
% pentru a următoarea mutare din starea State, fie o pereche de poziții, altfel.

% NEFOLOSIT, Nu mai fac upload
validMoveCheck('', UPos).
validMoveCheck(UPos, UPos).


validMove((SBoards, SPlayer, SUPos), Pos) :- getBoard((SBoards, SPlayer, SUPos), SUPos, Board), getBoardResult(Board, BoardResult), BoardResult='', !,
                                                                                                                                            member(Pos, [nw, n, ne, w, c, e, sw, s, se]), getPos((SBoards, SPlayer, SUPos), SUPos, Pos, BR), BR=''.
validMove((SBoards, SPlayer, SUPos), (UPos, Pos)) :- getNextAvailableBoards((SBoards, SPlayer, SUPos), ValidUPos), member(UPos, ValidUPos), member(Pos, [nw, n, ne, w, c, e, sw, s, se]), getPos((SBoards, SPlayer, SUPos),  UPos, Pos, BoardResult), BoardResult=''.

% makeMove/3
% makeMove(+State, +Move, -NewState)
% Este adevărat dacă în urma aplicării mutării Move în starea State
% rezulta starea NewState.
% Move este fie o poziție (din lista positions), în cazul în care nu sunt mai
% multe table disponibile pentru a următoarea mutare din starea State,
% fie o pereche de poziții, altfel.
%
% Hint: folosiți validMove pentru a verifica mutarea și buildState pentru a construi o stare.

setPos([NW, N, NE, W, C, E, SW, S, SE], nw, New, [New, N  , NE , W  , C  , E  , SW , S  , SE ]).
setPos([NW, N, NE, W, C, E, SW, S, SE], n , New, [NW , New, NE , W  , C  , E  , SW , S  , SE ]).
setPos([NW, N, NE, W, C, E, SW, S, SE], ne, New, [NW , N  , New, W  , C  , E  , SW , S  , SE ]).
setPos([NW, N, NE, W, C, E, SW, S, SE], w , New, [NW , N  , NE , New, C  , E  , SW , S  , SE ]).
setPos([NW, N, NE, W, C, E, SW, S, SE], c , New, [NW , N  , NE , W  , New, E  , SW , S  , SE ]).
setPos([NW, N, NE, W, C, E, SW, S, SE], e , New, [NW , N  , NE , W  , C  , New, SW , S  , SE ]).
setPos([NW, N, NE, W, C, E, SW, S, SE], sw, New, [NW , N  , NE , W  , C  , E  , New, S  , SE ]).
setPos([NW, N, NE, W, C, E, SW, S, SE], s , New, [NW , N  , NE , W  , C  , E  , SW , New, SE ]).
setPos([NW, N, NE, W, C, E, SW, S, SE], se, New, [NW , N  , NE , W  , C  , E  , SW , S  , New]).

setPos(Boards, UPos, Pos, New, BoardsRez) :- getPos(Boards, UPos, Board), setPos(Board, Pos, New, BoardRez), setPos(Boards, UPos, BoardRez, BoardsRez).

% makeMove((SBoards, SPlayer, SUPos),         Pos, NewState) :- validMove((SBoards, SPlayer, SUPos), (SUPos, Pos)),  getBoards((SBoards, _, _), Boards), buildState(Boards, Pos, NewState).
makeMove((SBoards, SPlayer, SUPos), (UPos, Pos), NewState) :- validMove((SBoards, SPlayer, SUPos), ( UPos, Pos)),  getBoards((SBoards, _, _), Boards), nextPlayer(SPlayer, NextPlayer), setPos(Boards,  UPos, Pos, SPlayer, NewBoards), buildState(NewBoards, Pos, NewState).
makeMove((SBoards, SPlayer, SUPos),         Pos, NewState) :- validMove((SBoards, SPlayer, SUPos),         Pos),   getBoards((SBoards, _, _), Boards), nextPlayer(SPlayer, NextPlayer), setPos(Boards, SUPos, Pos, SPlayer, NewBoards), buildState(NewBoards, Pos, NewState).

% dummy_first/2
% dummy_first(+State, -NextMove)
% Predicatul leagă NextMove la următoarea mutare pentru starea State.
% Strategia este foarte simplă: va fi aleasă cea mai din stânga-sus mutare posibilă
% (prima din lista de poziții disponibile).
dummy_first(S, M) :- findnsols(1, Move, validMove(S, Move), [M|_]).

% dummy_last/2
% dummy_last(+State, -NextMove)
% Predicatul leagă NextMove la următoarea mutare pentru starea State.
% Strategia este foarte simplă: va fi aleasă cea mai din dreapta-jos mutare posibilă
% (ultima din lista de poziții disponibile).
dummy_last(S, M) :- findall(Move, validMove(S, Move), Moves), last(Moves, M).
