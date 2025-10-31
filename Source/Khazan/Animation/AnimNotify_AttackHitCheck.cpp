// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"

//������ ���� �ٸ� ������ ����� �߰��ϴ� ���� �������� ����ٴ� �ǹ��̹Ƿ� 
//�� ��Ƽ���� ����� ��쿡�� �������� ���� ������ ĳ���Ͱ� ���� �� 
//���� ���Ǵ� ���� �����Ƿ� ������ ���� ������ ����� �߰��ϴ� ���� ���� �ٶ������� �ʱ� ������ 
//�� �� ���������� ��Ƽ���̸� ����ϱ� ���� �������̽��� Ȱ���Ѵ�.
//#include "Character/KZCharacterBase.h"

#include "Interface/KZAnimationAttackInterface.h"



void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp)
	{
		IKZAnimationAttackInterface* AttackPawn = Cast<IKZAnimationAttackInterface>(MeshComp->GetOwner());  // �������̽��� ����������� üũ
		if(AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}

}
