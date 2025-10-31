// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KZCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "KZComboActionData_1.h"
#include "Physics/KZCollision.h"
#include "Animation/KZAnimInstance.h"



// Sets default values
AKZCharacterBase::AKZCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	//Pawn 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	


	//CapSule 
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));


	//Movement 
	m_pMoveComponent = GetCharacterMovement();

	m_pMoveComponent->bOrientRotationToMovement = true; 
	// 이동하는 방향(속도 벡터)을 바라보게 캐릭터의 Yaw 회전 자동 조절
	m_pMoveComponent->RotationRate = FRotator(0.0f, 2000.f, 0.0f);	
	// bOrientRotationToMovement()가 true일 때 방향 전환 시 1초에 몇도 속도(Yaw 속도) 로 회전할 지를 결정 
	m_pMoveComponent->JumpZVelocity = 700.f; 
	// 점프 시작 시 위쪽(Z)으로 주는 초기속도
	m_pMoveComponent->AirControl = 0.35f; 
	// 공중에서 이동 입력 반응 정도 ( 0 = 방향 거의 못 틀음 , 1 = 지상과 동일한 제어 가능) 
	m_pMoveComponent->MaxWalkSpeed = 1000.f; 
	// 기본 지상 이동 최대 속도 (cm/s)
	m_pMoveComponent->MinAnalogWalkSpeed = 0.f; 
	// 입력이 아주 미세하게 들어왔을 때 보장되는 최소 속도 
	m_pMoveComponent->BrakingDecelerationWalking = 350.F;
	// 키를 뗐을 때 서서히 멈추는 감속 가속도
	m_pMoveComponent->MaxAcceleration = 100000.f;	
	// 최대 가속도로 입력이 들어오면 해당 값만큼 속도를 키움 


	


	//Mesh 
	m_pSkeletalMesh = GetMesh(); 
	m_pSkeletalMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.f, 0.0f));

	//Animation 
	m_pSkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_pSkeletalMesh->SetCollisionProfileName(TEXT("ChararcterMesh"));


	//USkeletalMesh 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/Character/Mesh/Kazan/Kazna_model.Kazna_model"));
	if (CharacterMeshRef.Object)
	{
		m_pSkeletalMesh->SetSkeletalMesh(CharacterMeshRef.Object);
	}


	// 일반 리소스는 FObjectFinder로 찾으며 ( 메시, 머터리얼, 텍스처) 
	// 블루 프린트 리소스는 FClassFinder로 찾는다 ( 블루 프린트, actor, pawn 등 ) 

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/BP_KhzanAnimation.BP_KhzanAnimation_C"));
	if (AnimInstanceClassRef.Class)
	{
		m_pSkeletalMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}


	// AnimMontage and Asset Data.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Game/Animation/ComboAttackMontage.ComboAttackMontage"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UKZComboActionData_1> ComboActionDataRef(TEXT("/Game/Character_Action/KZComboAction_1.KZComboAction_1"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

}

void AKZCharacterBase::ProcessComboCommand()
{
	if (m_iCurrentCombo == 0)
	{
		ComboActionBegin(); 
		return; 
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNexComboCommand = false; 
	}
	
	else
	{
		HasNexComboCommand = true; 
	}



}

void AKZCharacterBase::ComboActionBegin()
{
	// Combo Status

	m_iCurrentCombo = 1; 

	// Movement Setting 
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);	// 이동 기능이 사라짐 

	// Animation Setting
	const float AttackSpeedRate = 1.0f; 
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate; 
	EndDelegate.BindUObject(this, &AKZCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer(); 


}

void AKZCharacterBase::ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(m_iCurrentCombo != 0); 
	m_iCurrentCombo = 0; 

	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);	

}

void AKZCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = m_iCurrentCombo - 1; 
	
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f; 

	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;

	if(ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AKZCharacterBase::ComboCheck, ComboEffectiveTime, false); 
		// 여기서 ComboEffectiveTime 값의 "second"로 예약되어 second 만큼 지난 뒤에 ComboCheck 해당 함수가 발동됨.  
		// 여기서 다시 ComboTimerHandle이 Valid가 되어 true가 된다.
	}

}

void AKZCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate(); // 콤포 타이머 핸들을 초기화 

	if(HasNexComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		m_iCurrentCombo = FMath::Clamp(m_iCurrentCombo + 1, 1, ComboActionData->MaxComboCount);	
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, m_iCurrentCombo);

		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);

		SetComboCheckTimer();

		HasNexComboCommand = false; // 입력값 초기화
	}

}

void AKZCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this); //Trace Tage : 콜리전 분석할때의 태그 정보 , 복잡한 형태의 충돌체 캡슐이나를 감지할지, inignoreActor 무시할 엑터(자기자신이니깐 this넣음)

	const float AttackRange  = 40.0f; 
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f; 

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();  // 투사 시작 지점
	const FVector End = Start + GetActorForwardVector() * AttackRange; // 투사 끝 지점

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_KZACTION, FCollisionShape::MakeSphere(AttackRadius), Params);


	if (HitDetected) //감지가 되었을때 
	{



	}


#if ENABLE_DRAW_DEBUG

	FVector CapusleOrigin = Start + (End - Start) * 0.5f; 
	float CapsuleHalfHegiht = AttackRange * 0.5f; 
	FColor DrawColor = HitDetected ? FColor::Red : FColor::Green; 

	DrawDebugCapsule(GetWorld(), CapusleOrigin, CapsuleHalfHegiht, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif

}

