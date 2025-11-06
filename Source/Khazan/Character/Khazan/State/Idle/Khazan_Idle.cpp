// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Khazan/State/Idle/Khazan_Idle.h"

CKhazan_Idle::CKhazan_Idle()
{
}

CKhazan_Idle::~CKhazan_Idle()
{
}


CKhazan_Idle* CKhazan_Idle::Create()
{
	CKhazan_Idle* pInstance = new CKhazan_Idle();

	if(!pInstance)
	{
		return nullptr;
	}

	return pInstance;
}

void CKhazan_Idle::Free()
{
	delete this;
}
