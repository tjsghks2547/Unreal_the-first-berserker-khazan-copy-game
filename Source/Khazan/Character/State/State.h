// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class KHAZAN_API CState
{
protected:
	CState();
	virtual ~CState();


public:
	static CState* Create();
	virtual void Free();

};
