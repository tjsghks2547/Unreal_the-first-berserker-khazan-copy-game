// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/State/State.h"
/**
 * 
 */
class KHAZAN_API CKhazan_Idle : public CState
{
private:	
	CKhazan_Idle();
	virtual ~CKhazan_Idle();



public:
	static CKhazan_Idle* Create();
	virtual void Free() override;

};
