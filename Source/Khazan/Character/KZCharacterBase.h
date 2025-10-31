// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/KZAnimationAttackInterface.h"
#include "KZCharacterBase.generated.h"


UCLASS()
class KHAZAN_API AKZCharacterBase : public ACharacter,public IKZAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKZCharacterBase();


protected: 
	UCharacterMovementComponent* m_pMoveComponent = nullptr; 
	TObjectPtr<USkeletalMeshComponent>      m_pSkeletalMesh  = nullptr;
	TObjectPtr<class UKZAnimInstance>       m_pAnimInstance  = nullptr;



	//Combo Action Section 
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UKZComboActionData_1> ComboActionData; 
	
	void ProcessComboCommand(); 

	void ComboActionBegin(); 
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded); 
	void SetComboCheckTimer(); 
	void ComboCheck(); 

	int32 m_iCurrentCombo = 0;
	FTimerHandle ComboTimerHandle; // 원하는 시간에 특정 함수를 호출할도록 설정하는 것 
	
	bool HasNexComboCommand = false; 


// Attack Hit Section 

protected:
	virtual void AttackHitCheck();
};
