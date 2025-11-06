// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Khazan/State/Run/Khazan_Run.h"

CKhazan_Run::CKhazan_Run()
{
}

CKhazan_Run::~CKhazan_Run()
{
}


CKhazan_Run* CKhazan_Run::Create()
{
	CKhazan_Run* pInstance = new CKhazan_Run(); 

	if(!pInstance)
	{
		FMessageDialog::Open(
			EAppMsgType::Ok,
			FText::FromString(TEXT("Failed To Create : CKhazan_Run"))
		);
		return nullptr;
	}

	return pInstance;

}

void CKhazan_Run::Free()
{
	delete this;
}
