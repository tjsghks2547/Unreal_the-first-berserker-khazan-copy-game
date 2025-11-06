// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Khazan/State/KzStateMgr.h"
// 각 state import 구간 
#include "Idle/Khazan_Idle.h"
#include "Run/Khazan_Run.h"

CKzStateMgr::CKzStateMgr()
{
	// idle state 추가 
	if(CKhazan_Idle* pKzIdleInstance = CKhazan_Idle::Create())
		m_mapState.Add(PLAYER_STATE::IDLE, pKzIdleInstance);
	
	// Run State 추가 
	if (CKhazan_Run* pKzRunInstance = CKhazan_Run::Create())
		m_mapState.Add(PLAYER_STATE::RUN, pKzRunInstance);



}

CKzStateMgr::~CKzStateMgr()
{

}


CKzStateMgr* CKzStateMgr::Create()
{
	CKzStateMgr* pInstance = new CKzStateMgr(); 

	if(!pInstance)
	{
		return nullptr; 
	}

	return pInstance; 
}