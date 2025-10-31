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
	// �̵��ϴ� ����(�ӵ� ����)�� �ٶ󺸰� ĳ������ Yaw ȸ�� �ڵ� ����
	m_pMoveComponent->RotationRate = FRotator(0.0f, 2000.f, 0.0f);	
	// bOrientRotationToMovement()�� true�� �� ���� ��ȯ �� 1�ʿ� � �ӵ�(Yaw �ӵ�) �� ȸ���� ���� ���� 
	m_pMoveComponent->JumpZVelocity = 700.f; 
	// ���� ���� �� ����(Z)���� �ִ� �ʱ�ӵ�
	m_pMoveComponent->AirControl = 0.35f; 
	// ���߿��� �̵� �Է� ���� ���� ( 0 = ���� ���� �� Ʋ�� , 1 = ����� ������ ���� ����) 
	m_pMoveComponent->MaxWalkSpeed = 1000.f; 
	// �⺻ ���� �̵� �ִ� �ӵ� (cm/s)
	m_pMoveComponent->MinAnalogWalkSpeed = 0.f; 
	// �Է��� ���� �̼��ϰ� ������ �� ����Ǵ� �ּ� �ӵ� 
	m_pMoveComponent->BrakingDecelerationWalking = 350.F;
	// Ű�� ���� �� ������ ���ߴ� ���� ���ӵ�
	m_pMoveComponent->MaxAcceleration = 100000.f;	
	// �ִ� ���ӵ��� �Է��� ������ �ش� ����ŭ �ӵ��� Ű�� 


	


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


	// �Ϲ� ���ҽ��� FObjectFinder�� ã���� ( �޽�, ���͸���, �ؽ�ó) 
	// ��� ����Ʈ ���ҽ��� FClassFinder�� ã�´� ( ��� ����Ʈ, actor, pawn �� ) 

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
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);	// �̵� ����� ����� 

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
		// ���⼭ ComboEffectiveTime ���� "second"�� ����Ǿ� second ��ŭ ���� �ڿ� ComboCheck �ش� �Լ��� �ߵ���.  
		// ���⼭ �ٽ� ComboTimerHandle�� Valid�� �Ǿ� true�� �ȴ�.
	}

}

void AKZCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate(); // ���� Ÿ�̸� �ڵ��� �ʱ�ȭ 

	if(HasNexComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		m_iCurrentCombo = FMath::Clamp(m_iCurrentCombo + 1, 1, ComboActionData->MaxComboCount);	
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, m_iCurrentCombo);

		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);

		SetComboCheckTimer();

		HasNexComboCommand = false; // �Է°� �ʱ�ȭ
	}

}

void AKZCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this); //Trace Tage : �ݸ��� �м��Ҷ��� �±� ���� , ������ ������ �浹ü ĸ���̳��� ��������, inignoreActor ������ ����(�ڱ��ڽ��̴ϱ� this����)

	const float AttackRange  = 40.0f; 
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f; 

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();  // ���� ���� ����
	const FVector End = Start + GetActorForwardVector() * AttackRange; // ���� �� ����

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_KZACTION, FCollisionShape::MakeSphere(AttackRadius), Params);


	if (HitDetected) //������ �Ǿ����� 
	{



	}


#if ENABLE_DRAW_DEBUG

	FVector CapusleOrigin = Start + (End - Start) * 0.5f; 
	float CapsuleHalfHegiht = AttackRange * 0.5f; 
	FColor DrawColor = HitDetected ? FColor::Red : FColor::Green; 

	DrawDebugCapsule(GetWorld(), CapusleOrigin, CapsuleHalfHegiht, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif

}

