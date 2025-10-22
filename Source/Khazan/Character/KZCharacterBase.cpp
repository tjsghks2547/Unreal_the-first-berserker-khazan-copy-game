// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KZCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/BP_Khzan.BP_Khzan_C"));
	if (AnimInstanceClassRef.Class)
	{
		m_pSkeletalMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}

