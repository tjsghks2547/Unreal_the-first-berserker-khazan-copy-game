// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/KZAnimInstance.h"

/**
 * 
 */
class KHAZAN_API CStateManager
{
protected:
	CStateManager();
	virtual ~CStateManager();


public:
	void Add_State(PLAYER_STATE _PLAYER_STATE, class CState* _pState) { m_mapState.Add(_PLAYER_STATE, _pState); }
	


protected: 
	TMap<PLAYER_STATE,class CState*> m_mapState; 



public:
	static CStateManager* Create(); 


};
