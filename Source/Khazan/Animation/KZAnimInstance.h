// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KZAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API UKZAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UKZAnimInstance();

	void Set_IsAttack(bool _OnOff) { m_bIsAttack = _OnOff; }

protected:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character);
	TObjectPtr<class AKZCharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 m_bIsIdle : 1;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 m_bIsAttack : 1 = false;


private:
	UFUNCTION()
	void AnimNotify_stopattack();

};
