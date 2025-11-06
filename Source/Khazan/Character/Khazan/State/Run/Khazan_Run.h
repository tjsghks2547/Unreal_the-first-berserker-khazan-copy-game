// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/State/State.h"	


class KHAZAN_API CKhazan_Run : public CState 
{
private:
	CKhazan_Run();
	virtual ~CKhazan_Run();


public:
	static CKhazan_Run* Create(); 
	virtual void Free() override; 

};
