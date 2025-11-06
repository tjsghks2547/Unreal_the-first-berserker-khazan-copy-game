// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/State/State.h"

CState::CState()
{
}

CState::~CState()
{
}



CState* CState::Create()
{
	CState* pInstance = new CState(); 

	if(!pInstance)
	{
		return nullptr; 
	}

	return pInstance;

}

void CState::Free()
{

}