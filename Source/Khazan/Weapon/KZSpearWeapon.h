// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KZSpearWeapon.generated.h"

UCLASS()
class KHAZAN_API AKZSpearWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKZSpearWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USkeletalMeshComponent> m_WeaponSkeletalMesh = nullptr;	

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> m_HitCapsule = nullptr;
	

};
