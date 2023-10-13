#pragma mode( separator(.,;) integer(h64) )

EXPORT alpha := 0.1;
EXPORT Epsilon := 1ᴇ-5;
EXPORT Tolerance := 1ᴇ-5;
EXPORT MaxIterations := 1000;
EXPORT Batch := 100;
 
EXPORT Adagrad(ll, oo)
BEGIN
 PRINT("Initiating . . .");
 LOCAL VO;
 LOCAL VN;
 LOCAL EX := "";
 LOCAL TOS;
 LOCAL TNS;
 LOCAL Grad;
 LOCAL IT := 1, ITT := 1;

 INPUT({
  {alpha, [0]},
  {MaxIterations, [0]},
  {Batch, [0]},
  {Tolerance, [0]}
 },
 "Adagrad Optimizer");
 PRINT("α = " + STRING(alpha));
 PRINT("ε = " + STRING(Epsilon));
 PRINT("tol = " + STRING(Tolerance)); 
 LOCAL Cell := "";
 
 LOCAL BestState := GetCells(ll);
 LOCAL BestScore := GetCell(oo);
 
 LOCAL S:=SIZE(ll);
 LOCAL GradAccumulator := MAKEMAT(0,S);
 LOCAL CurrentState := MAKEMAT(0,S); 
 
 FOR IT FROM 1 TO MaxIterations DO 
  
  // get current target
  TOS := GetCell(oo);
   
  // per cell
  FOR ITT FROM 1 TO S DO
   Cell := ll[ITT];
   IFERR
     
    // get current var
    VO := GetCell(Cell);
   
    // set var + delta
    VN := VO + Epsilon;
    SetCell(Cell,VN);
   
    // get new target
    TNS := GetCell(oo);
  
    // reset values
    SetCell(Cell,VO);
    // target auto 
    
    // get gradient
    Grad := (TNS-TOS)/Epsilon;
    GradAccumulator[ITT] := GradAccumulator[ITT] + Grad^2;

    // calculate new values 
    CurrentState[ITT] := VO - alpha*Grad/√(GradAccumulator(ITT)+Epsilon); 
   
   THEN 
    SetCells(ll, BestState);  
   END;
  END;
  
  // apply vals
  SetCells(ll, CurrentState);
  
  LOCAL CurrScore := GetCell(oo);
  // check if applies
  IF IT MOD Batch == 0 THEN
   PRINT(STRING(IT) + ":  "+STRING(TOS)+" - BestScore: "+STRING(BestScore));
  ELSE IF CurrScore ≤ Tolerance THEN
   BREAK; 
  END; END;
  IF CurrScore < BestScore THEN
   BestScore := CurrScore;
   BestState := CurrentState; 
  END;
 END;
 
 MSGBOX("Finished!\n"+STRING(BestState)+"\n"+STRING(BestScore));
 
 SetCells(ll, BestState); 

END;
