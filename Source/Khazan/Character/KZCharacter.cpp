// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KZCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"	
#include "EnhancedInputSubsystems.h"	
#include "Animation/KZAnimInstance.h"
#include "Weapon/KZSpearWeapon.h"



// Sets default values
AKZCharacter::AKZCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_CameraBoom->SetupAttachment(RootComponent);
	m_CameraBoom->TargetArmLength = 800.0f;
	m_CameraBoom->SocketOffset = FVector(0.f, 0.f, 500.f);
	m_CameraBoom->bUsePawnControlRotation = true;

	m_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_FollowCamera->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);
	m_FollowCamera->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f)); // 카메라 회전 
	m_FollowCamera->bUsePawnControlRotation = false;

	//Input 
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/Input/KhazanInputMapping.KhazanInputMapping"));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Game/Input/Move/IA_Move.IA_Move"));
	if (InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveEndRef(TEXT("/Game/Input/Move/IA_MoveEnd.IA_MoveEnd"));
	if (InputActionMoveEndRef.Object)
	{
		MoveEndAction = InputActionMoveEndRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Game/Input/Look/IA_Look.IA_Look"));
	if (InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackActionRef(TEXT("/Game/Input/Attack/IA_Attack_1.IA_Attack_1"));
	if (InputActionAttackActionRef.Object)
	{
		AttackAction = InputActionAttackActionRef.Object;
	}


}

// Called when the game starts or when spawned
void AKZCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}


	FName WeaponSocket(TEXT("Weapon_R"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		FActorSpawnParameters SpawnParams; 
		SpawnParams.Owner = this; 
		SpawnParams.Instigator = this;	
		
		m_AKzWeaponSpear = GetWorld()->SpawnActor<AKZSpearWeapon>
			(
				AKZSpearWeapon::StaticClass(),
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				SpawnParams
			);

		if(m_AKzWeaponSpear)
		{
			//스케일 변경
			m_AKzWeaponSpear->SetActorScale3D(FVector(1.f, 1.f, 1.f));

			//캐릭터의 손 본(Socket)에 부착 
			m_AKzWeaponSpear->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				WeaponSocket);
		}
	}


	// 애니메이션 인스턴스 가져오기 
	m_pAnimInstance = Cast<UKZAnimInstance>(m_pSkeletalMesh->GetAnimInstance());
	




}

// Called every frame
void AKZCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//여기서 이제 해당 상태를 다뤄줘야 할듯 아무입력이 없으면 다시 phase 원상태로 돌린다던지.

}

// Called to bind functionality to input
void AKZCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction,    ETriggerEvent::Triggered, this, &AKZCharacter::Move);
	EnhancedInputComponent->BindAction(MoveEndAction, ETriggerEvent::Triggered, this, &AKZCharacter::Move_End);	
	EnhancedInputComponent->BindAction(LookAction,    ETriggerEvent::Triggered, this, &AKZCharacter::Look);
	EnhancedInputComponent->BindAction(AttackAction,  ETriggerEvent::Triggered, this, &AKZCharacter::Attack_Spear);

}

void AKZCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

	m_pAnimInstance->Initialize_PLAYER_PHASE();

	m_pAnimInstance->Set_PLAYER_PHASE(PLAYER_PHASE::PHASE_IDLE);
	m_pAnimInstance->Set_State(PLAYER_STATE::WALK);
	
}

void AKZCharacter::Move_End(const FInputActionValue& Value)	
{
	// 여기서부터 다시작성하자
	
	// story작성도 하자 기존에는 bit 플래그를 통해 안하고 
	// 이동 무브먼트로 작성할려니 좀 힘들다 이런식으로 그래서 bit 플래그로 바꿔서 한 코드 보여주기

	FVector2D MovementVector = Value.Get<FVector2D>();

	if(MovementVector.Length() < KINDA_SMALL_NUMBER)
	{
		m_pAnimInstance->Set_State(PLAYER_STATE::WALK_END);
	}


	//UE_LOG(LogTemp, Warning, TEXT("Speed2D: %f"), MovementVector.Length());	
}

void AKZCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AKZCharacter::Attack_Spear()
{
	ProcessComboCommand();

	m_pAnimInstance->Initialize_PLAYER_PHASE();	

	m_pAnimInstance->Set_State(PLAYER_STATE::NORMAL_ATTACK_1);
	m_pAnimInstance->Set_PLAYER_PHASE(PLAYER_PHASE::PHASE_FIGHT);

}

