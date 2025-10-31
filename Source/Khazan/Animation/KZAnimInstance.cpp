// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/KZAnimInstance.h"
#include "GameFrameWork/Character.h"
#include "GameFrameWork/CharacterMovementComponent.h"	
#include "Character/KZCharacter.h"	


UKZAnimInstance::UKZAnimInstance()
{
	MovingThreshould = 3.0f;
}


void UKZAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//GetOwningActor() //����Ÿ������ ��ȯ�ǹǷ� ĳ���ͷ� ����ȯ��.

	Owner = Cast<AKZCharacter>(GetOwningActor());	
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();	
	}
}

void UKZAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


}

void UKZAnimInstance::AnimNotify_stopattack()
{
	//m_bIsAttack = false;
}
