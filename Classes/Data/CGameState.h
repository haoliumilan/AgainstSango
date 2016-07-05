//
//  CGameState.h
//  CardDemo
//
//  Created by Hao Zhang on 12-4-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_CGameState_h
#define CardDemo_CGameState_h

enum enGameState{
    enStateNull = 0,
    enStateChgStone,
    enStateWaitTouch,
    enStateTouching,
    enStateTouchEnd,
    enStateClrCombo,
    enStatePlayerAttackPrepared,
    enStatePlayerAttack,
    enStateEnemyAttack,
    enStateEnemyDead,
    enStateGameOver,
    enStateEnemyDeadGameOver,
};

class CGameState {
    static CGameState* s_pcIf;
    
    
    
    enGameState state;
    
public:
    
    
    
	//Get the instance pointer
	static CGameState* Instance(){
        if(s_pcIf == NULL){
            s_pcIf = new CGameState();
        }
        
        return s_pcIf;
    }
    
    void setState(enGameState st){
        
        if (st == enStateGameOver && state == enStateEnemyDead)
        {
            state = enStateEnemyDeadGameOver;
            return;
        }
        
        if (st == enStateEnemyDead && state == enStateGameOver)
        {
            state = enStateEnemyDeadGameOver;
            return;
        }

        state = st;
    }
    
    enGameState getState(){
        return state;
    }
    
private:
    //forbid constructor being called outside
	CGameState(){};
    
	//forbid copy constructor being called outside
	CGameState(const CGameState& );
	
	//forbid assignment
	CGameState& operator=(const CGameState& );
    
};

#endif
