// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KZCharacterBase.generated.h"

UCLASS()
class KHAZAN_API AKZCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKZCharacterBase();


protected: 
	UCharacterMovementComponent* m_pMoveComponent = nullptr; 
	TObjectPtr<USkeletalMeshComponent>      m_pSkeletalMesh  = nullptr;
	TObjectPtr<class UKZAnimInstance>       m_pAnimInstance  = nullptr;
};
