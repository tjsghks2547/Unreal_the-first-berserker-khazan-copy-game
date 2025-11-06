// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/State/StateManager.h"
#include "State.h"

CStateManager::CStateManager()
{

}

CStateManager::~CStateManager()
{

}


CStateManager* CStateManager::Create()
{
	CStateManager* pInstance = new CStateManager(); 

	if(!pInstance)
	{
		return nullptr; 
	}

	return pInstance; 

}