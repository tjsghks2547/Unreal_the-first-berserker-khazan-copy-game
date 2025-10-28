// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KZCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"	
#include "EnhancedInputSubsystems.h"	
#include "Animation/KZAnimInstance.h"


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

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Move/IA_Move.IA_Move'"));
	if (InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Look/IA_Look.IA_Look'"));
	if (InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Attack/IA_Attack_1.IA_Attack_1'"));
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
		Weapon = NewObject<USkeletalMeshComponent>(this, TEXT("WEAPON"));

		USkeletalMesh* Weapon_Mesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Character/Mesh/Kazan_Weapon/ThiefSpearWeapon.ThiefSpearWeapon"));
		if (Weapon_Mesh)
		{
			Weapon->SetSkeletalMesh(Weapon_Mesh);
		}

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
		Weapon->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.35f));


		// 3) 월드에 등록 (중요!)
		Weapon->RegisterComponent();

		// 4) 소켓에 부착
		Weapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocket
		);
	}


	// 애니메이션 인스턴스 가져오기 
	m_pAnimInstance = Cast<UKZAnimInstance>(m_pSkeletalMesh->GetAnimInstance());
	




}

// Called every frame
void AKZCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKZCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKZCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKZCharacter::Look);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AKZCharacter::Attack_Spear);



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
}

