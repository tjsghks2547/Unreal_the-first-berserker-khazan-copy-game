// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/KZAnimInstance.h"
#include "GameFrameWork/Character.h"
#include "GameFrameWork/CharacterMovementComponent.h"	
#include "Character/Khazan/KZCharacter.h"	


UKZAnimInstance::UKZAnimInstance()
{
	MovingThreshould = 3.0f;
}


void UKZAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//GetOwningActor() //엑터타입으로 반환되므로 캐릭터로 형변환함.

	Owner = Cast<AKZCharacter>(GetOwningActor());	
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();	
	}
}

void UKZAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner)
	{
		FRotator ControlRot = Owner->GetControlRotation(); // 카메라 방향 
		FRotator ActorRot = Owner->GetActorRotation(); // 캐릭터가 현재보고있는 방향

		// 카메라 방향 - 캐릭터 방향 => Yaw 차이 구해서 DeltaYaw만들기 

		// ActorYaw에서 ControlYaw까지의 최소 회전 각도(-180 ~ 180)
		float DeltaYaw = FMath::FindDeltaAngleDegrees(ActorRot.Yaw, ControlRot.Yaw);

		// 이제 이걸로 조건 체크
		if (abs(DeltaYaw) >= 150.0f)
		{
			// 왼쪽으로 90도 이상 차이남 → 왼쪽 턴 애니
			m_bLeftTurnOnOff = true;	

		}

		if (abs(DeltaYaw) <= 10.0f)
			m_bLeftTurnOnOff = false;
		
	}

}

void UKZAnimInstance::AnimNotify_stopattack()
{
	//m_bIsAttack = false;
}
