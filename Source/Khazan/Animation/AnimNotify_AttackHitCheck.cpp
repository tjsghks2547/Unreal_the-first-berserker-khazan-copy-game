// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"

//다음과 같이 다른 폴더의 헤더를 추가하는 것은 의존성이 생긴다는 의미이므로 
//이 노티파이 기능의 경우에는 공용으로 여러 종류의 캐릭터가 있을 때 
//같이 사용되는 것이 좋으므로 다음과 같이 때마다 헤더를 추가하는 것을 별로 바람직하지 않기 때문에 
//좀 더 범용적으로 노티파이를 사용하기 위해 인터페이스를 활용한다.
//#include "Character/KZCharacterBase.h"

#include "Interface/KZAnimationAttackInterface.h"



void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp)
	{
		IKZAnimationAttackInterface* AttackPawn = Cast<IKZAnimationAttackInterface>(MeshComp->GetOwner());  // 인터페이스가 구현됬는지를 체크
		if(AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}

}
